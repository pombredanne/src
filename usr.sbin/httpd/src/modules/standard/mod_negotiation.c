/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * Portions of this software are based upon public domain software
 * originally written at the National Center for Supercomputing Applications,
 * University of Illinois, Urbana-Champaign.
 */

/*
 * mod_negotiation.c: keeps track of MIME types the client is willing to
 * accept, and contains code to handle type arbitration.
 *
 * rst
 */

#include "httpd.h"
#include "http_config.h"
#include "http_request.h"
#include "http_protocol.h"
#include "http_core.h"
#include "http_log.h"
#include "util_script.h"

/* Commands --- configuring document caching on a per (virtual?)
 * server basis... 
 */

typedef struct {
    array_header *language_priority;
} neg_dir_config;

module MODULE_VAR_EXPORT negotiation_module;

static void *create_neg_dir_config(pool *p, char *dummy)
{
    neg_dir_config *new = (neg_dir_config *) ap_palloc(p, sizeof(neg_dir_config));

    new->language_priority = ap_make_array(p, 4, sizeof(char *));
    return new;
}

static void *merge_neg_dir_configs(pool *p, void *basev, void *addv)
{
    neg_dir_config *base = (neg_dir_config *) basev;
    neg_dir_config *add = (neg_dir_config *) addv;
    neg_dir_config *new = (neg_dir_config *) ap_palloc(p, sizeof(neg_dir_config));

    /* give priority to the config in the subdirectory */
    new->language_priority = ap_append_arrays(p, add->language_priority,
                                           base->language_priority);
    return new;
}

static const char *set_language_priority(cmd_parms *cmd, void *n, char *lang)
{
    array_header *arr = ((neg_dir_config *) n)->language_priority;
    char **langp = (char **) ap_push_array(arr);

    *langp = lang;
    return NULL;
}

static const char *cache_negotiated_docs(cmd_parms *cmd, void *dummy,
                                         char *dummy2)
{
    void *server_conf = cmd->server->module_config;

    ap_set_module_config(server_conf, &negotiation_module, "Cache");
    return NULL;
}

static int do_cache_negotiated_docs(server_rec *s)
{
    return (ap_get_module_config(s->module_config, &negotiation_module) != NULL);
}

static const command_rec negotiation_cmds[] =
{
    {"CacheNegotiatedDocs", cache_negotiated_docs, NULL, RSRC_CONF, NO_ARGS,
     "no arguments (either present or absent)"},
    {"LanguagePriority", set_language_priority, NULL, OR_FILEINFO, ITERATE,
     "space-delimited list of MIME language abbreviations"},
    {NULL}
};

/*
 * Record of available info on a media type specified by the client
 * (we also use 'em for encodings and languages)
 */

typedef struct accept_rec {
    char *name;                 /* MUST be lowercase */
    float quality;
    float level;
    char *charset;              /* for content-type only */
} accept_rec;

/*
 * Record of available info on a particular variant
 *
 * Note that a few of these fields are updated by the actual negotiation
 * code.  These are:
 *
 * level_matched --- initialized to zero.  Set to the value of level
 *             if the client actually accepts this media type at that
 *             level (and *not* if it got in on a wildcard).  See level_cmp
 *             below.
 * mime_stars -- initialized to zero.  Set to the number of stars
 *               present in the best matching Accept header element.
 *               1 for star/star, 2 for type/star and 3 for
 *               type/subtype.
 *
 * definite -- initialized to 1.  Set to 0 if there is a match which
 *             makes the variant non-definite according to the rules
 *             in rfc2296.
 */

typedef struct var_rec {
    request_rec *sub_req;       /* May be NULL (is, for map files) */
    char *mime_type;            /* MUST be lowercase */
    char *file_name;
    const char *content_encoding;
    array_header *content_languages;   /* list of languages for this variant */
    char *content_charset;
    char *description;

    /* The next five items give the quality values for the dimensions
     * of negotiation for this variant. They are obtained from the
     * appropriate header lines, except for source_quality, which
     * is obtained from the variant itself (the 'qs' parameter value
     * from the variant's mime-type). Apart from source_quality,
     * these values are set when we find the quality for each variant
     * (see best_match()). source_quality is set from the 'qs' parameter
     * of the variant description or mime type: see set_mime_fields().
     */
    float lang_quality;         /* quality of this variant's language */
    float encoding_quality;     /* ditto encoding */
    float charset_quality;      /* ditto charset */
    float mime_type_quality;    /* ditto media type */
    float source_quality;       /* source quality for this variant */

    /* Now some special values */
    float level;                /* Auxiliary to content-type... */
    float bytes;                /* content length, if known */
    int lang_index;             /* pre HTTP/1.1 language priority stuff */
    int is_pseudo_html;         /* text/html, *or* the INCLUDES_MAGIC_TYPEs */

    /* Above are all written-once properties of the variant.  The
     * three fields below are changed during negotiation:
     */

    float level_matched;
    int mime_stars;
    int definite;
} var_rec;

/* Something to carry around the state of negotiation (and to keep
 * all of this thread-safe)...
 */

typedef struct {
    pool *pool;
    request_rec *r;
    char *dir_name;
    int accept_q;               /* 1 if an Accept item has a q= param */
    float default_lang_quality; /* fiddle lang q for variants with no lang */

    /* the array pointers below are NULL if the corresponding accept
     * headers are not present
     */
    array_header *accepts;            /* accept_recs */
    array_header *accept_encodings;   /* accept_recs */
    array_header *accept_charsets;    /* accept_recs */
    array_header *accept_langs;       /* accept_recs */

    array_header *avail_vars;         /* available variants */

    int count_multiviews_variants;    /* number of variants found on disk */

    int is_transparent;       /* 1 if this resource is trans. negotiable */

    int dont_fiddle_headers;  /* 1 if we may not fiddle with accept hdrs */
    int ua_supports_trans;    /* 1 if ua supports trans negotiation */
    int send_alternates;      /* 1 if we want to send an Alternates header */
    int may_choose;           /* 1 if we may choose a variant for the client */
    int use_rvsa;             /* 1 if we must use RVSA/1.0 negotiation algo */
} negotiation_state;

/* A few functions to manipulate var_recs.
 * Cleaning out the fields...
 */

static void clean_var_rec(var_rec *mime_info)
{
    mime_info->sub_req = NULL;
    mime_info->mime_type = "";
    mime_info->file_name = "";
    mime_info->content_encoding = NULL;
    mime_info->content_languages = NULL;
    mime_info->content_charset = "";
    mime_info->description = "";

    mime_info->is_pseudo_html = 0;
    mime_info->level = 0.0f;
    mime_info->level_matched = 0.0f;
    mime_info->bytes = 0.0f;
    mime_info->lang_index = -1;
    mime_info->mime_stars = 0;
    mime_info->definite = 1;

    mime_info->charset_quality = 1.0f;
    mime_info->encoding_quality = 1.0f;
    mime_info->lang_quality = 1.0f;
    mime_info->mime_type_quality = 1.0f;
    mime_info->source_quality = 0.0f;
}

/* Initializing the relevant fields of a variant record from the
 * accept_info read out of its content-type, one way or another.
 */

static void set_mime_fields(var_rec *var, accept_rec *mime_info)
{
    var->mime_type = mime_info->name;
    var->source_quality = mime_info->quality;
    var->level = mime_info->level;
    var->content_charset = mime_info->charset;

    var->is_pseudo_html = (!strcmp(var->mime_type, "text/html")
                           || !strcmp(var->mime_type, INCLUDES_MAGIC_TYPE)
                           || !strcmp(var->mime_type, INCLUDES_MAGIC_TYPE3));
}

/* Create a variant list validator in r using info from vlistr. */

static void set_vlist_validator(request_rec *r, request_rec *vlistr)
{
    /* Calculating the variant list validator is similar to
     * calculating an etag for the source of the variant list
     * information, so we use ap_make_etag().  Note that this
     * validator can be 'weak' in extreme case.
     */

    ap_update_mtime (vlistr, vlistr->finfo.st_mtime);
    r->vlist_validator = ap_make_etag(vlistr, 0);

    /* ap_set_etag will later take r->vlist_validator into account
     * when creating the etag header
     */
}


/*****************************************************************
 *
 * Parsing (lists of) media types and their parameters, as seen in
 * HTTPD header lines and elsewhere.
 */

/*
 * Get a single mime type entry --- one media type and parameters;
 * enter the values we recognize into the argument accept_rec
 */

static const char *get_entry(pool *p, accept_rec *result,
                             const char *accept_line)
{
    result->quality = 1.0f;
    result->level = 0.0f;
    result->charset = "";

    /*
     * Note that this handles what I gather is the "old format",
     *
     *    Accept: text/html text/plain moo/zot
     *
     * without any compatibility kludges --- if the token after the
     * MIME type begins with a semicolon, we know we're looking at parms,
     * otherwise, we know we aren't.  (So why all the pissing and moaning
     * in the CERN server code?  I must be missing something).
     */

    result->name = ap_get_token(p, &accept_line, 0);
    ap_str_tolower(result->name);     /* You want case-insensitive,
                                       * you'll *get* case-insensitive.
                                       */

    /* KLUDGE!!! Default HTML to level 2.0 unless the browser
     * *explicitly* says something else.
     */

    if (!strcmp(result->name, "text/html") && (result->level == 0.0)) {
        result->level = 2.0f;
    }
    else if (!strcmp(result->name, INCLUDES_MAGIC_TYPE)) {
        result->level = 2.0f;
    }
    else if (!strcmp(result->name, INCLUDES_MAGIC_TYPE3)) {
        result->level = 3.0f;
    }

    while (*accept_line == ';') {
        /* Parameters ... */

        char *parm;
        char *cp;
        char *end;

        ++accept_line;
        parm = ap_get_token(p, &accept_line, 1);

        /* Look for 'var = value' --- and make sure the var is in lcase. */

        for (cp = parm; (*cp && !ap_isspace(*cp) && *cp != '='); ++cp) {
            *cp = ap_tolower(*cp);
        }

        if (!*cp) {
            continue;           /* No '='; just ignore it. */
        }

        *cp++ = '\0';           /* Delimit var */
        while (*cp && (ap_isspace(*cp) || *cp == '=')) {
            ++cp;
        }

        if (*cp == '"') {
            ++cp;
            for (end = cp;
                 (*end && *end != '\n' && *end != '\r' && *end != '\"');
                 end++);
        }
        else {
            for (end = cp; (*end && !ap_isspace(*end)); end++);
        }
        if (*end) {
            *end = '\0';        /* strip ending quote or return */
        }
        ap_str_tolower(cp);

        if (parm[0] == 'q'
            && (parm[1] == '\0' || (parm[1] == 's' && parm[2] == '\0'))) {
            result->quality = (float)atof(cp);
        }
        else if (parm[0] == 'l' && !strcmp(&parm[1], "evel")) {
            result->level = (float)atof(cp);
        }
        else if (!strcmp(parm, "charset")) {
            result->charset = cp;
        }
    }

    if (*accept_line == ',') {
        ++accept_line;
    }

    return accept_line;
}

