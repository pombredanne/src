/*	$OpenBSD: ipmon.c,v 1.18 1999/02/05 05:58:48 deraadt Exp $
 * Copyright (C) 1993-1998 by Darren Reed.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and due credit is given
 * to the original author and the contributors.
 */
#if !defined(lint)
static const char sccsid[] = "@(#)ipmon.c	1.21 6/5/96 (C)1993-1997 Darren Reed";
static const char rcsid[] = "@(#)$Id: ipmon.c,v 1.17 1998/11/13 00:04:01 deraadt Exp $";
#endif

#ifndef SOLARIS
#define SOLARIS (defined(__SVR4) || defined(__svr4__)) && defined(sun)
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#if !defined(__SVR4) && !defined(__svr4__)
#include <strings.h>
#include <signal.h>
#include <sys/dir.h>
#else
#include <sys/filio.h>
#include <sys/byteorder.h>
#endif
#include <stdlib.h>
#include <stddef.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/tcp_fsm.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include <sys/uio.h>
#ifndef linux
# include <sys/protosw.h>
# include <sys/user.h>
# include <netinet/ip_var.h>
#endif

#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>

#include <ctype.h>
#include <syslog.h>
#if defined(__OpenBSD__)
# include <netinet/ip_fil_compat.h>
#else
# include <netinet/ip_compat.h>
#endif
#include <netinet/tcpip.h>
#include <netinet/ip_fil.h>
#include <netinet/ip_proxy.h>
#include <netinet/ip_nat.h>
#include <netinet/ip_state.h>


#if	defined(sun) && !defined(SOLARIS2)
#define	STRERROR(x)	sys_errlist[x]
extern	char	*sys_errlist[];
#else
#define	STRERROR(x)	strerror(x)
#endif


struct	flags {
	int	value;
	char	flag;
};

struct	flags	tcpfl[] = {
	{ TH_ACK, 'A' },
	{ TH_RST, 'R' },
	{ TH_SYN, 'S' },
	{ TH_FIN, 'F' },
	{ TH_URG, 'U' },
	{ TH_PUSH,'P' },
	{ 0, '\0' }
};

#if SOLARIS
static	char	*pidfile = "/etc/opt/ipf/ipmon.pid";
#else
# if BSD >= 199306
static	char	*pidfile = "/var/run/ipmon.pid";
# else
static	char	*pidfile = "/etc/ipmon.pid";
# endif
#endif

struct hlist {
	struct hlist *next;
	struct in_addr addr;
	char name[MAXHOSTNAMELEN];
};
#define PRIME 367
static  struct	hlist htable[PRIME];

static	char	line[2048];
static	int	opts = 0;
static	FILE	*newlog = NULL;
static	char	*logfile = NULL;
static	int	donehup = 0;
static	void	usage __P((char *));
static	void	handlehup __P((int));
static	void	write_pid __P((char *));
static	void	flushlogs __P((char *, FILE *));
static	void	print_log __P((int, FILE *, char *, int));
static	void	print_ipflog __P((FILE *, char *, int));
static	void	print_natlog __P((FILE *, char *, int));
static	void	print_statelog __P((FILE *, char *, int));
static	void	dumphex __P((FILE *, u_char *, int));
static	int	read_log __P((int, int *, char *, int, FILE *));
char	*hostname __P((int, struct in_addr));
char	*portname __P((int, char *, u_short));
int	main __P((int, char *[]));

static	void	logopts __P((int, char *));


#define	OPT_SYSLOG	0x001
#define	OPT_RESOLVE	0x002
#define	OPT_HEXBODY	0x004
#define	OPT_VERBOSE	0x008
#define	OPT_HEXHDR	0x010
#define	OPT_TAIL	0x020
#define	OPT_NAT		0x080
#define	OPT_STATE	0x100
#define	OPT_FILTER	0x200
#define	OPT_PORTNUM	0x400
#define	OPT_ALL		(OPT_NAT|OPT_STATE|OPT_FILTER)

#ifndef	LOGFAC
#define	LOGFAC	LOG_LOCAL0
#endif


void handlehup(sig)
int sig;
{
	FILE	*fp;

	if (logfile && (fp = fopen(logfile, "a")))
		newlog = fp;
	donehup = 1;
}


