/*		   /Net/dxcern/userd/timbl/hypertext/WWW/Library/Implementation/HTAnchor.html
 */

/*	Hypertext "Anchor" Object				     HTAnchor.h
**	==========================
**
**	An anchor represents a region of a hypertext document which is linked
**	to another anchor in the same or a different document.
*/

#ifndef HTANCHOR_H
#define HTANCHOR_H

/* Version 0 (TBL) written in Objective-C for the NeXT browser */
/* Version 1 of 24-Oct-1991 (JFG), written in C, browser-independent */

#include <HTList.h>
#include <HTBTree.h>
#include <HTChunk.h>
#include <HTAtom.h>
#include <UCDefs.h>

/*			Main definition of anchor
**			=========================
*/

typedef struct _HyperDoc HyperDoc;  /* Ready for forward references */
typedef struct _HTAnchor HTAnchor;
typedef struct _HTParentAnchor HTParentAnchor;
typedef struct _HTParentAnchor0 HTParentAnchor0;

/*	After definition of HTFormat: */
#include <HTFormat.h>


struct _HTAnchor {		/* Generic anchor */
  HTParentAnchor0 * parent;	/* Parent of this anchor (self for adults) */
};

struct _HTParentAnchor0 {	/* One for adult_table,
				 * generally not used outside HTAnchor.c */
  /* Common part from the generic anchor structure */
  HTParentAnchor0 * parent;	/* (self) */

  /* ParentAnchor0-specific information */
  char		  * address;	/* Absolute address of this node */
  HTParentAnchor  * info;	/* additional info, allocated on demand */

  HTBTree *	children;	/* Subanchors <a name="tag">, sorted by tag */
  HTList	sources;	/* List of anchors pointing to this, if any */

  HTList	_add_adult;	/* - just a memory for list entry:) */
  short		adult_hash;	/* adult list number */
  BOOL		underway;	/* Document about to be attached to it */
};

/*
 *  Separated from the above to save memory:  allocated on demand,
 *  it is nearly 1:1 to HText (well, sometimes without HText...),
 *  available for SGML, HTML, and HText stages.
 *  [being precise, we currently allocate it before HTLoadDocument(),
 *  in HTAnchor_findAddress() and HTAnchor_parent()].
 */
struct _HTParentAnchor {
  /* Common part from the generic anchor structure */
  HTParentAnchor0 * parent;	/* Parent of this anchor */

  /* ParentAnchor-specific information */
  HTList	children_notag;	/* Subanchors <a href=...>, tag is NULL */
  HyperDoc *	document;	/* The document within which this is an anchor*/

  char *	address;	/* parent->address, a pointer */
  bstring *	post_data;	/* Posting data */
  char *	post_content_type;  /* Type of post data */
  char *	bookmark;	/* Bookmark filename */
  HTFormat	format;		/* Pointer to node format descriptor */
  char *	charset;	/* Pointer to character set (kludge, for now */
  BOOL		isIndex;	/* Acceptance of a keyword search */
  char *	isIndexAction;	/* URL of isIndex server */
  char *	isIndexPrompt;	/* Prompt for isIndex query */
  char *	title;		/* Title of document */
  char *	owner;		/* Owner of document */
  char *	RevTitle;	/* TITLE in REV="made" or REV="owner" LINK */
  char *	citehost;	/* Citehost from REL="citehost" LINK */
#ifdef USE_COLOR_STYLE
  char *	style;
#endif

  HTList*	methods;	/* Methods available as HTAtoms */
  void *	protocol;	/* Protocol object */
  char *	physical;	/* Physical address */
  BOOL		isISMAPScript;	/* Script for clickable image map */
  BOOL		isHEAD;		/* Document is headers from a HEAD request */
  BOOL		safe;			/* Safe */
#ifdef USE_SOURCE_CACHE
  char *	source_cache_file;
  HTChunk *	source_cache_chunk;
#endif
  char *	FileCache;	/* Path to a disk-cached copy (see src/HTFWriter.c) */
  char *	SugFname;	/* Suggested filename */
  char *	cache_control;	/* Cache-Control */
  BOOL		no_cache;	/* Cache-Control, Pragma or META "no-cache"? */
  BOOL		inBASE;		/* duplicated from HTStructured (HTML.c/h) */
  char *	content_type;		/* Content-Type */
  char *	content_language;	/* Content-Language */
  char *	content_encoding;	/* Compression algorithm */
  char *	content_base;		/* Content-Base */
  char *	content_disposition;	/* Content-Disposition */
  char *	content_location;	/* Content-Location */
  char *	content_md5;		/* Content-MD5 */
  char *	message_id;		/* Message-ID */
  char *	subject;		/* Subject */
  int		content_length;		/* Content-Length */
  char *	date;			/* Date */
  char *	expires;		/* Expires */
  char *	last_modified;		/* Last-Modified */
  char *	ETag;			/* ETag (HTTP1.1 cache validator) */
  char *	server;			/* Server */
  UCAnchorInfo *UCStages;		/* chartrans stages */
  HTList *	imaps;			/* client side image maps */
};