/*****************************************************************
 *
 * Dealing with header lines ...
 *
 * Accept, Accept-Charset, Accept-Language and Accept-Encoding
 * are handled by do_header_line() - they all have the same
 * basic structure of a list of items of the format
 *    name; q=N; charset=TEXT
 *
 * where charset is only valid in Accept.
 */

static array_header *do_header_line(pool *p, const char *accept_line)
{
    array_header *accept_recs;

    if (!accept_line) {
        return NULL;
    }

    accept_recs = ap_make_array(p, 40, sizeof(accept_rec));

    while (*accept_line) {
        accept_rec *new = (accept_rec *) ap_push_array(accept_recs);
        accept_line = get_entry(p, new, accept_line);
    }

    return accept_recs;
}

/* Given the text of the Content-Languages: line from the var map file,
 * return an array containing the languages of this variant
 */

static array_header *do_languages_line(pool *p, const char **lang_line)
{
    array_header *lang_recs = ap_make_array(p, 2, sizeof(char *));

    if (!lang_line) {
        return lang_recs;
    }

    while (**lang_line) {
        char **new = (char **) ap_push_array(lang_recs);
        *new = ap_get_token(p, lang_line, 0);
        ap_str_tolower(*new);
        if (**lang_line == ',' || **lang_line == ';') {
            ++(*lang_line);
        }
    }

    return lang_recs;
}

/*****************************************************************
 *
 * Handling header lines from clients...
 */

static negotiation_state *parse_accept_headers(request_rec *r)
{
    negotiation_state *new =
        (negotiation_state *) ap_pcalloc(r->pool, sizeof(negotiation_state));
    accept_rec *elts;
    table *hdrs = r->headers_in;
    int i;

    new->pool = r->pool;
    new->r = r;
    new->dir_name = ap_make_dirstr_parent(r->pool, r->filename);

    new->accepts = do_header_line(r->pool, ap_table_get(hdrs, "Accept"));

    /* calculate new->accept_q value */
    if (new->accepts) {
        elts = (accept_rec *) new->accepts->elts;

        for (i = 0; i < new->accepts->nelts; ++i) {
            if (elts[i].quality < 1.0) {
                new->accept_q = 1;
            }
        }
    }

    new->accept_encodings =
        do_header_line(r->pool, ap_table_get(hdrs, "Accept-Encoding"));
    new->accept_langs =
        do_header_line(r->pool, ap_table_get(hdrs, "Accept-Language"));
    new->accept_charsets =
        do_header_line(r->pool, ap_table_get(hdrs, "Accept-Charset"));

    new->avail_vars = ap_make_array(r->pool, 40, sizeof(var_rec));

    return new;
}


static void parse_negotiate_header(request_rec *r, negotiation_state *neg)
{
    const char *negotiate = ap_table_get(r->headers_in, "Negotiate");
    char *tok;
    
    /* First, default to no TCN, no Alternates, and the original Apache
     * negotiation algorithm with fiddles for broken browser configs.
     *
     * To save network bandwidth, we do not configure to send an
     * Alternates header to the user agent by default.  User
     * agents that want an Alternates header for agent-driven
     * negotiation will have to request it by sending an
     * appropriate Negotiate header.
     */
    neg->ua_supports_trans   = 0;
    neg->send_alternates     = 0;
    neg->may_choose          = 1;
    neg->use_rvsa            = 0;
    neg->dont_fiddle_headers = 0;

    if (!negotiate)
        return;

    if (strcmp(negotiate, "trans") == 0) {
        /* Lynx 2.7 and 2.8 send 'negotiate: trans' even though they
         * do not support transparent content negotiation, so for Lynx we
         * ignore the negotiate header when its contents are exactly "trans".
         * If future versions of Lynx ever need to say 'negotiate: trans',
         * they can send the equivalent 'negotiate: trans, trans' instead
         * to avoid triggering the workaround below. 
         */
        const char *ua = ap_table_get(r->headers_in, "User-Agent");

        if (ua && (strncmp(ua, "Lynx", 4) == 0))
            return;
    }

    neg->may_choose = 0;  /* An empty Negotiate would require 300 response */

    while ((tok = ap_get_list_item(neg->pool, &negotiate)) != NULL) {

        if (strcmp(tok, "trans") == 0 ||
            strcmp(tok, "vlist") == 0 ||
            strcmp(tok, "guess-small") == 0 ||
            ap_isdigit(tok[0]) ||
            strcmp(tok, "*") == 0) {

            /* The user agent supports transparent negotiation */
            neg->ua_supports_trans = 1;

            /* Send-alternates could be configurable, but note
             * that it must be 1 if we have 'vlist' in the
             * negotiate header.
             */
            neg->send_alternates = 1;

            if (strcmp(tok, "1.0") == 0) {
                /* we may use the RVSA/1.0 algorithm, configure for it */
                neg->may_choose = 1;
                neg->use_rvsa = 1;
                neg->dont_fiddle_headers = 1;
            }
            else if (tok[0] == '*') {
                /* we may use any variant selection algorithm, configure
                 * to use the Apache algorithm
                 */
                neg->may_choose = 1;
                
                /* We disable header fiddles on the assumption that a
                 * client sending Negotiate knows how to send correct
                 * headers which don't need fiddling.
                 */
                neg->dont_fiddle_headers = 1; 
            }
        }
    }

#ifdef NEG_DEBUG
    fprintf(stderr, "dont_fiddle_headers=%d use_rvsa=%d ua_supports_trans=%d "
            "send_alternates=%d, may_choose=%d\n",
            neg->dont_fiddle_headers, neg->use_rvsa,  
            neg->ua_supports_trans, neg->send_alternates, neg->may_choose);
#endif

}

/* Sometimes clients will give us no Accept info at all; this routine sets
 * up the standard default for that case, and also arranges for us to be
 * willing to run a CGI script if we find one.  (In fact, we set up to
 * dramatically prefer CGI scripts in cases where that's appropriate,
 * e.g., POST or when URI includes query args or extra path info).
 */
static void maybe_add_default_accepts(negotiation_state *neg, 
                                      int prefer_scripts)
{
    accept_rec *new_accept;

    if (!neg->accepts) {
        neg->accepts = ap_make_array(neg->pool, 4, sizeof(accept_rec));

        new_accept = (accept_rec *) ap_push_array(neg->accepts);
        
        new_accept->name = "*/*";
        new_accept->quality = 1.0f;
        new_accept->level = 0.0f;
    }    

    new_accept = (accept_rec *) ap_push_array(neg->accepts);

    new_accept->name = CGI_MAGIC_TYPE;
    if (neg->use_rvsa) {
        new_accept->quality = 0;
    }
    else {
        new_accept->quality = prefer_scripts ? 2.0f : 0.001f;
    }
    new_accept->level = 0.0f;
}

/*****************************************************************
 *
 * Parsing type-map files, in Roy's meta/http format augmented with
 * #-comments.
 */

/* Reading RFC822-style header lines, ignoring #-comments and
 * handling continuations.
 */

enum header_state {
    header_eof, header_seen, header_sep
};

static enum header_state get_header_line(char *buffer, int len, FILE *map)
{
    char *buf_end = buffer + len;
    char *cp;
    int c;

    /* Get a noncommented line */

    do {
        if (fgets(buffer, MAX_STRING_LEN, map) == NULL) {
            return header_eof;
        }
    } while (buffer[0] == '#');

    /* If blank, just return it --- this ends information on this variant */

    for (cp = buffer; (*cp && ap_isspace(*cp)); ++cp) {
        continue;
    }

    if (*cp == '\0') {
        return header_sep;
    }

    /* If non-blank, go looking for header lines, but note that we still
     * have to treat comments specially...
     */

    cp += strlen(cp);

    while ((c = getc(map)) != EOF) {
        if (c == '#') {
            /* Comment line */
            while ((c = getc(map)) != EOF && c != '\n') {
                continue;
            }
        }
        else if (ap_isspace(c)) {
            /* Leading whitespace.  POSSIBLE continuation line
             * Also, possibly blank --- if so, we ungetc() the final newline
             * so that we will pick up the blank line the next time 'round.
             */

            while (c != EOF && c != '\n' && ap_isspace(c)) {
                c = getc(map);
            }

            ungetc(c, map);

            if (c == '\n') {
                return header_seen;     /* Blank line */
            }

            /* Continuation */

            while (cp < buf_end - 2 && (c = getc(map)) != EOF && c != '\n') {
                *cp++ = c;
            }

            *cp++ = '\n';
            *cp = '\0';
        }
        else {

            /* Line beginning with something other than whitespace */

            ungetc(c, map);
            return header_seen;
        }
    }

    return header_seen;
}

/* Stripping out RFC822 comments */

static void strip_paren_comments(char *hdr)
{
    /* Hmmm... is this correct?  In Roy's latest draft, (comments) can nest! */
    /* Nope, it isn't correct.  Fails to handle backslash escape as well.    */

    while (*hdr) {
        if (*hdr == '"') {
            hdr = strchr(hdr, '"');
            if (hdr == NULL) {
                return;
            }
            ++hdr;
        }
        else if (*hdr == '(') {
            while (*hdr && *hdr != ')') {
                *hdr++ = ' ';
            }

            if (*hdr) {
                *hdr++ = ' ';
            }
        }
        else {
            ++hdr;
        }
    }
}

/* Getting to a header body from the header */

static char *lcase_header_name_return_body(char *header, request_rec *r)
{
    char *cp = header;

    for ( ; *cp && *cp != ':' ; ++cp) {
        *cp = ap_tolower(*cp);
    }

    if (!*cp) {
        ap_log_rerror(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, r,
                      "Syntax error in type map --- no ':': %s", r->filename);
        return NULL;
    }

    do {
        ++cp;
    } while (*cp && ap_isspace(*cp));

    if (!*cp) {
        ap_log_rerror(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, r,
                      "Syntax error in type map --- no header body: %s",
                      r->filename);
        return NULL;
    }

    return cp;
}