static int read_log(fd, lenp, buf, bufsize, log)
int fd, bufsize, *lenp;
char *buf;
FILE *log;
{
	int	nr;

	nr = read(fd, buf, bufsize);
	if (!nr)
		return 2;
	if ((nr < 0) && (errno != EINTR))
		return -1;
	*lenp = nr;
	return 0;
}


char	*hostname(res, ip)
int	res;
struct	in_addr	ip;
{
	struct hostent *hp;
	struct hlist *hl;

	if (!res)
		return inet_ntoa(ip);

	hl = &htable[ip.s_addr % PRIME];
	for ( ; hl->next != NULL; hl = hl->next) 
		if (hl->addr.s_addr == ip.s_addr)
			return hl->name;

	hl->addr.s_addr = ip.s_addr;
	hl->next = (struct hlist *)calloc(1, sizeof(*hl));
	if (hl->next == NULL) {
		perror("calloc");
		exit(1);
	}
		
	hp = gethostbyaddr((char *)&ip, sizeof(ip), AF_INET);
	if (!hp)
		strlcpy(hl->name, inet_ntoa(ip), sizeof hl->name);
	else
		strlcpy(hl->name, hp->h_name, sizeof hl->name);

	return hl->name;
}


char	*portname(res, proto, port)
int	res;
char	*proto;
u_short	port;
{
	static	char	pname[8];
	struct	servent	*serv;

	(void) sprintf(pname, "%hu", htons(port));
	if (!res || (opts & OPT_PORTNUM))
		return pname;
	serv = getservbyport((int)port, proto);
	if (!serv)
		return pname;
	return serv->s_name;
}


static	void	dumphex(log, buf, len)
FILE	*log;
u_char	*buf;
int	len;
{
	char	line[80];
	int	i, j, k;
	u_char	*s = buf, *t = (u_char *)line;

	for (i = len, j = 0; i; i--, j++, s++) {
		if (j && !(j & 0xf)) {
			*t++ = '\n';
			*t = '\0';
			if (!(opts & OPT_SYSLOG))
				fputs(line, log);
			else
				syslog(LOG_INFO, "%s", line);
			t = (u_char *)line;
			*t = '\0';
		}
		sprintf((char *)t, "%02x", *s & 0xff);
		t += 2;
		if (!((j + 1) & 0xf)) {
			s -= 15;
			sprintf((char *)t, "        ");
			t += 8;
			for (k = 16; k; k--, s++)
				*t++ = (isprint(*s) ? *s : '.');
			s--;
		}
			
		if ((j + 1) & 0xf)
			*t++ = ' ';;
	}

	if (j & 0xf) {
		for (k = 16 - (j & 0xf); k; k--) {
			*t++ = ' ';
			*t++ = ' ';
			*t++ = ' ';
		}
		sprintf((char *)t, "       ");
		t += 7;
		s -= j & 0xf;
		for (k = j & 0xf; k; k--, s++)
			*t++ = (isprint(*s) ? *s : '.');
		*t++ = '\n';
		*t = '\0';
	}
	if (!(opts & OPT_SYSLOG)) {
		fputs(line, log);
		fflush(log);
	} else
		syslog(LOG_INFO, "%s", line);
}