typedef HTAtom HTLinkType;

typedef struct {
  /* Common part from the generic anchor structure */
  HTParentAnchor0 * parent;	/* Parent of this anchor */

  /* ChildAnchor-specific information */
  char *	tag;		/* #fragment,  relative to the parent */

  HTAnchor *	dest;		/* The anchor to which this leads */
  HTLinkType *	type;		/* Semantics of this link */

  HTList	_add_children_notag;	/* - just a memory for list entry:) */
  HTList	_add_sources;		/* - just a memory for list entry:) */
} HTChildAnchor;

/*
**  DocAddress structure is used for loading an absolute anchor with all
**  needed information including posting data and post content type.
*/
typedef struct _DocAddress {
    char *	address;
    bstring *	post_data;
    char *	post_content_type;
    char *	bookmark;
    BOOL	isHEAD;
    BOOL	safe;
} DocAddress;

/* "internal" means "within the same document, with certainty". */
extern HTLinkType * HTInternalLink;

/*	Create or find a child anchor with a possible link
**	--------------------------------------------------
**
**	Create new anchor with a given parent and possibly
**	a name, and possibly a link to a _relatively_ named anchor.
**	(Code originally in ParseHTML.h)
*/
extern HTChildAnchor * HTAnchor_findChildAndLink PARAMS((
	HTParentAnchor * parent,	/* May not be 0 */
	CONST char * tag,		/* May be "" or 0 */
	CONST char * href,		/* May be "" or 0 */
	HTLinkType * ltype));		/* May be 0 */

/*	Create new or find old parent anchor
**	------------------------------------
**
**	This one is for a reference which is found in a document, and might
**	not be already loaded.
**	Note: You are not guaranteed a new anchor -- you might get an old one,
**	like with fonts.
*/
extern HTParentAnchor * HTAnchor_findAddress PARAMS((
	CONST DocAddress *	address));

/*	Create new or find old named anchor - simple form
**	-------------------------------------------------
**
**	Like the previous one, but simpler to use for simple cases.
**	No post data etc. can be supplied. - kw
*/
extern HTParentAnchor * HTAnchor_findSimpleAddress PARAMS((
	CONST char *	url));

/*	Delete an anchor and possibly related things (auto garbage collection)
**	--------------------------------------------
**
**	The anchor is only deleted if the corresponding document is not loaded.
**	All outgoing links from children are deleted, and children are
**	removed from the sources lists of their targets.
**	We also try to delete the targets whose documents are not loaded.
**	If this anchor's sources list is empty, we delete it and its children.
*/
extern BOOL HTAnchor_delete PARAMS((
	HTParentAnchor0 *	me));

/*
 *  Unnamed children (children_notag) have no sense without HText -
 *  delete them and their links if we are about to free HText.
 *  Document currently exists.  Called within HText_free().
 */
extern void HTAnchor_delete_links PARAMS((
	HTParentAnchor *	me));

#ifdef USE_SOURCE_CACHE
extern void HTAnchor_clearSourceCache PARAMS((
	HTParentAnchor *	me));
#endif

/*	Data access functions
**	---------------------
*/
extern HTParentAnchor * HTAnchor_parent PARAMS((
	HTAnchor *		me));

extern void HTAnchor_setDocument PARAMS((
	HTParentAnchor *	me,
	HyperDoc *		doc));

extern HyperDoc * HTAnchor_document PARAMS((
	HTParentAnchor *	me));

/*	Returns the full URI of the anchor, child or parent
**	as a malloc'd string to be freed by the caller.
*/
extern char * HTAnchor_address PARAMS((
	HTAnchor *		me));

extern void HTAnchor_setFormat PARAMS((
	HTParentAnchor *	me,
	HTFormat		form));

extern HTFormat HTAnchor_format PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setIndex PARAMS((
	HTParentAnchor *	me,
	CONST char *		address));

extern void HTAnchor_setPrompt PARAMS((
	HTParentAnchor *	me,
	CONST char *		prompt));

extern BOOL HTAnchor_isIndex PARAMS((
	HTParentAnchor *	me));

extern BOOL HTAnchor_isISMAPScript PARAMS((
	HTAnchor *		me));

#if defined(USE_COLOR_STYLE)
extern CONST char * HTAnchor_style PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setStyle PARAMS((
	HTParentAnchor *	me,
	CONST char *		style));
#endif

/*	Title handling.
*/
extern CONST char * HTAnchor_title PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setTitle PARAMS((
	HTParentAnchor *	me,
	CONST char *		title));