static int read_type_map(negotiation_state *neg, request_rec *rr)
{
    request_rec *r = neg->r;
    FILE *map;
    char buffer[MAX_STRING_LEN];
    enum header_state hstate;
    struct var_rec mime_info;
    int has_content;

    /* We are not using multiviews */
    neg->count_multiviews_variants = 0;

    map = ap_pfopen(neg->pool, rr->filename, "r");
    if (map == NULL) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, r,
                      "cannot access type map file: %s", rr->filename);
        return HTTP_FORBIDDEN;
    }

    clean_var_rec(&mime_info);
    has_content = 0;

    do {
        hstate = get_header_line(buffer, MAX_STRING_LEN, map);

        if (hstate == header_seen) {
            char *body1 = lcase_header_name_return_body(buffer, neg->r);
            const char *body;

            if (body1 == NULL) {
                return SERVER_ERROR;
            }

            strip_paren_comments(body1);
            body = body1;

            if (!strncmp(buffer, "uri:", 4)) {
                mime_info.file_name = ap_get_token(neg->pool, &body, 0);
            }
            else if (!strncmp(buffer, "content-type:", 13)) {
                struct accept_rec accept_info;

                get_entry(neg->pool, &accept_info, body);
                set_mime_fields(&mime_info, &accept_info);
                has_content = 1;
            }
            else if (!strncmp(buffer, "content-length:", 15)) {
                mime_info.bytes = (float)atof(body);
                has_content = 1;
            }
            else if (!strncmp(buffer, "content-language:", 17)) {
                mime_info.content_languages = do_languages_line(neg->pool,
                                                                &body);
                has_content = 1;
            }
            else if (!strncmp(buffer, "content-encoding:", 17)) {
                mime_info.content_encoding = ap_get_token(neg->pool, &body, 0);
                has_content = 1;
            }
            else if (!strncmp(buffer, "description:", 12)) {
                char *desc = ap_pstrdup(neg->pool, body);
                char *cp;

                for (cp = desc; *cp; ++cp) {
                    if (*cp=='\n') *cp=' ';
                }
                if (cp>desc) *(cp-1)=0;
                mime_info.description = desc;
            }
        }
        else {
            if (*mime_info.file_name && has_content) {
                void *new_var = ap_push_array(neg->avail_vars);

                memcpy(new_var, (void *) &mime_info, sizeof(var_rec));
            }

            clean_var_rec(&mime_info);
            has_content = 0;
        }
    } while (hstate != header_eof);

    ap_pfclose(neg->pool, map);

    set_vlist_validator(r, rr);

    return OK;
}


/* Sort function used by read_types_multi. */
static int variantsortf(var_rec *a, var_rec *b) {

    /* First key is the source quality, sort in descending order. */

    /* XXX: note that we currently implement no method of setting the
     * source quality for multiviews variants, so we are always comparing
     * 1.0 to 1.0 for now
     */
    if (a->source_quality < b->source_quality)
        return 1;
    if (a->source_quality > b->source_quality)
        return -1;

    /* Second key is the variant name */
    return strcmp(a->file_name, b->file_name);
}

/*****************************************************************
 *
 * Same as read_type_map, except we use a filtered directory listing
 * as the map...  
 */

static int read_types_multi(negotiation_state *neg)
{
    request_rec *r = neg->r;

    char *filp;
    int prefix_len;
    DIR *dirp;
    struct DIR_TYPE *dir_entry;
    struct var_rec mime_info;
    struct accept_rec accept_info;
    void *new_var;
    struct { int any, all; } forbidden;

    clean_var_rec(&mime_info);

    if (!(filp = strrchr(r->filename, '/'))) {
        return DECLINED;        /* Weird... */
    }

    if (strncmp(r->filename, "proxy:", 6) == 0) {
        return DECLINED;
    }

    ++filp;
    prefix_len = strlen(filp);

    dirp = ap_popendir(neg->pool, neg->dir_name);

    if (dirp == NULL) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, r,
                    "cannot read directory for multi: %s", neg->dir_name);
        return HTTP_FORBIDDEN;
    }

    forbidden.any = 0;
    forbidden.all = 1;

    while ((dir_entry = readdir(dirp))) {
        array_header *exception_list;
        request_rec *sub_req;

        /* Do we have a match? */
        if (strncmp(dir_entry->d_name, filp, prefix_len)) {
            continue;
        }
        if (dir_entry->d_name[prefix_len] != '.') {
            continue;
        }

        /* Yep.  See if it's something which we have access to, and 
         * which has a known type and encoding (as opposed to something
         * which we'll be slapping default_type on later).
         */

        sub_req = ap_sub_req_lookup_file(dir_entry->d_name, r);

        /* If it has a handler, we'll pretend it's a CGI script,
         * since that's a good indication of the sort of thing it
         * might be doing.
         */
        if (sub_req->handler && !sub_req->content_type) {
            sub_req->content_type = CGI_MAGIC_TYPE;
        }

        /* HTTP_FORBIDDEN is returned, e.g., if the path length limit was exceeded */
        /* HTTP_OK does NOT necessarily mean that the file is really readable! */
        if (sub_req->status == HTTP_OK)
            forbidden.all = 0;
        else if (sub_req->status == HTTP_FORBIDDEN)
            forbidden.any = 1;

        /* 
         * mod_mime will _always_ provide us the base name in the
         * ap-mime-exception-list, if it processed anything.  If
         * this list is empty, give up immediately, there was
         * nothing interesting.  For example, looking at the files
         * readme.txt and readme.foo, we will throw away .foo if
         * it's an insignificant file (e.g. did not identify a 
         * language, charset, encoding, content type or handler,)
         */
        exception_list = 
            (array_header *) ap_table_get(sub_req->notes,
                                          "ap-mime-exceptions-list");
        if (!exception_list) {
            ap_destroy_sub_req(sub_req);
            continue;
        }

        /* Each unregonized bit better match our base name, in sequence.
         * A test of index.html.foo will match index.foo or index.html.foo,
         * but it will never transpose the segments and allow index.foo.html
         * because that would introduce too much CPU consumption.  Better that
         * we don't attempt a many-to-many match here.
         */
        {
            int nexcept = exception_list->nelts;
            char **cur_except = (char**)exception_list->elts;
            char *segstart = filp, *segend, saveend;

            while (*segstart && nexcept) {
                if (!(segend = strchr(segstart, '.')))
                    segend = strchr(segstart, '\0');
                saveend = *segend;
                *segend = '\0';

#ifdef CASE_BLIND_FILESYSTEM
                if (strcasecmp(segstart, *cur_except) == 0) {
#else
                if (strcmp(segstart, *cur_except) == 0) {
#endif
                    --nexcept;
                    ++cur_except;
                }

                if (!saveend)
                    break;

                *segend = saveend;
                segstart = segend + 1;
            }

            if (nexcept) {
                /* Something you don't know is, something you don't know...
                 */
                ap_destroy_sub_req(sub_req);
                continue;
            }
        }

        /* 
         * ###: be warned, the _default_ content type is already
         * picked up here!  If we failed the subrequest, or don't 
         * know what we are serving, then continue.
         */
        if (sub_req->status != HTTP_OK || (!sub_req->content_type)) {
            ap_destroy_sub_req(sub_req);
            continue;
        }

        /* If it's a map file, we use that instead of the map
         * we're building...
         */
        if (((sub_req->content_type) &&
             !strcmp(sub_req->content_type, MAP_FILE_MAGIC_TYPE)) ||
            ((sub_req->handler) &&
             !strcmp(sub_req->handler, "type-map"))) {

            ap_pclosedir(neg->pool, dirp);
            neg->avail_vars->nelts = 0;
            if (sub_req->status != HTTP_OK) {
                return sub_req->status;
            }
            return read_type_map(neg, sub_req);
        }

        /* Have reasonable variant --- gather notes. */

        mime_info.sub_req = sub_req;
        mime_info.file_name = ap_pstrdup(neg->pool, dir_entry->d_name);
        if (sub_req->content_encoding) {
            mime_info.content_encoding = sub_req->content_encoding;
        }
        if (sub_req->content_languages) {
            mime_info.content_languages = sub_req->content_languages;
        }

        get_entry(neg->pool, &accept_info, sub_req->content_type);
        set_mime_fields(&mime_info, &accept_info);

        new_var = ap_push_array(neg->avail_vars);
        memcpy(new_var, (void *) &mime_info, sizeof(var_rec));

        neg->count_multiviews_variants++;

        clean_var_rec(&mime_info);
    }

    ap_pclosedir(neg->pool, dirp);

    /* If all variants we considered turn out to be forbidden, then return FORBIDDEN */
    if (forbidden.any && forbidden.all)
        return HTTP_FORBIDDEN;

    set_vlist_validator(r, r);

    /* Sort the variants into a canonical order.  The negotiation
     * result sometimes depends on the order of the variants.  By
     * sorting the variants into a canonical order, rather than using
     * the order in which readdir() happens to return them, we ensure
     * that the negotiation result will be consistent over filesystem
     * backup/restores and over all mirror sites.
     */
       
    qsort((void *) neg->avail_vars->elts, neg->avail_vars->nelts,
          sizeof(var_rec), (int (*)(const void *, const void *)) variantsortf);

    return OK;
}


/*****************************************************************
 * And now for the code you've been waiting for... actually
 * finding a match to the client's requirements.  
 */

/* Matching MIME types ... the star/star and foo/star commenting conventions
 * are implemented here.  (You know what I mean by star/star, but just
 * try mentioning those three characters in a C comment).  Using strcmp()
 * is legit, because everything has already been smashed to lowercase.
 *
 * Note also that if we get an exact match on the media type, we update
 * level_matched for use in level_cmp below...
 * 
 * We also give a value for mime_stars, which is used later. It should
 * be 1 for star/star, 2 for type/star and 3 for type/subtype.
 */

static int mime_match(accept_rec *accept_r, var_rec *avail)
{
    char *accept_type = accept_r->name;
    char *avail_type = avail->mime_type;
    int len = strlen(accept_type);

    if (accept_type[0] == '*') {        /* Anything matches star/star */
        if (avail->mime_stars < 1) {
            avail->mime_stars = 1;
        }
        return 1;
    }
    else if ((accept_type[len - 1] == '*') &&
             !strncmp(accept_type, avail_type, len - 2)) {
        if (avail->mime_stars < 2) {
            avail->mime_stars = 2;
        }
        return 1;
    }
    else if (!strcmp(accept_type, avail_type)
             || (!strcmp(accept_type, "text/html")
                 && (!strcmp(avail_type, INCLUDES_MAGIC_TYPE)
                     || !strcmp(avail_type, INCLUDES_MAGIC_TYPE3)))) {
        if (accept_r->level >= avail->level) {
            avail->level_matched = avail->level;
            avail->mime_stars = 3;
            return 1;
        }
    }

    return OK;
}

/* This code implements a piece of the tie-breaking algorithm between
 * variants of equal quality.  This piece is the treatment of variants
 * of the same base media type, but different levels.  What we want to
 * return is the variant at the highest level that the client explicitly
 * claimed to accept.
 *
 * If all the variants available are at a higher level than that, or if
 * the client didn't say anything specific about this media type at all
 * and these variants just got in on a wildcard, we prefer the lowest
 * level, on grounds that that's the one that the client is least likely
 * to choke on.
 *
 * (This is all motivated by treatment of levels in HTML --- we only
 * want to give level 3 to browsers that explicitly ask for it; browsers
 * that don't, including HTTP/0.9 browsers that only get the implicit
 * "Accept: * / *" [space added to avoid confusing cpp --- no, that
 * syntax doesn't really work] should get HTML2 if available).
 *
 * (Note that this code only comes into play when we are choosing among
 * variants of equal quality, where the draft standard gives us a fair
 * bit of leeway about what to do.  It ain't specified by the standard;
 * rather, it is a choice made by this server about what to do in cases
 * where the standard does not specify a unique course of action).
 */