static	void	print_natlog(log, buf, blen)
FILE	*log;
char	*buf;
int	blen;
{
	struct	natlog	*nl;
	iplog_t	*ipl = (iplog_t *)buf;
	char	*t = line;
	struct	tm	*tm;
	int	res, i, len;

	nl = (struct natlog *)((char *)ipl + sizeof(*ipl));
	res = (opts & OPT_RESOLVE) ? 1 : 0;
	tm = localtime((time_t *)&ipl->ipl_sec);
	len = sizeof(line);
	if (!(opts & OPT_SYSLOG)) {
		(void) strftime(t, len, "%d/%m/%Y ", tm);
		i = strlen(t);
		len -= i;
		t += i;
	}
	(void) strftime(t, len, "%T", tm);
	t += strlen(t);
	(void) sprintf(t, ".%-.6ld @%hd ", ipl->ipl_usec, nl->nl_rule + 1);
	t += strlen(t);

	if (nl->nl_type == NL_NEWMAP)
		strcpy(t, "NAT:MAP ");
	else if (nl->nl_type == NL_NEWRDR)
		strcpy(t, "NAT:RDR ");
	else if (nl->nl_type == NL_EXPIRE)
		strcpy(t, "NAT:EXPIRE ");
	else
		sprintf(t, "Type: %d ", nl->nl_type);
	t += strlen(t);

	(void) sprintf(t, "%s,%s <- -> ", hostname(res, nl->nl_inip),
		portname(res, NULL, nl->nl_inport));
	t += strlen(t);
	(void) sprintf(t, "%s,%s ", hostname(res, nl->nl_outip),
		portname(res, NULL, nl->nl_outport));
	t += strlen(t);
	(void) sprintf(t, "[%s,%s]", hostname(res, nl->nl_origip),
		portname(res, NULL, nl->nl_origport));
	t += strlen(t);
	if (nl->nl_type == NL_EXPIRE) {
#ifdef	USE_QUAD_T
		(void) sprintf(t, " Pkts %qd Bytes %qd",
#else
		(void) sprintf(t, " Pkts %ld Bytes %ld",
#endif
				nl->nl_pkts, nl->nl_bytes);
		t += strlen(t);
	}

	*t++ = '\n';
	*t++ = '\0';
	if (opts & OPT_SYSLOG)
		syslog(LOG_INFO, "%s", line);
	else
		(void) fprintf(log, "%s", line);
}


static	void	print_statelog(log, buf, blen)
FILE	*log;
char	*buf;
int	blen;
{
	struct	ipslog *sl;
	iplog_t	*ipl = (iplog_t *)buf;
	struct	protoent *pr;
	char	*t = line, *proto, pname[6];
	struct	tm	*tm;
	int	res, i, len;

	sl = (struct ipslog *)((char *)ipl + sizeof(*ipl));
	res = (opts & OPT_RESOLVE) ? 1 : 0;
	tm = localtime((time_t *)&ipl->ipl_sec);
	len = sizeof(line);
	if (!(opts & OPT_SYSLOG)) {
		(void) strftime(t, len, "%d/%m/%Y ", tm);
		i = strlen(t);
		len -= i;
		t += i;
	}
	(void) strftime(t, len, "%T", tm);
	t += strlen(t);
	(void) sprintf(t, ".%-.6ld ", ipl->ipl_usec);
	t += strlen(t);

	if (sl->isl_type == ISL_NEW)
		strcpy(t, "STATE:NEW ");
	else if (sl->isl_type == ISL_EXPIRE) {
		if (sl->isl_state[0] > TCPS_ESTABLISHED ||
		    sl->isl_state[1] > TCPS_ESTABLISHED)
			strcpy(t, "STATE:CLOSE ");
		else
			strcpy(t, "STATE:EXPIRE ");
	} else if (sl->isl_type == ISL_FLUSH)
		strcpy(t, "STATE:FLUSH ");
	else
		sprintf(t, "Type: %d ", sl->isl_type);
	t += strlen(t);

	pr = getprotobynumber((int)sl->isl_p);
	if (!pr) {
		proto = pname;
		sprintf(proto, "%d", (u_int)sl->isl_p);
	} else
		proto = pr->p_name;

	if (sl->isl_p == IPPROTO_TCP || sl->isl_p == IPPROTO_UDP) {
		(void) sprintf(t, "%s,%s -> ",
			hostname(res, sl->isl_src),
			portname(res, proto, sl->isl_sport));
		t += strlen(t);
		(void) sprintf(t, "%s,%s PR %s",
			hostname(res, sl->isl_dst),
			portname(res, proto, sl->isl_dport), proto);
	} else if (sl->isl_p == IPPROTO_ICMP) {
		(void) sprintf(t, "%s -> ", hostname(res, sl->isl_src));
		t += strlen(t);
		(void) sprintf(t, "%s PR icmp %d",
			hostname(res, sl->isl_dst), sl->isl_itype);
	}
	t += strlen(t);
	if (sl->isl_type != ISL_NEW) {
#ifdef	USE_QUAD_T
		(void) sprintf(t, " Pkts %qd Bytes %qd",
#else
		(void) sprintf(t, " Pkts %ld Bytes %ld",
#endif
				sl->isl_pkts, sl->isl_bytes);
		t += strlen(t);
	}

	*t++ = '\n';
	*t++ = '\0';
	if (opts & OPT_SYSLOG)
		syslog(LOG_INFO, "%s", line);
	else
		(void) fprintf(log, "%s", line);
}


static	void	print_log(logtype, log, buf, blen)
FILE	*log;
char	*buf;
int	logtype, blen;
{
	iplog_t	*ipl;
	char *bp = NULL, *bpo = NULL;
	int psize;

	while (blen > 0) {
		ipl = (iplog_t *)buf;
		if ((u_long)ipl & (sizeof(long)-1)) {
			if (bp)
				bpo = bp;
			bp = (char *)malloc(blen);
			bcopy((char *)ipl, bp, blen);
			if (bpo) {
				free(bpo);
				bpo = NULL;
			}
			buf = bp;
			continue;
		}
		if (ipl->ipl_magic != IPL_MAGIC) {
			/* invalid data or out of sync */
			break;
		}
		psize = ipl->ipl_dsize;
		switch (logtype)
		{
		case IPL_LOGIPF :
			print_ipflog(log, buf, psize);
			break;
		case IPL_LOGNAT :
			print_natlog(log, buf, psize);
			break;
		case IPL_LOGSTATE :
			print_statelog(log, buf, psize);
			break;
		}

		blen -= psize;
		buf += psize;
	}
	if (bp)
		free(bp);
	return;
}


static	void	print_ipflog(log, buf, blen)
FILE	*log;
char	*buf;
int	blen;
{
	struct	protoent *pr;
	tcphdr_t	*tp;
	struct	icmp	*ic;
	struct	tm	*tm;
	char	c[3], pname[8], *t, *proto;
	u_short	hl, p;
	int	i, lvl, res, len;
	ip_t	*ipc, *ip;
	iplog_t	*ipl;
	ipflog_t *ipf;

	ipl = (iplog_t *)buf;
	ipf = (ipflog_t *)((char *)buf + sizeof(*ipl));
	ip = (ip_t *)((char *)ipf + sizeof(*ipf));
	res = (opts & OPT_RESOLVE) ? 1 : 0;
	t = line;
	*t = '\0';
	hl = (ip->ip_hl << 2);
	p = (u_short)ip->ip_p;
	tm = localtime((time_t *)&ipl->ipl_sec);
#ifdef	linux
	ip->ip_len = ntohs(ip->ip_len);
#endif

	len = sizeof(line);
	if (!(opts & OPT_SYSLOG)) {
		(void) strftime(t, len, "%d/%m/%Y ", tm);
		i = strlen(t);
		len -= i;
		t += i;
	}
	(void) strftime(t, len, "%T", tm);
	t += strlen(t);
	(void) sprintf(t, ".%-.6ld ", ipl->ipl_usec);
	t += strlen(t);
	if (ipl->ipl_count > 1) {
		(void) sprintf(t, "%dx ", ipl->ipl_count);
		t += strlen(t);
	}
#if (SOLARIS || \
	(defined(NetBSD) && (NetBSD <= 1991011) && (NetBSD >= 199603)) || \
	(defined(OpenBSD) && (OpenBSD >= 199603))) || defined(linux)
	len = (int)sizeof(ipf->fl_ifname);
	(void) sprintf(t, "%*.*s", len, len, ipf->fl_ifname);
#else
	for (len = 0; len < 3; len++)
		if (!ipf->fl_ifname[len])
			break;
	if (ipf->fl_ifname[len])
		len++;
	(void) sprintf(t, "%*.*s%u", len, len, ipf->fl_ifname, ipf->fl_unit);
#endif
	t += strlen(t);
	(void) sprintf(t, " @%hu:%hu ", ipf->fl_group, ipf->fl_rule + 1);
	pr = getprotobynumber((int)p);
	if (!pr) {
		proto = pname;
		sprintf(proto, "%d", (u_int)p);
	} else
		proto = pr->p_name;

 	if (ipf->fl_flags & FF_SHORT) {
		c[0] = 'S';
		lvl = LOG_ERR;
	} else if (ipf->fl_flags & FR_PASS) {
		if (ipf->fl_flags & FR_LOGP)
			c[0] = 'p';
		else
			c[0] = 'P';
		lvl = LOG_NOTICE;
	} else if (ipf->fl_flags & FR_BLOCK) {
		if (ipf->fl_flags & FR_LOGB)
			c[0] = 'b';
		else
			c[0] = 'B';
		lvl = LOG_WARNING;
	} else if (ipf->fl_flags & FF_LOGNOMATCH) {
		c[0] = 'n';
		lvl = LOG_NOTICE;
	} else {
		c[0] = 'L';
		lvl = LOG_INFO;
	}
	c[1] = ' ';
	c[2] = '\0';
	(void) strcat(line, c);
	t = line + strlen(line);

	if ((p == IPPROTO_TCP || p == IPPROTO_UDP) &&
	    !(ip->ip_off & IP_OFFMASK)) {
		tp = (tcphdr_t *)((char *)ip + hl);
		if (!(ipf->fl_flags & (FI_SHORT << 16))) {
			(void) sprintf(t, "%s,%s -> ",
				hostname(res, ip->ip_src),
				portname(res, proto, tp->th_sport));
			t += strlen(t);
			(void) sprintf(t, "%s,%s PR %s len %hu %hu ",
				hostname(res, ip->ip_dst),
				portname(res, proto, tp->th_dport),
				proto, hl, ip->ip_len);
			t += strlen(t);

			if (p == IPPROTO_TCP) {
				*t++ = '-';
				for (i = 0; tcpfl[i].value; i++)
					if (tp->th_flags & tcpfl[i].value)
						*t++ = tcpfl[i].flag;
				if (opts & OPT_VERBOSE) {
					(void) sprintf(t, " %lu %lu %hu",
						(u_long)tp->th_seq,
						(u_long)tp->th_ack,
						tp->th_win);
					t += strlen(t);
				}
			}
			*t = '\0';
		} else {
			(void) sprintf(t, "%s -> ", hostname(res, ip->ip_src));
			t += strlen(t);
			(void) sprintf(t, "%s PR %s len %hu %hu",
				hostname(res, ip->ip_dst), proto,
				hl, ip->ip_len);
		}
	} else if (p == IPPROTO_ICMP) {
		ic = (struct icmp *)((char *)ip + hl);
		(void) sprintf(t, "%s -> ", hostname(res, ip->ip_src));
		t += strlen(t);
		(void) sprintf(t, "%s PR icmp len %hu %hu icmp %d/%d",
			hostname(res, ip->ip_dst), hl, ntohs(ip->ip_len),
			ic->icmp_type, ic->icmp_code);
		if (ic->icmp_type == ICMP_UNREACH ||
		    ic->icmp_type == ICMP_SOURCEQUENCH ||
		    ic->icmp_type == ICMP_PARAMPROB ||
		    ic->icmp_type == ICMP_REDIRECT ||
		    ic->icmp_type == ICMP_TIMXCEED) {
			ipc = &ic->icmp_ip;
			tp = (tcphdr_t *)((char *)ipc + hl);

			p = (u_short)ipc->ip_p;
			pr = getprotobynumber((int)p);
			if (!pr) {
				proto = pname;
				(void) sprintf(proto, "%d", (int)p);
			} else
				proto = pr->p_name;

			t += strlen(t);
			(void) sprintf(t, " for %s,%s -",
				hostname(res, ipc->ip_src),
				portname(res, proto, tp->th_sport));
			t += strlen(t);
			(void) sprintf(t, " %s,%s PR %s len %hu %hu",
				hostname(res, ipc->ip_dst),
				portname(res, proto, tp->th_dport),
				proto, ipc->ip_hl << 2, ipc->ip_len);
		}
	} else {
		(void) sprintf(t, "%s -> ", hostname(res, ip->ip_src));
		t += strlen(t);
		(void) sprintf(t, "%s PR %s len %hu (%hu)",
			hostname(res, ip->ip_dst), proto, hl, ip->ip_len);
		t += strlen(t);
		if (ip->ip_off & IP_OFFMASK)
			(void) sprintf(t, " frag %s%s%hu@%hu",
				ip->ip_off & IP_MF ? "+" : "",
				ip->ip_off & IP_DF ? "-" : "",
				ip->ip_len - hl,
				(ip->ip_off & IP_OFFMASK) << 3);
	}
	t += strlen(t);

	if (ipf->fl_flags & FR_KEEPSTATE) {
		(void) strcpy(t, " K-S");
		t += strlen(t);
	}

	if (ipf->fl_flags & FR_KEEPFRAG) {
		(void) strcpy(t, " K-F");
		t += strlen(t);
	}

	*t++ = '\n';
	*t++ = '\0';
	if (opts & OPT_SYSLOG)
		syslog(lvl, "%s", line);
	else
		(void) fprintf(log, "%s", line);
	if (opts & OPT_HEXHDR)
		dumphex(log, (u_char *)buf, sizeof(iplog_t));
	if (opts & OPT_HEXBODY)
		dumphex(log, (u_char *)ip, ipf->fl_plen + ipf->fl_hlen);
}


static void usage(prog)
char *prog;
{
	fprintf(stderr, "%s: [-NFhstvxX] [-f <logfile>]\n", prog);
	exit(1);
}


static void write_pid(file)
char *file;
{
	FILE *fp = NULL;
	int fd;

	if ((fd = open(file, O_CREAT|O_TRUNC|O_WRONLY, 0644)) >= 0)
		fp = fdopen(fd, "w");
	if (!fp) {
		close(fd);
		fprintf(stderr, "unable to open/create pid file: %s\n", file);
		return;
	}
	fprintf(fp, "%d", getpid());
	fclose(fp);
	close(fd);
}


static void flushlogs(file, log)
char *file;
FILE *log;
{
	int	fd, cmd, flushed = 0;

	if ((fd = open(file, O_RDWR)) == -1) {
		(void) fprintf(stderr, "%s: open: %s\n", file,STRERROR(errno));
		exit(-1);
	}

	if (ioctl(fd, cmd, &flushed) == 0) {
		printf("%d bytes flushed from log buffer\n",
			flushed);
		fflush(stdout);
	} else
		perror("SIOCIPFFB");
	(void) close(fd);

	if (flushed) {
		if (opts & OPT_SYSLOG)
			syslog(LOG_INFO, "%d bytes flushed from log\n",
				flushed);
		else if (log != stdout)
			fprintf(log, "%d bytes flushed from log\n", flushed);
	}
}


static void logopts(turnon, options)
int turnon;
char *options;
{
	int flags = 0;
	char *s;

	for (s = options; *s; s++)
	{
		switch (*s)
		{
		case 'N' :
			flags |= OPT_NAT;
			break;
		case 'S' :
			flags |= OPT_STATE;
			break;
		case 'I' :
			flags |= OPT_FILTER;
			break;
		default :
			fprintf(stderr, "Unknown log option %c\n", *s);
			exit(1);
		}
	}

	if (turnon)
		opts |= flags;
	else
		opts &= ~(flags);
}


int main(argc, argv)
int argc;
char *argv[];
{
	struct	stat	sb;
	FILE	*log = stdout;
	int	fd[3], doread, n, i;
	int	tr, nr, regular[3], c;
	int	fdt[3], devices = 0, make_daemon = 0;
	char	buf[512], *iplfile[3], *s;
	extern	int	optind;
	extern	char	*optarg;

	fd[0] = fd[1] = fd[2] = -1;
	fdt[0] = fdt[1] = fdt[2] = -1;
	iplfile[0] = IPL_NAME;
	iplfile[1] = IPNAT_NAME;
	iplfile[2] = IPSTATE_NAME;

	while ((c = getopt(argc, argv, "?aDf:FhnN:o:O:pP:sS:tvxX")) != -1)
		switch (c)
		{
		case 'a' :
			opts |= OPT_ALL;
			fdt[0] = IPL_LOGIPF;
			fdt[1] = IPL_LOGNAT;
			fdt[2] = IPL_LOGSTATE;
			break;
		case 'D' :
			make_daemon = 1;
			break;
		case 'f' : case 'I' :
			opts |= OPT_FILTER;
			fdt[0] = IPL_LOGIPF;
			iplfile[0] = optarg;
			break;
		case 'F' :
			flushlogs(iplfile[0], log);
			flushlogs(iplfile[1], log);
			flushlogs(iplfile[2], log);
			break;
		case 'n' :
			opts |= OPT_RESOLVE;
			break;
		case 'N' :
			opts |= OPT_NAT;
			fdt[1] = IPL_LOGNAT;
			iplfile[1] = optarg;
			break;
		case 'o' : case 'O' :
			logopts(c == 'o', optarg);
			fdt[0] = fdt[1] = fdt[2] = -1;
			if (opts & OPT_FILTER)
				fdt[0] = IPL_LOGIPF;
			if (opts & OPT_NAT)
				fdt[1] = IPL_LOGNAT;
			if (opts & OPT_STATE)
				fdt[2] = IPL_LOGSTATE;
			break;
		case 'p' :
			opts |= OPT_PORTNUM;
			break;
		case 'P' :
			pidfile = optarg;
			break;
		case 's' :
			s = strrchr(argv[0], '/');
			if (s == NULL)
				s = argv[0];
			openlog(s, LOG_NDELAY|LOG_PID, LOGFAC);
			opts |= OPT_SYSLOG;
			log = NULL;
			break;
		case 'S' :
			opts |= OPT_STATE;
			fdt[2] = IPL_LOGSTATE;
			iplfile[2] = optarg;
			break;
		case 't' :
			opts |= OPT_TAIL;
			break;
		case 'v' :
			opts |= OPT_VERBOSE;
			break;
		case 'x' :
			opts |= OPT_HEXBODY;
			break;
		case 'X' :
			opts |= OPT_HEXHDR;
			break;
		default :
		case 'h' :
		case '?' :
			usage(argv[0]);
		}

	/*
	 * Default action is to only open the filter log file.
	 */
	if ((fdt[0] == -1) && (fdt[1] == -1) && (fdt[2] == -1))
		fdt[0] = IPL_LOGIPF;

	for (i = 0; i < 3; i++) {
		if (fdt[i] == -1)
			continue;
		if (!strcmp(iplfile[i], "-"))
			fd[i] = 0;
		else {
			if ((fd[i] = open(iplfile[i], O_RDONLY)) == -1) {
				(void) fprintf(stderr,
					       "%s: open: %s\n", iplfile[i],
					       STRERROR(errno));
				exit(-1);
			}

			if (fstat(fd[i], &sb) == -1) {
				(void) fprintf(stderr, "%d: fstat: %s\n",fd[i],
					       STRERROR(errno));
				exit(-1);
			}
			if (!(regular[i] = !S_ISCHR(sb.st_mode)))
				devices++;
		}
	}

	if (!(opts & OPT_SYSLOG)) {
		logfile = argv[optind];
		log = logfile ? fopen(logfile, "a") : stdout;
		if (log == NULL) {
			
			(void) fprintf(stderr, "%s: fopen: %s\n", argv[optind],
				STRERROR(errno));
			exit(-1);
		}
		setvbuf(log, NULL, _IONBF, 0);
	}

	if (make_daemon && (log != stdout)) {
		if (fork() > 0)
			exit(0);
		write_pid(pidfile);
		close(0);
		close(1);
		close(2);
		setsid();
	} else
		write_pid(pidfile);

	signal(SIGHUP, handlehup);

	for (doread = 1; doread; ) {
		nr = 0;

		for (i = 0; i < 3; i++) {
			tr = 0;
			if (fdt[i] == -1)
				continue;
			if (!regular[i]) {
				if (ioctl(fd[i], FIONREAD, &tr) == -1) {
					perror("ioctl(FIONREAD)");
					exit(-1);
				}
			} else {
				tr = (lseek(fd[i], 0, SEEK_CUR) < sb.st_size);
				if (!tr && !(opts & OPT_TAIL))
					doread = 0;
			}
			if (!tr)
				continue;
			nr += tr;

			tr = read_log(fd[i], &n, buf, sizeof(buf), log);
			if (donehup) {
				donehup = 0;
				if (newlog) {
					fclose(log);
					log = newlog;
					newlog = NULL;
				}
			}

			switch (tr)
			{
			case -1 :
				if (opts & OPT_SYSLOG)
					syslog(LOG_ERR, "read: %m\n");
				else
					perror("read");
				doread = 0;
				break;
			case 1 :
				if (opts & OPT_SYSLOG)
					syslog(LOG_ERR, "aborting logging\n");
				else
					fprintf(log, "aborting logging\n");
				doread = 0;
				break;
			case 2 :
				break;
			case 0 :
				if (n > 0) {
					print_log(fdt[i], log, buf, n);
					if (!(opts & OPT_SYSLOG))
						fflush(log);
				}
				break;
			}
		}
		if (!nr && ((opts & OPT_TAIL) || devices))
			sleep(1);
	}
	exit(0);
	/* NOTREACHED */
}