extern void HTAnchor_appendTitle PARAMS((
	HTParentAnchor *	me,
	CONST char *		title));

/*	Bookmark handling.
*/
extern CONST char * HTAnchor_bookmark PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setBookmark PARAMS((
	HTParentAnchor *	me,
	CONST char *		bookmark));

/*	Owner handling.
*/
extern CONST char * HTAnchor_owner PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setOwner PARAMS((
	HTParentAnchor *	me,
	CONST char *		owner));

/*	TITLE handling in LINKs with REV="made" or REV="owner". - FM
*/
extern CONST char * HTAnchor_RevTitle PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setRevTitle PARAMS((
	HTParentAnchor *	me,
	CONST char *		title));

/*	Citehost for bibp links from LINKs with REL="citehost". - RDC
*/
extern CONST char * HTAnchor_citehost PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setCitehost PARAMS((
	HTParentAnchor *	me,
	CONST char *		citehost));

/*	Suggested filename handling. - FM
**	(will be loaded if we had a Content-Disposition
**	 header or META element with filename=name.suffix)
*/
extern CONST char * HTAnchor_SugFname PARAMS((
	HTParentAnchor *	me));

/*	Content-Type handling. - FM
*/
extern CONST char * HTAnchor_content_type PARAMS((
	HTParentAnchor *	me));

/*	Content-Encoding handling. - FM
**	(will be loaded if we had a Content-Encoding
**	 header.)
*/
extern CONST char * HTAnchor_content_encoding PARAMS((
	HTParentAnchor *	me));

/*	Last-Modified header handling. - FM
*/
extern CONST char * HTAnchor_last_modified PARAMS((
	HTParentAnchor *	me));

/*	Date header handling. - FM
*/
extern CONST char * HTAnchor_date PARAMS((
	HTParentAnchor *	me));

/*	Server header handling. - FM
*/
extern CONST char * HTAnchor_server PARAMS((
	HTParentAnchor *	me));

/*	Safe header handling. - FM
*/
extern BOOL HTAnchor_safe PARAMS((
	HTParentAnchor *	me));

/*	Content-Base header handling. - FM
*/
extern CONST char * HTAnchor_content_base PARAMS((
	HTParentAnchor *	me));

/*	Content-Location header handling. - FM
*/
extern CONST char * HTAnchor_content_location PARAMS((
	HTParentAnchor *	me));

/*	Message-ID, used for mail replies - kw
*/
extern CONST char * HTAnchor_messageID PARAMS((
	HTParentAnchor *	me));

extern BOOL HTAnchor_setMessageID PARAMS((
	HTParentAnchor *	me,
	CONST char *		messageid));

/*	Subject, used for mail replies - kw
*/
extern CONST char * HTAnchor_subject PARAMS((
	HTParentAnchor *	me));

extern BOOL HTAnchor_setSubject PARAMS((
	HTParentAnchor *	me,
	CONST char *		subject));

/*	Manipulation of links
**	---------------------
*/
extern HTAnchor * HTAnchor_followLink PARAMS((
	HTChildAnchor *		me));

extern HTAnchor * HTAnchor_followTypedLink PARAMS((
	HTChildAnchor *		me,
	HTLinkType *		type));

/*	Read and write methods
**	----------------------
*/
extern HTList * HTAnchor_methods PARAMS((
	HTParentAnchor *	me));

/*	Protocol
**	--------
*/
extern void * HTAnchor_protocol PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setProtocol PARAMS((
	HTParentAnchor *	me,
	void *			protocol));

/*	Physical address
**	----------------
*/
extern char * HTAnchor_physical PARAMS((
	HTParentAnchor *	me));

extern void HTAnchor_setPhysical PARAMS((
	HTParentAnchor *	me,
	char *			protocol));

extern LYUCcharset * HTAnchor_getUCInfoStage PARAMS((
	HTParentAnchor *	me,
	int			which_stage));

extern int HTAnchor_getUCLYhndl PARAMS((
	HTParentAnchor *	me,
	int			which_stage));

extern LYUCcharset * HTAnchor_setUCInfoStage PARAMS((
	HTParentAnchor *	me,
	int			LYhndl,
	int			which_stage,
	int			set_by));

extern LYUCcharset * HTAnchor_setUCInfoStage PARAMS((
	HTParentAnchor *	me,
	int			LYhndl,
	int			which_stage,
	int			set_by));

extern LYUCcharset * HTAnchor_resetUCInfoStage PARAMS((
	HTParentAnchor *	me,
	int			LYhndl,
	int			which_stage,
	int			set_by));

extern LYUCcharset * HTAnchor_copyUCInfoStage PARAMS((
	HTParentAnchor *	me,
	int			to_stage,
	int			from_stage,
	int			set_by));

extern void ImageMapList_free PARAMS((HTList * list));

#endif /* HTANCHOR_H */