static int level_cmp(var_rec *var1, var_rec *var2)
{
    /* Levels are only comparable between matching media types */

    if (var1->is_pseudo_html && !var2->is_pseudo_html) {
        return 0;
    }

    if (!var1->is_pseudo_html && strcmp(var1->mime_type, var2->mime_type)) {
        return 0;
    }
    /* The result of the above if statements is that, if we get to
     * here, both variants have the same mime_type or both are
     * pseudo-html.
     */    

    /* Take highest level that matched, if either did match. */

    if (var1->level_matched > var2->level_matched) {
        return 1;
    }
    if (var1->level_matched < var2->level_matched) {
        return -1;
    }

    /* Neither matched.  Take lowest level, if there's a difference. */

    if (var1->level < var2->level) {
        return 1;
    }
    if (var1->level > var2->level) {
        return -1;
    }

    /* Tied */

    return 0;
}

/* Finding languages.  The main entry point is set_language_quality()
 * which is called for each variant. It sets two elements in the
 * variant record:
 *    language_quality  - the 'q' value of the 'best' matching language
 *                        from Accept-Language: header (HTTP/1.1)
 *    lang_index    -     Pre HTTP/1.1 language priority, using
 *                        position of language on the Accept-Language:
 *                        header, if present, else LanguagePriority
 *                        directive order.
 *
 * When we do the variant checking for best variant, we use language
 * quality first, and if a tie, language_index next (this only applies
 * when _not_ using the RVSA/1.0 algorithm). If using the RVSA/1.0
 * algorithm, lang_index is never used.
 *
 * set_language_quality() calls find_lang_index() and find_default_index()
 * to set lang_index.  
 */

static int find_lang_index(array_header *accept_langs, char *lang)
{
    accept_rec *accs;
    int i;

    if (!lang || !accept_langs) {
        return -1;
    }

    accs = (accept_rec *) accept_langs->elts;

    for (i = 0; i < accept_langs->nelts; ++i) {
        if (!strncmp(lang, accs[i].name, strlen(accs[i].name))) {
            return i;
        }
    }

    return -1;
}

/* This function returns the priority of a given language
 * according to LanguagePriority.  It is used in case of a tie
 * between several languages.
 */

static int find_default_index(neg_dir_config *conf, char *lang)
{
    array_header *arr;
    int nelts;
    char **elts;
    int i;

    if (!lang) {
        return -1;
    }

    arr = conf->language_priority;
    nelts = arr->nelts;
    elts = (char **) arr->elts;

    for (i = 0; i < nelts; ++i) {
        if (!strcasecmp(elts[i], lang)) {
            return i;
        }
    }

    return -1;
}

/* set_default_lang_quality() sets the quality we apply to variants
 * which have no language assigned to them. If none of the variants
 * have a language, we are not negotiating on language, so all are
 * acceptable, and we set the default q value to 1.0. However if
 * some of the variants have languages, we set this default to 0.001.
 * The value of this default will be applied to all variants with
 * no explicit language -- which will have the effect of making them
 * acceptable, but only if no variants with an explicit language
 * are acceptable. The default q value set here is assigned to variants
 * with no language type in set_language_quality().
 *
 * Note that if using the RVSA/1.0 algorithm, we don't use this
 * fiddle.  
 */

static void set_default_lang_quality(negotiation_state *neg)
{
    var_rec *avail_recs = (var_rec *) neg->avail_vars->elts;
    int j;

    if (!neg->dont_fiddle_headers) {
        for (j = 0; j < neg->avail_vars->nelts; ++j) {
            var_rec *variant = &avail_recs[j];
            if (variant->content_languages &&
                variant->content_languages->nelts) {
                neg->default_lang_quality = 0.001f;
                return;
            }
        }
    }

    neg->default_lang_quality = 1.0f;
}

/* Set the language_quality value in the variant record. Also
 * assigns lang_index for back-compat. 
 *
 * To find the language_quality value, we look for the 'q' value
 * of the 'best' matching language on the Accept-Language
 * header. The 'best' match is the language on Accept-Language
 * header which matches the language of this variant either fully,
 * or as far as the prefix marker (-). If two or more languages
 * match, use the longest string from the Accept-Language header
 * (see HTTP/1.1 [14.4])
 *
 * When a variant has multiple languages, we find the 'best'
 * match for each variant language tag as above, then select the
 * one with the highest q value. Because both the accept-header
 * and variant can have multiple languages, we now have a hairy
 * loop-within-a-loop here.
 *
 * If the variant has no language and we have no Accept-Language
 * items, leave the quality at 1.0 and return.
 *
 * If the variant has no language, we use the default as set by
 * set_default_lang_quality() (1.0 if we are not negotiating on
 * language, 0.001 if we are).
 *
 * Following the setting of the language quality, we drop through to
 * set the old 'lang_index'. This is set based on either the order
 * of the languages on the Accept-Language header, or the
 * order on the LanguagePriority directive. This is only used
 * in the negotiation if the language qualities tie.
 */

static void set_language_quality(negotiation_state *neg, var_rec *variant)
{
    char *firstlang;
    int idx;

    if (!variant->content_languages || !variant->content_languages->nelts) {
        /* This variant has no content-language, so use the default
         * quality factor for variants with no content-language
         * (previously set by set_default_lang_quality()).
         * Leave the factor alone (it remains at 1.0) when we may not fiddle
         * with the headers.
         */
        if (!neg->dont_fiddle_headers) {
            variant->lang_quality = neg->default_lang_quality;
        }
        if (!neg->accept_langs) {
            return;             /* no accept-language header */
        }

    }
    else {
        /* Variant has one (or more) languages.  Look for the best
         * match. We do this by going through each language on the
         * variant description looking for a match on the
         * Accept-Language header. The best match is the longest
         * matching language on the header. The final result is the
         * best q value from all the languages on the variant
         * description.
         */

        if (!neg->accept_langs) {
            /* no accept-language header makes the variant indefinite */
            variant->definite = 0;
        }
        else {    /* There is an accept-language with 0 or more items */
            accept_rec *accs = (accept_rec *) neg->accept_langs->elts;
            accept_rec *best = NULL, *star = NULL;
            accept_rec *bestthistag;
            char *lang, *p;
            float fiddle_q = 0.0f;
            int any_match_on_star = 0;
            int i, j, alen, longest_lang_range_len;

            for (j = 0; j < variant->content_languages->nelts; ++j) {
                p = NULL;
                bestthistag = NULL;
                longest_lang_range_len = 0;
                alen = 0;
                
                /* lang is the variant's language-tag, which is the one
                 * we are allowed to use the prefix of in HTTP/1.1
                 */
                lang = ((char **) (variant->content_languages->elts))[j];
                
                /* now find the best (i.e. longest) matching
                 * Accept-Language header language. We put the best match
                 * for this tag in bestthistag. We cannot update the
                 * overall best (based on q value) because the best match
                 * for this tag is the longest language item on the accept
                 * header, not necessarily the highest q.
                 */
                for (i = 0; i < neg->accept_langs->nelts; ++i) {
                    if (!strcmp(accs[i].name, "*")) {
                        if (!star) {
                            star = &accs[i];
                        }
                        continue;
                    }
                    /* Find language. We match if either the variant
                     * language tag exactly matches the language range
                     * from the accept header, or a prefix of the variant
                     * language tag up to a '-' character matches the
                     * whole of the language range in the Accept-Language
                     * header.  Note that HTTP/1.x allows any number of
                     * '-' characters in a tag or range, currently only
                     * tags with zero or one '-' characters are defined
                     * for general use (see rfc1766).
                     *  
                     * We only use language range in the Accept-Language
                     * header the best match for the variant language tag
                     * if it is longer than the previous best match.
                     */
                    
                    alen = strlen(accs[i].name);
                    
                    if (((int)strlen(lang) >= alen) &&
                        !strncmp(lang, accs[i].name, alen) &&
                        ((lang[alen] == 0) || (lang[alen] == '-')) ) {
                        
                        if (alen > longest_lang_range_len) {
                            longest_lang_range_len = alen;
                            bestthistag = &accs[i];
                        }
                    }
                    
                    if (!bestthistag && !neg->dont_fiddle_headers) {
                        /* The next bit is a fiddle. Some browsers might
                         * be configured to send more specific language
                         * ranges than desirable. For example, an
                         * Accept-Language of en-US should never match
                         * variants with languages en or en-GB. But US
                         * English speakers might pick en-US as their
                         * language choice.  So this fiddle checks if the
                         * language range has a prefix, and if so, it
                         * matches variants which match that prefix with a
                         * priority of 0.001. So a request for en-US would
                         * match variants of types en and en-GB, but at
                         * much lower priority than matches of en-US
                         * directly, or of any other language listed on
                         * the Accept-Language header. Note that this
                         * fiddle does not handle multi-level prefixes.
                         */
                        if ((p = strchr(accs[i].name, '-'))) {
                            int plen = p - accs[i].name;

                            if (!strncmp(lang, accs[i].name, plen)) {
                                fiddle_q = 0.001f;
                            }
                        }
                    }
                }
                /* Finished looking at Accept-Language headers, the best
                 * (longest) match is in bestthistag, or NULL if no match
                 */
                if (!best ||
                    (bestthistag && bestthistag->quality > best->quality)) {
                    best = bestthistag;
                }
                
                /* See if the tag matches on a * in the Accept-Language
                 * header. If so, record this fact for later use 
                 */
                if (!bestthistag && star) {
                    any_match_on_star = 1;
                }
            }
            
            /* If one of the language tags of the variant matched on *, we
             * need to see if its q is better than that of any non-* match
             * on any other tag of the variant.  If so the * match takes
             * precedence and the overall match is not definite.
             */
            if ( any_match_on_star &&
                ((best && star->quality > best->quality) ||
                 (!best)) ) {
                best = star;
                variant->definite = 0;
            }
            
            variant->lang_quality = best ? best->quality : fiddle_q;
        }
    }

    /* Now set the old lang_index field. Since this is old 
     * stuff anyway, don't bother with handling multiple languages
     * per variant, just use the first one assigned to it 
     */
    idx = 0;
    if (variant->content_languages && variant->content_languages->nelts) {
        firstlang = ((char **) variant->content_languages->elts)[0];
    }
    else {
        firstlang = "";
    }
    if (!neg->accept_langs) {   /* Client doesn't care */
        idx = find_default_index((neg_dir_config *) ap_get_module_config(
                                  neg->r->per_dir_config, &negotiation_module),
                                 firstlang);
    }
    else {                      /* Client has Accept-Language */
        idx = find_lang_index(neg->accept_langs, firstlang);
    }
    variant->lang_index = idx;

    return;
}

/* Determining the content length --- if the map didn't tell us,
 * we have to do a stat() and remember for next time.
 *
 * Grump.  For Apache, even the first stat here may well be
 * redundant (for multiviews) with a stat() done by the sub_req
 * machinery.  At some point, that ought to be fixed.
 */

static float find_content_length(negotiation_state *neg, var_rec *variant)
{
    struct stat statb;

    if (variant->bytes == 0) {
        char *fullname = ap_make_full_path(neg->pool, neg->dir_name,
                                           variant->file_name);

        if (stat(fullname, &statb) >= 0) {
            /* Note, precision may be lost */
            variant->bytes = (float) statb.st_size;
        }
    }

    return variant->bytes;
}

/* For a given variant, find the best matching Accept: header
 * and assign the Accept: header's quality value to the
 * mime_type_quality field of the variant, for later use in
 * determining the best matching variant.
 */

static void set_accept_quality(negotiation_state *neg, var_rec *variant)
{
    int i;
    accept_rec *accept_recs;
    float q = 0.0f;
    int q_definite = 1;

    /* if no Accept: header, leave quality alone (will
     * remain at the default value of 1) 
     *
     * XXX: This if is currently never true because of the effect of
     * maybe_add_default_accepts().
     */
    if (!neg->accepts) {
        if (variant->mime_type && *variant->mime_type)
            variant->definite = 0;
        return;
    }

    accept_recs = (accept_rec *) neg->accepts->elts;

    /*
     * Go through each of the ranges on the Accept: header,
     * looking for the 'best' match with this variant's
     * content-type. We use the best match's quality
     * value (from the Accept: header) for this variant's
     * mime_type_quality field.
     *
     * The best match is determined like this:
     *    type/type is better than type/ * is better than * / *
     *    if match is type/type, use the level mime param if available
     */
    for (i = 0; i < neg->accepts->nelts; ++i) {

        accept_rec *type = &accept_recs[i];
        int prev_mime_stars;

        prev_mime_stars = variant->mime_stars;

        if (!mime_match(type, variant)) {
            continue;           /* didn't match the content type at all */
        }
        else {
            /* did match - see if there were less or more stars than
             * in previous match
             */
            if (prev_mime_stars == variant->mime_stars) {
                continue;       /* more stars => not as good a match */
            }
        }

        /* If we are allowed to mess with the q-values
         * and have no explicit q= parameters in the accept header,
         * make wildcards very low, so we have a low chance
         * of ending up with them if there's something better.
         */

        if (!neg->dont_fiddle_headers && !neg->accept_q &&
            variant->mime_stars == 1) {
            q = 0.01f;
        }
        else if (!neg->dont_fiddle_headers && !neg->accept_q &&
                 variant->mime_stars == 2) {
            q = 0.02f;
        }
        else {
            q = type->quality;
        }

        q_definite = (variant->mime_stars == 3);
    }
    variant->mime_type_quality = q;
    variant->definite = variant->definite && q_definite;

}

/* For a given variant, find the 'q' value of the charset given
 * on the Accept-Charset line. If no charsets are listed,
 * assume value of '1'.
 */
static void set_charset_quality(negotiation_state *neg, var_rec *variant)
{
    int i;
    accept_rec *accept_recs;
    char *charset = variant->content_charset;
    accept_rec *star = NULL;

    /* if no Accept-Charset: header, leave quality alone (will
     * remain at the default value of 1)
     */
    if (!neg->accept_charsets) {
        if (charset && *charset)
            variant->definite = 0;
        return;
    }

    accept_recs = (accept_rec *) neg->accept_charsets->elts;

    if (charset == NULL || !*charset) {
        /* Charset of variant not known */

        /* if not a text / * type, leave quality alone */
        if (!(!strncmp(variant->mime_type, "text/", 5)
              || !strcmp(variant->mime_type, INCLUDES_MAGIC_TYPE)
              || !strcmp(variant->mime_type, INCLUDES_MAGIC_TYPE3) 
              ))
            return;

        /* Don't go guessing if we are in strict header mode,
         * e.g. when running the rvsa, as any guess won't be reflected
         * in the variant list or content-location headers.
         */
        if (neg->dont_fiddle_headers)
            return;

        charset = "iso-8859-1"; /* The default charset for HTTP text types */
    }

    /*
     * Go through each of the items on the Accept-Charset header,
     * looking for a match with this variant's charset. If none
     * match, charset is unacceptable, so set quality to 0.
     */
    for (i = 0; i < neg->accept_charsets->nelts; ++i) {

        accept_rec *type = &accept_recs[i];

        if (!strcmp(type->name, charset)) {
            variant->charset_quality = type->quality;
            return;
        }
        else if (strcmp(type->name, "*") == 0) {
            star = type;
        }
    }
    /* No explicit match */
    if (star) {
        variant->charset_quality = star->quality;
        variant->definite = 0;
        return;
    }
    /* If this variant is in charset iso-8859-1, the default is 1.0 */
    if (strcmp(charset, "iso-8859-1") == 0) {
        variant->charset_quality = 1.0f;
    }
    else {
        variant->charset_quality = 0.0f;
    }
}


/* is_identity_encoding is included for back-compat, but does anyone
 * use 7bit, 8bin or binary in their var files??
 */

static int is_identity_encoding(const char *enc)
{
    return (!enc || !enc[0] || !strcmp(enc, "7bit") || !strcmp(enc, "8bit")
            || !strcmp(enc, "binary"));
}

/*
 * set_encoding_quality determines whether the encoding for a particular
 * variant is acceptable for the user-agent.
 *
 * The rules for encoding are that if the user-agent does not supply
 * any Accept-Encoding header, then all encodings are allowed but a
 * variant with no encoding should be preferred.
 * If there is an empty Accept-Encoding header, then no encodings are 
 * acceptable. If there is a non-empty Accept-Encoding header, then
 * any of the listed encodings are acceptable, as well as no encoding
 * unless the "identity" encoding is specifically excluded.
 */
static void set_encoding_quality(negotiation_state *neg, var_rec *variant)
{
    accept_rec *accept_recs;
    const char *enc = variant->content_encoding;
    accept_rec *star = NULL;
    float value_if_not_found = 0.0f;
    int i;

    if (!neg->accept_encodings) {
        /* We had no Accept-Encoding header, assume that all
         * encodings are acceptable with a low quality,
         * but we prefer no encoding if available.
         */
        if (!enc || is_identity_encoding(enc))
            variant->encoding_quality = 1.0f;
        else
            variant->encoding_quality = 0.5f;

        return;
    }

    if (!enc || is_identity_encoding(enc)) {
        enc = "identity";
        value_if_not_found = 0.0001f;
    }

    accept_recs = (accept_rec *) neg->accept_encodings->elts;

    /* Go through each of the encodings on the Accept-Encoding: header,
     * looking for a match with our encoding. x- prefixes are ignored.
     */
    if (enc[0] == 'x' && enc[1] == '-') {
        enc += 2;
    }
    for (i = 0; i < neg->accept_encodings->nelts; ++i) {

        char *name = accept_recs[i].name;

        if (name[0] == 'x' && name[1] == '-') {
            name += 2;
        }

        if (!strcmp(name, enc)) {
            variant->encoding_quality = accept_recs[i].quality;
            return;
        }

        if (strcmp(name, "*") == 0) {
            star = &accept_recs[i];
        }

    }
    /* No explicit match */
    if (star) {
        variant->encoding_quality = star->quality;
        return;
    }

    /* Encoding not found on Accept-Encoding: header, so it is
     * _not_ acceptable unless it is the identity (no encoding)
     */
    variant->encoding_quality = value_if_not_found;
}

/************************************************************* 
 * Possible results of the variant selection algorithm 
 */
enum algorithm_results {
    alg_choice = 1,              /* choose variant */
    alg_list                     /* list variants */
};

/* Below is the 'best_match' function. It returns an int, which has
 * one of the two values alg_choice or alg_list, which give the result
 * of the variant selection algorithm.  alg_list means that no best
 * variant was found by the algorithm, alg_choice means that a best
 * variant was found and should be returned.  The list/choice
 * terminology comes from TCN (rfc2295), but is used in a more generic
 * way here.  The best variant is returned in *pbest. best_match has
 * two possible algorithms for determining the best variant: the
 * RVSA/1.0 algorithm (from RFC2296), and the standard Apache
 * algorithm. These are split out into separate functions
 * (is_variant_better_rvsa() and is_variant_better()).  Selection of
 * one is through the neg->use_rvsa flag.
 *
 * The call to best_match also creates full information, including
 * language, charset, etc quality for _every_ variant. This is needed
 * for generating a correct Vary header, and can be used for the
 * Alternates header, the human-readable list responses and 406 errors.
 */

/* Firstly, the RVSA/1.0 (HTTP Remote Variant Selection Algorithm
 * v1.0) from rfc2296.  This is the algorithm that goes together with
 * transparent content negotiation (TCN).
 */
static int is_variant_better_rvsa(negotiation_state *neg, var_rec *variant,
                                  var_rec *best, float *p_bestq)
{
    float bestq = *p_bestq, q;

    /* TCN does not cover negotiation on content-encoding.  For now,
     * we ignore the encoding unless it was explicitly excluded.
     */
    if (variant->encoding_quality == 0.0f)
        return 0;
    
    q = variant->mime_type_quality *
        variant->source_quality *
        variant->charset_quality *
        variant->lang_quality;

   /* RFC 2296 calls for the result to be rounded to 5 decimal places,
    * but we don't do that because it serves no useful purpose other
    * than to ensure that a remote algorithm operates on the same
    * precision as ours.  That is silly, since what we obviously want
    * is for the algorithm to operate on the best available precision
    * regardless of who runs it.  Since the above calculation may
    * result in significant variance at 1e-12, rounding would be bogus.
    */

#ifdef NEG_DEBUG
    fprintf(stderr, "Variant: file=%s type=%s lang=%s sourceq=%1.3f "
           "mimeq=%1.3f langq=%1.3f charq=%1.3f encq=%1.3f "
           "q=%1.5f definite=%d\n",            
            (variant->file_name ? variant->file_name : ""),
            (variant->mime_type ? variant->mime_type : ""),
            (variant->content_languages
             ? ap_array_pstrcat(neg->pool, variant->content_languages, ',')
             : ""),
            variant->source_quality,
            variant->mime_type_quality,
            variant->lang_quality,
            variant->charset_quality,
            variant->encoding_quality,
            q,
            variant->definite);
#endif

    if (q <= 0.0f) {
        return 0;
    }
    if (q > bestq) {
        *p_bestq = q;
        return 1;
    }
    if (q == bestq) {
        /* If the best variant's encoding is of lesser quality than
         * this variant, then we prefer this variant
         */
        if (variant->encoding_quality > best->encoding_quality) {
            *p_bestq = q;
            return 1;
        }
    }
    return 0;
}

/* Negotiation algorithm as used by previous versions of Apache
 * (just about). 
 */

static int is_variant_better(negotiation_state *neg, var_rec *variant,
                             var_rec *best, float *p_bestq)
{
    float bestq = *p_bestq, q;
    int levcmp;

    /* For non-transparent negotiation, server can choose how
     * to handle the negotiation. We'll use the following in
     * order: content-type, language, content-type level, charset,
     * content encoding, content length.
     *
     * For each check, we have three possible outcomes:
     *   This variant is worse than current best: return 0
     *   This variant is better than the current best:
     *          assign this variant's q to *p_bestq, and return 1
     *   This variant is just as desirable as the current best:
     *          drop through to the next test.
     *
     * This code is written in this long-winded way to allow future
     * customisation, either by the addition of additional
     * checks, or to allow the order of the checks to be determined
     * by configuration options (e.g. we might prefer to check
     * language quality _before_ content type).
     */

    /* First though, eliminate this variant if it is not
     * acceptable by type, charset, encoding or language.
     */

#ifdef NEG_DEBUG
    fprintf(stderr, "Variant: file=%s type=%s lang=%s sourceq=%1.3f "
           "mimeq=%1.3f langq=%1.3f langidx=%d charq=%1.3f encq=%1.3f \n",
            (variant->file_name ? variant->file_name : ""),
            (variant->mime_type ? variant->mime_type : ""),
            (variant->content_languages
             ? ap_array_pstrcat(neg->pool, variant->content_languages, ',')
             : ""),
            variant->source_quality,
            variant->mime_type_quality,
            variant->lang_quality,
            variant->lang_index,
            variant->charset_quality,
            variant->encoding_quality);
#endif

    if (variant->encoding_quality == 0.0f ||
        variant->lang_quality == 0.0f ||
        variant->source_quality == 0.0f ||
        variant->charset_quality == 0.0f ||
        variant->mime_type_quality == 0.0f) {
        return 0;               /* don't consider unacceptables */
    }

    q = variant->mime_type_quality * variant->source_quality;
    if (q == 0.0 || q < bestq) {
        return 0;
    }
    if (q > bestq || !best) {
        *p_bestq = q;
        return 1;
    }

    /* language */
    if (variant->lang_quality < best->lang_quality) {
        return 0;
    }
    if (variant->lang_quality > best->lang_quality) {
        *p_bestq = q;
        return 1;
    }

    /* if language qualities were equal, try the LanguagePriority stuff */
    if (best->lang_index != -1 &&
        (variant->lang_index == -1 || variant->lang_index > best->lang_index)) {
        return 0;
    }
    if (variant->lang_index != -1 &&
        (best->lang_index == -1 || variant->lang_index < best->lang_index)) {
        *p_bestq = q;
        return 1;
    }

    /* content-type level (sometimes used with text/html, though we
     * support it on other types too)
     */
    levcmp = level_cmp(variant, best);
    if (levcmp == -1) {
        return 0;
    }
    if (levcmp == 1) {
        *p_bestq = q;
        return 1;
    }

    /* charset */
    if (variant->charset_quality < best->charset_quality) {
        return 0;
    }
    /* If the best variant's charset is ISO-8859-1 and this variant has
     * the same charset quality, then we prefer this variant
     */

    if (variant->charset_quality > best->charset_quality ||
        ((variant->content_charset != NULL &&
          *variant->content_charset != '\0' &&
          strcmp(variant->content_charset, "iso-8859-1") != 0) &&
         (best->content_charset == NULL ||
          *best->content_charset == '\0' ||
          strcmp(best->content_charset, "iso-8859-1") == 0))) {
        *p_bestq = q;
        return 1;
    }

    /* Prefer the highest value for encoding_quality.
     */
    if (variant->encoding_quality < best->encoding_quality) {
       return 0;
    }
    if (variant->encoding_quality > best->encoding_quality) {
       *p_bestq = q;
       return 1;
    }

    /* content length if all else equal */
    if (find_content_length(neg, variant) >= find_content_length(neg, best)) {
        return 0;
    }

    /* ok, to get here means every thing turned out equal, except
     * we have a shorter content length, so use this variant
     */
    *p_bestq = q;
    return 1;
}

static int best_match(negotiation_state *neg, var_rec **pbest)
{
    int j;
    var_rec *best = NULL;
    float bestq = 0.0f;
    enum algorithm_results algorithm_result;

    var_rec *avail_recs = (var_rec *) neg->avail_vars->elts;

    set_default_lang_quality(neg);

    /*
     * Find the 'best' variant 
     */

    for (j = 0; j < neg->avail_vars->nelts; ++j) {
        var_rec *variant = &avail_recs[j];

        /* Find all the relevant 'quality' values from the
         * Accept... headers, and store in the variant.  This also
         * prepares for sending an Alternates header etc so we need to
         * do it even if we do not actually plan to find a best
         * variant.  
         */
        set_accept_quality(neg, variant);
        set_language_quality(neg, variant);
        set_encoding_quality(neg, variant);
        set_charset_quality(neg, variant);

        /* Only do variant selection if we may actually choose a
         * variant for the client 
         */
        if (neg->may_choose) {

            /* Now find out if this variant is better than the current
             * best, either using the RVSA/1.0 algorithm, or Apache's
             * internal server-driven algorithm. Presumably other
             * server-driven algorithms are possible, and could be
             * implemented here.
             */
     
            if (neg->use_rvsa) {
                if (is_variant_better_rvsa(neg, variant, best, &bestq)) {
                    best = variant;
                }
            }
            else {
                if (is_variant_better(neg, variant, best, &bestq)) {
                    best = variant;
                }
            }
        }
    }

    /* We now either have a best variant, or no best variant */

    if (neg->use_rvsa)    {
        /* calculate result for RVSA/1.0 algorithm:
         * only a choice response if the best variant has q>0
         * and is definite
         */
        algorithm_result = (best && best->definite) && (bestq > 0) ?
                           alg_choice : alg_list;
    }
    else {
        /* calculate result for Apache negotiation algorithm */
        algorithm_result = bestq > 0 ? alg_choice : alg_list;        
    }

    /* Returning a choice response with a non-neighboring variant is a
     * protocol security error in TCN (see rfc2295).  We do *not*
     * verify here that the variant and URI are neighbors, even though
     * we may return alg_choice.  We depend on the environment (the
     * caller) to only declare the resource transparently negotiable if
     * all variants are neighbors.
     */
    *pbest = best;
    return algorithm_result;
}

/* Sets response headers for a negotiated response.
 * neg->is_transparent determines whether a transparently negotiated
 * response or a plain `server driven negotiation' response is
 * created.   Applicable headers are Alternates, Vary, and TCN.
 *
 * The Vary header we create is sometimes longer than is required for
 * the correct caching of negotiated results by HTTP/1.1 caches.  For
 * example if we have 3 variants x.html, x.ps.en and x.ps.nl, and if
 * the Accept: header assigns a 0 quality to .ps, then the results of
 * the two server-side negotiation algorithms we currently implement
 * will never depend on Accept-Language so we could return `Vary:
 * negotiate, accept' instead of the longer 'Vary: negotiate, accept,
 * accept-language' which the code below will return.  A routine for
 * computing the exact minimal Vary header would be a huge pain to code
 * and maintain though, especially because we need to take all possible
 * twiddles in the server-side negotiation algorithms into account.
 */
static void set_neg_headers(request_rec *r, negotiation_state *neg,
                            int alg_result)
{
    table *hdrs;
    var_rec *avail_recs = (var_rec *) neg->avail_vars->elts;
    const char *sample_type = NULL;
    const char *sample_language = NULL;
    const char *sample_encoding = NULL;
    const char *sample_charset = NULL;
    char *lang;
    char *qstr;
    char *lenstr;
    long len;
    array_header *arr;
    int max_vlist_array = (neg->avail_vars->nelts * 21);
    int first_variant = 1;
    int vary_by_type = 0;
    int vary_by_language = 0;
    int vary_by_charset = 0;
    int vary_by_encoding = 0;
    int j;

    /* In order to avoid O(n^2) memory copies in building Alternates,
     * we preallocate a table with the maximum substrings possible,
     * fill it with the variant list, and then concatenate the entire array.
     * Note that if you change the number of substrings pushed, you also
     * need to change the calculation of max_vlist_array above.
     */
    if (neg->send_alternates && neg->avail_vars->nelts)
        arr = ap_make_array(r->pool, max_vlist_array, sizeof(char *));
    else
        arr = NULL;

    /* Put headers into err_headers_out, since send_http_header()
     * outputs both headers_out and err_headers_out.
     */
    hdrs = r->err_headers_out;

    for (j = 0; j < neg->avail_vars->nelts; ++j) {
        var_rec *variant = &avail_recs[j];

        if (variant->content_languages && variant->content_languages->nelts) {
            lang = ap_array_pstrcat(r->pool, variant->content_languages, ',');
        }
        else {
            lang = NULL;
        }

        /* Calculate Vary by looking for any difference between variants */

        if (first_variant) {
            sample_type     = variant->mime_type;
            sample_charset  = variant->content_charset;
            sample_language = lang;
            sample_encoding = variant->content_encoding;
        }
        else {
            if (!vary_by_type &&
                strcmp(sample_type ? sample_type : "", 
                       variant->mime_type ? variant->mime_type : "")) {
                vary_by_type = 1;
            }
            if (!vary_by_charset &&
                strcmp(sample_charset ? sample_charset : "",
                       variant->content_charset ?
                       variant->content_charset : "")) {
                vary_by_charset = 1;
            }
            if (!vary_by_language &&
                strcmp(sample_language ? sample_language : "", 
                       lang ? lang : "")) {
                vary_by_language = 1;
            }
            if (!vary_by_encoding &&
                strcmp(sample_encoding ? sample_encoding : "",
                       variant->content_encoding ? 
                       variant->content_encoding : "")) {
                vary_by_encoding = 1;
            }
        }
        first_variant = 0;

        if (!neg->send_alternates)
            continue;

        /* Generate the string components for this Alternates entry */

        *((const char **) ap_push_array(arr)) = "{\"";
        *((const char **) ap_push_array(arr)) = variant->file_name;
        *((const char **) ap_push_array(arr)) = "\" ";

        qstr = (char *) ap_palloc(r->pool, 6);
        ap_snprintf(qstr, 6, "%1.3f", variant->source_quality);

        /* Strip trailing zeros (saves those valuable network bytes) */
        if (qstr[4] == '0') {
            qstr[4] = '\0';
            if (qstr[3] == '0') {
                qstr[3] = '\0';
                if (qstr[2] == '0') {
                    qstr[1] = '\0';
                }
            }
        }
        *((const char **) ap_push_array(arr)) = qstr;

        if (variant->mime_type && *variant->mime_type) {
            *((const char **) ap_push_array(arr)) = " {type ";
            *((const char **) ap_push_array(arr)) = variant->mime_type;
            *((const char **) ap_push_array(arr)) = "}";
        }
        if (variant->content_charset && *variant->content_charset) {
            *((const char **) ap_push_array(arr)) = " {charset ";
            *((const char **) ap_push_array(arr)) = variant->content_charset;
            *((const char **) ap_push_array(arr)) = "}";
        }
        if (lang) {
            *((const char **) ap_push_array(arr)) = " {language ";
            *((const char **) ap_push_array(arr)) = lang;
            *((const char **) ap_push_array(arr)) = "}";
        }
        if (variant->content_encoding && *variant->content_encoding) {
            /* Strictly speaking, this is non-standard, but so is TCN */

            *((const char **) ap_push_array(arr)) = " {encoding ";
            *((const char **) ap_push_array(arr)) = variant->content_encoding;
            *((const char **) ap_push_array(arr)) = "}";
        }

        /* Note that the Alternates specification (in rfc2295) does
         * not require that we include {length x}, so we could omit it
         * if determining the length is too expensive.  We currently
         * always include it though.  22 bytes is enough for 2^64.
         *
         * If the variant is a CGI script, find_content_length would
         * return the length of the script, not the output it
         * produces, so we check for the presence of a handler and if
         * there is one we don't add a length.
         * 
         * XXX: TODO: This check does not detect a CGI script if we
         * get the variant from a type map.  This needs to be fixed
         * (without breaking things if the type map specifies a
         * content-length, which currently leads to the correct result).
         */
        if (!(variant->sub_req && variant->sub_req->handler)
            && (len = (long)find_content_length(neg, variant)) != 0) {

            lenstr = (char *) ap_palloc(r->pool, 22);
            ap_snprintf(lenstr, 22, "%ld", len);
            *((const char **) ap_push_array(arr)) = " {length ";
            *((const char **) ap_push_array(arr)) = lenstr;
            *((const char **) ap_push_array(arr)) = "}";
        }
      
        *((const char **) ap_push_array(arr)) = "}";
        *((const char **) ap_push_array(arr)) = ", "; /* trimmed below */
    }

    if (neg->send_alternates && neg->avail_vars->nelts) {
        arr->nelts--;                                 /* remove last comma */
        ap_table_mergen(hdrs, "Alternates",
                        ap_array_pstrcat(r->pool, arr, '\0'));
    } 

    if (neg->is_transparent || vary_by_type || vary_by_language ||
        vary_by_language || vary_by_charset || vary_by_encoding) {

        ap_table_mergen(hdrs, "Vary", 2 + ap_pstrcat(r->pool,
            neg->is_transparent ? ", negotiate"       : "",
            vary_by_type        ? ", accept"          : "",
            vary_by_language    ? ", accept-language" : "",
            vary_by_charset     ? ", accept-charset"  : "",
            vary_by_encoding    ? ", accept-encoding" : "", NULL));
    }

    if (neg->is_transparent) { /* Create TCN response header */
        ap_table_setn(hdrs, "TCN",
                      alg_result == alg_list ? "list" : "choice");
    }
}

/**********************************************************************
 *
 * Return an HTML list of variants. This is output as part of the
 * choice response or 406 status body.
 */

static char *make_variant_list(request_rec *r, negotiation_state *neg)
{
    array_header *arr;
    int i;
    int max_vlist_array = (neg->avail_vars->nelts * 15) + 2;

    /* In order to avoid O(n^2) memory copies in building the list,
     * we preallocate a table with the maximum substrings possible,
     * fill it with the variant list, and then concatenate the entire array.
     */
    arr = ap_make_array(r->pool, max_vlist_array, sizeof(char *));

    *((const char **) ap_push_array(arr)) = "Available variants:\n<ul>\n";

    for (i = 0; i < neg->avail_vars->nelts; ++i) {
        var_rec *variant = &((var_rec *) neg->avail_vars->elts)[i];
        char *filename = variant->file_name ? variant->file_name : "";
        array_header *languages = variant->content_languages;
        char *description = variant->description ? variant->description : "";

        /* The format isn't very neat, and it would be nice to make
         * the tags human readable (eg replace 'language en' with 'English').
         * Note that if you change the number of substrings pushed, you also
         * need to change the calculation of max_vlist_array above.
         */
        *((const char **) ap_push_array(arr)) = "<li><a href=\"";
        *((const char **) ap_push_array(arr)) = filename;
        *((const char **) ap_push_array(arr)) = "\">";
        *((const char **) ap_push_array(arr)) = filename;
        *((const char **) ap_push_array(arr)) = "</a> ";
        *((const char **) ap_push_array(arr)) = description;

        if (variant->mime_type && *variant->mime_type) {
            *((const char **) ap_push_array(arr)) = ", type ";
            *((const char **) ap_push_array(arr)) = variant->mime_type;
        }
        if (languages && languages->nelts) {
            *((const char **) ap_push_array(arr)) = ", language ";
            *((const char **) ap_push_array(arr)) = ap_array_pstrcat(r->pool,
                                                       languages, ',');
        }
        if (variant->content_charset && *variant->content_charset) {
            *((const char **) ap_push_array(arr)) = ", charset ";
            *((const char **) ap_push_array(arr)) = variant->content_charset;
        }
        if (variant->content_encoding) {
            *((const char **) ap_push_array(arr)) = ", encoding ";
            *((const char **) ap_push_array(arr)) = variant->content_encoding;
        }
        *((const char **) ap_push_array(arr)) = "\n";
    }
    *((const char **) ap_push_array(arr)) = "</ul>\n";

    return ap_array_pstrcat(r->pool, arr, '\0');
}

static void store_variant_list(request_rec *r, negotiation_state *neg)
{
    if (r->main == NULL) {
        ap_table_setn(r->notes, "variant-list", make_variant_list(r, neg));
    }
    else {
        ap_table_setn(r->main->notes, "variant-list",
                      make_variant_list(r->main, neg));
    }
}

/* Called if we got a "Choice" response from the variant selection algorithm.
 * It checks the result of the chosen variant to see if it
 * is itself negotiated (if so, return error VARIANT_ALSO_VARIES).
 * Otherwise, add the appropriate headers to the current response.
 */

static int setup_choice_response(request_rec *r, negotiation_state *neg,
                                 var_rec *variant)
{
    request_rec *sub_req;
    const char *sub_vary;

    if (!variant->sub_req) {
        int status;

        sub_req = ap_sub_req_lookup_file(variant->file_name, r);
        status = sub_req->status;

        if (status != HTTP_OK && 
            !ap_table_get(sub_req->err_headers_out, "TCN")) {
            ap_destroy_sub_req(sub_req);
            return status;
        }
        variant->sub_req = sub_req;
    }
    else {
        sub_req = variant->sub_req;
    }

    /* The variant selection algorithm told us to return a "Choice"
     * response. This is the normal variant response, with
     * some extra headers. First, ensure that the chosen
     * variant did or will not itself engage in transparent negotiation.
     * If not, set the appropriate headers, and fall through to
     * the normal variant handling 
     */

    /* This catches the error that a transparent type map selects a
     * transparent multiviews resource as the best variant.
     *
     * XXX: We do not signal an error if a transparent type map
     * selects a _non_transparent multiviews resource as the best
     * variant, because we can generate a legal negotiation response
     * in this case.  In this case, the vlist_validator of the
     * nontransparent subrequest will be lost however.  This could
     * lead to cases in which a change in the set of variants or the
     * negotiation algorithm of the nontransparent resource is never
     * propagated up to a HTTP/1.1 cache which interprets Vary.  To be
     * completely on the safe side we should return VARIANT_ALSO_VARIES
     * for this type of recursive negotiation too.
     */
    if (neg->is_transparent &&
        ap_table_get(sub_req->err_headers_out, "TCN")) {
        return VARIANT_ALSO_VARIES;
    }

    /* This catches the error that a transparent type map recursively
     * selects, as the best variant, another type map which itself
     * causes transparent negotiation to be done.
     *
     * XXX: Actually, we catch this error by catching all cases of
     * type map recursion.  There are some borderline recursive type
     * map arrangements which would not produce transparent
     * negotiation protocol errors or lack of cache propagation
     * problems, but such arrangements are very hard to detect at this
     * point in the control flow, so we do not bother to single them
     * out.
     *
     * Recursive type maps imply a recursive arrangement of negotiated
     * resources which is visible to outside clients, and this is not
     * supported by the transparent negotiation caching protocols, so
     * if we are to have generic support for recursive type maps, we
     * have to create some configuration setting which makes all type
     * maps non-transparent when recursion is enabled.  Also, if we
     * want recursive type map support which ensures propagation of
     * type map changes into HTTP/1.1 caches that handle Vary, we
     * would have to extend the current mechanism for generating
     * variant list validators.
     */
    if (sub_req->handler && strcmp(sub_req->handler, "type-map") == 0) {
        return VARIANT_ALSO_VARIES;
    }

    /* This adds an appropriate Variant-Vary header if the subrequest
     * is a multiviews resource.
     *
     * XXX: TODO: Note that this does _not_ handle any Vary header
     * returned by a CGI if sub_req is a CGI script, because we don't
     * see that Vary header yet at this point in the control flow.
     * This won't cause any cache consistency problems _unless_ the
     * CGI script also returns a Cache-Control header marking the
     * response as cachable.  This needs to be fixed, also there are
     * problems if a CGI returns an Etag header which also need to be
     * fixed.
     */
    if ((sub_vary = ap_table_get(sub_req->err_headers_out, "Vary")) != NULL) {
        ap_table_setn(r->err_headers_out, "Variant-Vary", sub_vary);

        /* Move the subreq Vary header into the main request to
         * prevent having two Vary headers in the response, which
         * would be legal but strange.
         */
        ap_table_setn(r->err_headers_out, "Vary", sub_vary);
        ap_table_unset(sub_req->err_headers_out, "Vary");
    }
    
    ap_table_setn(r->err_headers_out, "Content-Location",
                  ap_pstrdup(r->pool, variant->file_name));

    set_neg_headers(r, neg, alg_choice);         /* add Alternates and Vary */

    /* Still to do by caller: add Expires */

    return 0;
}

/****************************************************************
 *
 * Executive...
 */

static int do_negotiation(request_rec *r, negotiation_state *neg, 
                          var_rec **bestp, int prefer_scripts) 
{
    var_rec *avail_recs = (var_rec *) neg->avail_vars->elts;
    int alg_result;              /* result of variant selection algorithm */
    int res;
    int j;

    /* Decide if resource is transparently negotiable */

    /* GET or HEAD? (HEAD has same method number as GET) */
    if (r->method_number == M_GET) {

        /* maybe this should be configurable, see also the comment
         * about recursive type maps in setup_choice_response()
         */
        neg->is_transparent = 1;       

        /* We can't be transparent if we are a map file in the middle
         * of the request URI.
         */
        if (r->path_info && *r->path_info)
            neg->is_transparent = 0;

        for (j = 0; j < neg->avail_vars->nelts; ++j) {
            var_rec *variant = &avail_recs[j];

            /* We can't be transparent, because of internal
             * assumptions in best_match(), if there is a
             * non-neighboring variant.  We can have a non-neighboring
             * variant when processing a type map.  
             */
            if (strchr(variant->file_name, '/'))
                neg->is_transparent = 0;
        }
    }

    if (neg->is_transparent)  {
        parse_negotiate_header(r, neg);
    }
    else { /* configure negotiation on non-transparent resource */
        neg->may_choose = 1;
    }

    maybe_add_default_accepts(neg, prefer_scripts);

    alg_result = best_match(neg, bestp);

    /* alg_result is one of
     *   alg_choice: a best variant is chosen
     *   alg_list: no best variant is chosen
     */

    if (alg_result == alg_list) {
        /* send a list response or NOT_ACCEPTABLE error response  */

        neg->send_alternates = 1; /* always include Alternates header */
        set_neg_headers(r, neg, alg_result); 
        store_variant_list(r, neg);

        if (neg->is_transparent && neg->ua_supports_trans) {
            /* XXX todo: expires? cachability? */
            
            /* Some HTTP/1.0 clients are known to choke when they get
             * a 300 (multiple choices) response without a Location
             * header.  However the 300 code response we are are about
             * to generate will only reach 1.0 clients which support
             * transparent negotiation, and they should be OK. The
             * response should never reach older 1.0 clients, even if
             * we have CacheNegotiatedDocs enabled, because no 1.0
             * proxy cache (we know of) will cache and return 300
             * responses (they certainly won't if they conform to the
             * HTTP/1.0 specification).
             */
            return MULTIPLE_CHOICES;
        }
        
        if (!*bestp) {
            ap_log_rerror(APLOG_MARK, APLOG_NOERRNO|APLOG_ERR, r,
                          "no acceptable variant: %s", r->filename);
            return NOT_ACCEPTABLE;
        }
    }

    /* Variant selection chose a variant */

    /* XXX todo: merge the two cases in the if statement below */
    if (neg->is_transparent) {

        if ((res = setup_choice_response(r, neg, *bestp)) != 0) {
            return res; /* return if error */
        }
    }
    else {
        set_neg_headers(r, neg, alg_result);
    }

    /* Make sure caching works - Vary should handle HTTP/1.1, but for
     * HTTP/1.0, we can't allow caching at all.
     */

    /* XXX: Note that we only set r->no_cache to 1, which causes
     * Expires: <now> to be added, when responding to a HTTP/1.0
     * client.  If we return the response to a 1.1 client, we do not
     * add Expires <now>, because doing so would degrade 1.1 cache
     * performance by preventing re-use of the response without prior
     * revalidation.  On the other hand, if the 1.1 client is a proxy
     * which was itself contacted by a 1.0 client, or a proxy cache
     * which can be contacted later by 1.0 clients, then we currently
     * rely on this 1.1 proxy to add the Expires: <now> when it
     * forwards the response.
     *
     * XXX: TODO: Find out if the 1.1 spec requires proxies and
     * tunnels to add Expires: <now> when forwarding the response to
     * 1.0 clients.  I (kh) recall it is rather vague on this point.
     * Testing actual 1.1 proxy implementations would also be nice. If
     * Expires: <now> is not added by proxies then we need to always
     * include Expires: <now> ourselves to ensure correct caching, but
     * this would degrade HTTP/1.1 cache efficiency unless we also add
     * Cache-Control: max-age=N, which we currently don't.
     *
     * Roy: No, we are not going to screw over HTTP future just to
     *      ensure that people who can't be bothered to upgrade their
     *      clients will always receive perfect server-side negotiation.
     *      Hell, those clients are sending bogus accept headers anyway.
     *
     *      Manual setting of cache-control/expires always overrides this
     *      automated kluge, on purpose.
     */
    
    if ((!do_cache_negotiated_docs(r->server)
         && (r->proto_num < HTTP_VERSION(1,1)))        
         && neg->count_multiviews_variants != 1) {
        r->no_cache = 1;
    }

    return OK;
}

static int handle_map_file(request_rec *r)
{
    negotiation_state *neg = parse_accept_headers(r);
    var_rec *best;
    int res;

    char *udir;

    if ((res = read_type_map(neg, r))) {
        return res;
    }

    res = do_negotiation(r, neg, &best, 0);
    if (res != 0) return res;

    if (r->path_info && *r->path_info) {
        r->uri[ap_find_path_info(r->uri, r->path_info)] = '\0';
    }
    udir = ap_make_dirstr_parent(r->pool, r->uri);
    udir = ap_escape_uri(r->pool, udir);
    ap_internal_redirect(ap_pstrcat(r->pool, udir, best->file_name,
                                    r->path_info, NULL), r);
    return OK;
}

static int handle_multi(request_rec *r)
{
    negotiation_state *neg;
    var_rec *best, *avail_recs;
    request_rec *sub_req;
    int res;
    int j;

    if (r->finfo.st_mode != 0 || !(ap_allow_options(r) & OPT_MULTI)) {
        return DECLINED;
    }

    neg = parse_accept_headers(r);

    if ((res = read_types_multi(neg))) {
      return_from_multi:
        /* free all allocated memory from subrequests */
        avail_recs = (var_rec *) neg->avail_vars->elts;
        for (j = 0; j < neg->avail_vars->nelts; ++j) {
            var_rec *variant = &avail_recs[j];
            if (variant->sub_req) {
                ap_destroy_sub_req(variant->sub_req);
            }
        }
        return res;
    }
    if (neg->avail_vars->nelts == 0) {
        return DECLINED;
    }

    res = do_negotiation(r, neg, &best,
                         (r->method_number != M_GET) || r->args ||
                         (r->path_info && *r->path_info));
    if (res != 0)
        goto return_from_multi;

    if (!(sub_req = best->sub_req)) {
        /* We got this out of a map file, so we don't actually have
         * a sub_req structure yet.  Get one now.
         */

        sub_req = ap_sub_req_lookup_file(best->file_name, r);
        if (sub_req->status != HTTP_OK) {
            res = sub_req->status;
            ap_destroy_sub_req(sub_req);
            goto return_from_multi;
        }
    }

    /* BLECH --- don't multi-resolve non-ordinary files */

    if (!S_ISREG(sub_req->finfo.st_mode)) {
        res = NOT_FOUND;
        goto return_from_multi;
    }

    /* Otherwise, use it. */

    /* now do a "fast redirect" ... promote the sub_req into the main req */
    /* We need to tell POOL_DEBUG that we're guaranteeing that sub_req->pool
     * will exist as long as r->pool.  Otherwise we run into troubles because
     * some values in this request will be allocated in r->pool, and others in
     * sub_req->pool.
     */
    ap_pool_join(r->pool, sub_req->pool);
    r->mtime = 0; /* reset etag info for subrequest */
    /* XXX: uri/args/path_info are all retained from the original request.
     *      It is entirely possible, but not common, for a handler to choke
     *      on some expectation based on the uri (or more commonly, args) that 
     *      the file subrequest was prepared to handle, but a lookup_uri would
     *      have considered an error.  This leaves an improbable possibility 
     *      that the user might fail a mod_dir request later, and the server 
     *      may respond with a mod_autoindex response.  However, this has been
     *      the behavior throughout much of the Apache 1.3 era with minimal
     *      side effects, mostly caused by obscure configuration bugs.
     *  r->uri = sub_req->uri;
     *  r->args = sub_req->args;
     *  r->path_info = sub_req->path_info;  
     */
    r->filename = sub_req->filename;
    r->handler = sub_req->handler;
    r->content_type = sub_req->content_type;
    r->content_encoding = sub_req->content_encoding;
    r->content_languages = sub_req->content_languages;
    r->content_language = sub_req->content_language;
    r->finfo = sub_req->finfo;
    r->per_dir_config = sub_req->per_dir_config;
    /* copy output headers from subrequest, but leave negotiation headers */
    r->notes = ap_overlay_tables(r->pool, sub_req->notes, r->notes);
    r->headers_out = ap_overlay_tables(r->pool, sub_req->headers_out,
                                    r->headers_out);
    r->err_headers_out = ap_overlay_tables(r->pool, sub_req->err_headers_out,
                                        r->err_headers_out);
    r->subprocess_env = ap_overlay_tables(r->pool, sub_req->subprocess_env,
                                       r->subprocess_env);
    avail_recs = (var_rec *) neg->avail_vars->elts;
    for (j = 0; j < neg->avail_vars->nelts; ++j) {
        var_rec *variant = &avail_recs[j];
        if (variant != best && variant->sub_req) {
            ap_destroy_sub_req(variant->sub_req);
        }
    }
    return OK;
}

/********************************************************************** 
 * There is a problem with content-encoding, as some clients send and
 * expect an x- token (e.g. x-gzip) while others expect the plain token
 * (i.e. gzip). To try and deal with this as best as possible we do
 * the following: if the client sent an Accept-Encoding header and it
 * contains a plain token corresponding to the content encoding of the
 * response, then set content encoding using the plain token. Else if
 * the A-E header contains the x- token use the x- token in the C-E
 * header. Else don't do anything.
 *
 * Note that if no A-E header was sent, or it does not contain a token
 * compatible with the final content encoding, then the token in the
 * C-E header will be whatever was specified in the AddEncoding
 * directive.
 */
static int fix_encoding(request_rec *r)
{
    const char *enc = r->content_encoding;
    char *x_enc = NULL;
    array_header *accept_encodings;
    accept_rec *accept_recs;
    int i;

    if (!enc || !*enc) {
        return DECLINED;
    }

    if (enc[0] == 'x' && enc[1] == '-') {
        enc += 2;
    }

    if ((accept_encodings = do_header_line(r->pool,
             ap_table_get(r->headers_in, "Accept-Encoding"))) == NULL) {
        return DECLINED;
    }

    accept_recs = (accept_rec *) accept_encodings->elts;

    for (i = 0; i < accept_encodings->nelts; ++i) {
        char *name = accept_recs[i].name;

        if (!strcmp(name, enc)) {
            r->content_encoding = name;
            return OK;
        }

        if (name[0] == 'x' && name[1] == '-' && !strcmp(name+2, enc)) {
            x_enc = name;
        }
    }

    if (x_enc) {
        r->content_encoding = x_enc;
        return OK;
    }

    return DECLINED;
}

static const handler_rec negotiation_handlers[] =
{
    {MAP_FILE_MAGIC_TYPE, handle_map_file},
    {"type-map", handle_map_file},
    {NULL}
};

module MODULE_VAR_EXPORT negotiation_module =
{
    STANDARD_MODULE_STUFF,
    NULL,                       /* initializer */
    create_neg_dir_config,      /* dir config creator */
    merge_neg_dir_configs,      /* dir merger --- default is to override */
    NULL,                       /* server config */
    NULL,                       /* merge server config */
    negotiation_cmds,           /* command table */
    negotiation_handlers,       /* handlers */
    NULL,                       /* filename translation */
    NULL,                       /* check_user_id */
    NULL,                       /* check auth */
    NULL,                       /* check access */
    handle_multi,               /* type_checker */
    fix_encoding,               /* fixups */
    NULL,                       /* logger */
    NULL,                       /* header parser */
    NULL,                       /* child_init */
    NULL,                       /* child_exit */
    NULL                        /* post read-request */
};
