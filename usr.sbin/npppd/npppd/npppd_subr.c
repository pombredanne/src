/*-
 * Copyright (c) 2009 Internet Initiative Japan Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/**@file
 * npppd の補助的な関数を提供します。
 */
/* $Id: npppd_subr.c 38793 2009-12-17 06:10:08Z yasuoka $ */
#include <sys/cdefs.h>
#ifndef LINT
__COPYRIGHT(
"@(#) Copyright (c) 2000 Ruslan Ermilov and Brian Somers <brian@Awfulhak.org>\n"
"@(#) All rights reserved.\n"
);
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <net/route.h>
#include <net/if_dl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <syslog.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <resolv.h>

#include "debugutil.h"
#include "addr_range.h"

#include "npppd_defs.h"
#include "npppd_subr.h"
#include "rtev.h"
#ifdef NPPPD_USE_RT_ZEBRA
#include "bytebuf.h"
#include <event.h>
#include "rt_zebra.h"
#endif

static u_int16_t route_seq = 0;
static int  in_route0(int, struct in_addr *, struct in_addr *, struct in_addr *, int, const char *, uint32_t);
#define ROUNDUP(a) \
	((a) > 0 ? (1 + (((a) - 1) | (sizeof(long) - 1))) : sizeof(long))

static const char *
skip_space(const char *s)
{
	const char *r;
	for (r = s; *r != '\0' && isspace((unsigned char)*r); r++)
		;; /* skip */

	return r;
}

/**
 * resolv.conf からネームサーバの IPv4 のエントリを読み出します。
 * resolv.conf のパスは、resolv.h の _PATH_RESCONF を使います。
 */
int
load_resolv_conf(struct in_addr *pri, struct in_addr *sec)
{
	FILE *filep;
	int i;
	struct in_addr *addr;
	char *ap, *line, buf[BUFSIZ];

	pri->s_addr = INADDR_NONE;
	sec->s_addr = INADDR_NONE;

	filep = NULL;
	if ((filep = fopen(_PATH_RESCONF, "r")) == NULL)
		return 1;

	i = 0;
	while (fgets(buf, sizeof(buf), filep) != NULL) {
		line = (char *)skip_space(buf);
		if (strncmp(line, "nameserver", 10) != 0)
			continue;
		line += 10;
		if (!isspace((unsigned char)*line))
			continue;
		while ((ap = strsep(&line, " \t\r\n")) != NULL) {
			if (*ap == '\0')
				continue;
			if (i == 0)
				addr = pri;
			else
				addr = sec;
			if (inet_aton(ap, addr) != 1) {
				/*
				 * FIXME IPv6 使ってるの場合 IPv6 アドレスが入
				 * っているかもしれない。とりあえず、continue。
				 */
				continue;
				//goto reigai;
			}
			addr->s_addr = addr->s_addr;
			if (++i >= 2)
				goto end_loop;
		}
	}
end_loop:
	if (filep != NULL)
		fclose(filep);

	return 0;
}

// 経路追加削除
static int
in_route0(int type, struct in_addr *dest, struct in_addr *mask,
    struct in_addr *gate, int mtu, const char *ifname, uint32_t rtm_flags)
{
	struct rt_msghdr *rtm;
	struct sockaddr_in sdest, smask, sgate;
	struct sockaddr_dl *sdl;
	char dl_buf[512];	// enough size
	char *cp, buf[sizeof(*rtm) + sizeof(struct sockaddr_in) * 3 +
	    sizeof(dl_buf) + 128];
	const char *strtype;
#ifndef NPPPD_USE_RTEV_WRITE
	int rval, dummy, flags, sock;

	sock = -1;
#endif

	ASSERT(type == RTM_ADD || type == RTM_DELETE);
#ifdef NPPPD_USE_RT_ZEBRA
	if ((rtm_flags & RTF_BLACKHOLE) != 0) {
		if (type == RTM_ADD)
			return rt_zebra_add_ipv4_blackhole_rt(
			    rt_zebra_get_instance(), dest->s_addr,
			    (mask == NULL)? 0xffffffffL : mask->s_addr);
		else
			return rt_zebra_delete_ipv4_blackhole_rt(
			    rt_zebra_get_instance(), dest->s_addr,
			    (mask == NULL)? 0xffffffffL : mask->s_addr);
		return -1;
	}
#endif
	if(type == RTM_ADD)
		strtype = "RTM_ADD";
	else
		strtype = "RTM_DELETE";

	memset(buf, 0, sizeof(buf));
	memset(&sdest, 0, sizeof(sdest));
	memset(&smask, 0, sizeof(smask));
	memset(&sgate, 0, sizeof(sgate));
	memset(&dl_buf, 0, sizeof(dl_buf));

	sdl = (struct sockaddr_dl *)dl_buf;

	sdest.sin_addr = *dest;
	if (mask != NULL)
		smask.sin_addr = *mask;
	if (gate != NULL)
		sgate.sin_addr = *gate;

	sdest.sin_family = smask.sin_family = sgate.sin_family = AF_INET;
	sdest.sin_len = smask.sin_len = sgate.sin_len = sizeof(sgate);

	rtm = (struct rt_msghdr *)buf;

	rtm->rtm_version = RTM_VERSION;
	rtm->rtm_type = type;
	rtm->rtm_flags = RTF_DONE | rtm_flags;
	if (gate != NULL)
		rtm->rtm_flags |= RTF_GATEWAY;
	if (mask == NULL)
		rtm->rtm_flags |= RTF_HOST;
#ifdef	RTF_MASK
	else
		rtm->rtm_flags |= RTF_MASK;
#endif
#ifdef	_SEIL_EXT_
	/*
	 * phash で interface のアドレスを変更されると、route flush される。
	 * route flush コマンドに -xpppac (Except pppac) オプションを追加
	 * して、RTF_PROTO2 ならば flush しないような修正を行い、phash で
	 * 使用される の flush_route() では -xpppac 付きで route コマンド
	 * を実行する。
	 */
	rtm->rtm_flags |= RTF_PROTO2;
	/* 
	 * RTF_STATIC が立っていないと Zebra が消す場合がある。
	 */
	rtm->rtm_flags |= RTF_STATIC;
#endif	/* _SEIL_EXT_ */

	if (type == RTM_ADD && mtu > 0) {
		rtm->rtm_inits = RTV_MTU;
		rtm->rtm_rmx.rmx_mtu = mtu;
	}

	if (type == RTM_ADD)
		rtm->rtm_flags |= RTF_UP;

	rtm->rtm_addrs = RTA_DST;
	if (gate != NULL)
		rtm->rtm_addrs |= RTA_GATEWAY;
	if (mask != NULL)
		rtm->rtm_addrs |= RTA_NETMASK;
#ifdef RTA_IFP
	if (ifname != NULL)
		rtm->rtm_addrs |= RTA_IFP;
#endif

	rtm->rtm_pid = getpid();
	route_seq = ((route_seq + 1)&0x0000ffff);
	rtm->rtm_seq = route_seq;

	cp = (char *)rtm;
	cp += ROUNDUP(sizeof(*rtm));

	memcpy(cp, &sdest, sdest.sin_len);
	cp += ROUNDUP(sdest.sin_len);
	if (gate != NULL) {
		memcpy(cp, &sgate, sgate.sin_len);
		cp += ROUNDUP(sgate.sin_len);
	}
	if (mask != NULL) {
		memcpy(cp, &smask, smask.sin_len);
		cp += ROUNDUP(smask.sin_len);
	}
#ifdef RTA_IFP
	if (ifname != NULL) {
		strlcpy(sdl->sdl_data, ifname, IFNAMSIZ);
		sdl->sdl_family = AF_LINK;
		sdl->sdl_len = offsetof(struct sockaddr_dl, sdl_data) +IFNAMSIZ;
		memcpy(cp, sdl, sdl->sdl_len);
		cp += ROUNDUP(sdl->sdl_len);
	}
#endif

	rtm->rtm_msglen = cp - buf;

#ifdef NPPPD_USE_RTEV_WRITE
	if (rtev_write(rtm) < 0)
		log_printf(LOG_ERR, "rtev_write failed in %s: %m", __func__);
#else

	if ((sock = socket(PF_ROUTE, SOCK_RAW, AF_UNSPEC)) < 0) {
		log_printf(LOG_ERR, "socket() failed in %s() on %s : %m",
		    __func__, strtype);
		goto reigai;
	}

	dummy = 0;
	if ((flags = fcntl(sock, F_GETFL, dummy)) < 0) {
		log_printf(LOG_ERR, "fcntl(,F_GETFL) failed on %s : %m",
		    __func__);
		goto reigai;
	}

	if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
		log_printf(LOG_ERR, "fcntl(,F_SETFL) failed on %s : %m",
		    __func__);
		goto reigai;
	}

	if ((rval = write(sock, buf, rtm->rtm_msglen)) <= 0) {
		if (!(type == RTM_DELETE && errno == ESRCH) &&
		    !(type == RTM_ADD    && errno == EEXIST)) {
			log_printf(LOG_DEBUG,
			    "write() failed in %s on %s : %m", __func__,
			    strtype);
		} else {
			log_printf(LOG_WARNING,
			    "write() failed in %s on %s : %m", __func__,
			    strtype);
		}
		goto reigai;
	}

	close(sock);
#endif

	return 0;

#ifndef NPPPD_USE_RTEV_WRITE
reigai:
	if (sock >= 0)
		close(sock);
#endif
	
	return 1;
}

/** ホスト経路を追加 */
int
in_host_route_add(struct in_addr *dest, struct in_addr *gate,
    const char *ifname, int mtu)
{
	return in_route0(RTM_ADD, dest, NULL, gate, mtu, ifname, 0);
}

/** ホスト経路を削除 */
int
in_host_route_delete(struct in_addr *dest, struct in_addr *gate)
{
	return in_route0(RTM_DELETE, dest, NULL, gate, 0, NULL, 0);
}

/** ネット経路を追加 */
int
in_route_add(struct in_addr *dest, struct in_addr *mask, struct in_addr *gate,
    const char *ifname, uint32_t rtm_flags, int mtu)
{
	return in_route0(RTM_ADD, dest, mask, gate, mtu, ifname, rtm_flags);
}

/** ネット経路を削除 */
int
in_route_delete(struct in_addr *dest, struct in_addr *mask,
    struct in_addr *gate, uint32_t rtm_flags)
{
	return in_route0(RTM_DELETE, dest, mask, gate, 0, NULL, rtm_flags);
}

// 従来 ppp の ip.c より流用
/**
 *  Check whether a packet should reset idle timer
 *  Returns 1 to don't reset timer (i.e. the packet is "idle" packet)
 */
int
ip_is_idle_packet(const struct ip * pip, int len)
{
	u_int16_t ip_off;
	const struct udphdr *uh;

	/*
         * フラグメントされたパケットはアイドルパケットではない
         * （フラグメントするほど長いパケットはアイドルパケットではない）
         */
	ip_off = ntohs(pip->ip_off);
	if ((ip_off & IP_MF) || ((ip_off & IP_OFFMASK) != 0))
		return 0;

	switch (pip->ip_p) {
	case IPPROTO_IGMP:
		return 1;
	case IPPROTO_ICMP:
		/* 長さは足りている？ */
		if (pip->ip_hl * 4 + 8 > len)
			return 1;

		switch (((unsigned char *) pip)[pip->ip_hl * 4]) {
		case 0:	/* Echo Reply */
		case 8:	/* Echo Request */
			return 0;
		default:
			return 1;
		}
	case IPPROTO_UDP:
	case IPPROTO_TCP:
		/*
		 * UDP も TCP も、ポートの部分は同じなので、そこだけは共用で
		 * きる。
		 */
		uh = (const struct udphdr *) (((const char *) pip) +
		    (pip->ip_hl * 4));

		/* 長さは足りている？ */
		if (pip->ip_hl * 4 + sizeof(struct udphdr) > len)
			return 1;

		switch (ntohs(uh->uh_sport)) {
		case 53:	/* DOMAIN */
		case 67:	/* BOOTPS */
		case 68:	/* BOOTPC */
		case 123:	/* NTP */
		case 137:	/* NETBIOS-NS */
		case 520:	/* RIP */
			return 1;
		}
		switch (ntohs(uh->uh_dport)) {
		case 53:	/* DOMAIN */
		case 67:	/* BOOTPS */
		case 68:	/* BOOTPC */
		case 123:	/* NTP */
		case 137:	/* NETBIOS-NS */
		case 520:	/* RIP */
			return 1;
		}
		return 0;
	default:
		return 0;
	}
}

/***********************************************************************
 * プールしているアドレスへの経路追加/削除
 ***********************************************************************/
void
in_addr_range_add_route(struct in_addr_range *range)
{
	struct in_addr_range *range0;
	struct in_addr dest, mask, loop;

	for (range0 = range; range0 != NULL; range0 = range0->next){
		dest.s_addr = htonl(range0->addr);
		mask.s_addr = htonl(range0->mask);
		loop.s_addr = htonl(INADDR_LOOPBACK);
		in_route_add(&dest, &mask, &loop, LOOPBACK_IFNAME,
		    RTF_BLACKHOLE, 0);
	}
	log_printf(LOG_INFO, "Added routes for pooled addresses");
}

void
in_addr_range_delete_route(struct in_addr_range *range)
{
	struct in_addr_range *range0;
	struct in_addr dest, mask, loop;

	for (range0 = range; range0 != NULL; range0 = range0->next){
		dest.s_addr = htonl(range0->addr);
		mask.s_addr = htonl(range0->mask);
		loop.s_addr = htonl(INADDR_LOOPBACK);

		in_route_delete(&dest, &mask, &loop, RTF_BLACKHOLE);
	}
	log_printf(LOG_NOTICE, "Deleted routes for pooled addresses");
}


/* #inlude <arpa/nameser_compat.h>	も GETSHORT を定義している */
#undef	GETCHAR
#undef	GETSHORT
#undef	PUTSHORT

#define GETCHAR(c, cp) { (c) = *(cp)++; }
#define GETSHORT(s, cp) { \
	(s) = *(cp)++ << 8; \
	(s) |= *(cp)++; \
}
#define PUTSHORT(s, cp) { \
	*(cp)++ = (u_char) ((s) >> 8); \
	*(cp)++ = (u_char) (s); \
}
#define TCP_OPTLEN_IN_SEGMENT	12	/* timestamp option and padding */
#define MAXMSS(mtu) (mtu - sizeof(struct ip) - sizeof(struct tcphdr) - \
    TCP_OPTLEN_IN_SEGMENT)

/* adapted from FreeBSD:src/usr.sbin/ppp/tcpmss.c */
/*
 * Copyright (c) 2000 Ruslan Ermilov and Brian Somers <brian@Awfulhak.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/usr.sbin/ppp/tcpmss.c,v 1.1.4.3 2001/07/19 11:39:54 brian Exp $
 */

/*
 * The following macro is used to update an internet checksum.  "acc" is a
 * 32-bit accumulation of all the changes to the checksum (adding in old
 * 16-bit words and subtracting out new words), and "cksum" is the checksum
 * value to be updated.
 */
#define ADJUST_CHECKSUM(acc, cksum) {			\
	acc += cksum;					\
	if (acc < 0) {					\
		acc = -acc;				\
		acc = (acc >> 16) + (acc & 0xffff);	\
		acc += acc >> 16;			\
		cksum = (u_short) ~acc;			\
	} else {					\
		acc = (acc >> 16) + (acc & 0xffff);	\
		acc += acc >> 16;			\
		cksum = (u_short) acc;			\
	}						\
}

/**
 * IPパケットが MTU 以下となるように mss を調整します。
 * @param	pktp	IPパケットのポインタ
 * @param	lpktp	長さ
 * @param	mtu	MTU
 */
int
adjust_tcp_mss(u_char *pktp, int lpktp, int mtu)
{
	int opt, optlen, acc, ip_off, mss, maxmss;
	struct ip *pip;
	struct tcphdr *th;

	if (lpktp < sizeof(struct ip) + sizeof(struct tcphdr))
		return 1;

	pip = (struct ip *)pktp;
	ip_off = ntohs(pip->ip_off);

	/* TCP じゃないパケットやフラグメントされたパケットは対象外 */
	if (pip->ip_p != IPPROTO_TCP || (ip_off & IP_MF) != 0 ||
	    (ip_off & IP_OFFMASK) != 0)
		return 0;

	pktp += pip->ip_hl << 2;
	lpktp -= pip->ip_hl << 2;

	/* 壊れてる */
	if (sizeof(struct tcphdr) > lpktp)
		return 1;

	th = (struct tcphdr *)pktp;
	/* MSS は SYN がセットされたセグメントに限る。(See RFC 793) */
	if ((th->th_flags & TH_SYN) == 0)
		return 0;

	pktp += sizeof(struct tcphdr);
	lpktp -= sizeof(struct tcphdr);

	while (lpktp >= TCPOLEN_MAXSEG) {
		GETCHAR(opt, pktp);
		switch (opt) {
		case TCPOPT_MAXSEG:
			GETCHAR(optlen, pktp);
			GETSHORT(mss, pktp);
			maxmss = MAXMSS(mtu);
			if (mss > maxmss) {
				pktp-=2;
				PUTSHORT(maxmss, pktp);
				acc = htons(mss);
				acc -= htons(maxmss);
				ADJUST_CHECKSUM(acc, th->th_sum);
			}
			return 0;
			/* NOTREACHED */
			break;
		case TCPOPT_EOL:
			return 0;
			/* NOTREACHED */
			break;
		case TCPOPT_NOP:
			lpktp--;
			break;
		default:
			GETCHAR(optlen, pktp);
			pktp += 2 - optlen;
			lpktp -= optlen;
			break;
		}
	}
	return 0;
}

void
set_faith(int sock, int onoff)
{
#ifdef IP_FAITH
	int ival;

	ival = onoff;
	if(setsockopt(sock, IPPROTO_IP, IP_FAITH, &ival, sizeof(ival))
	    < 0) {
		log_printf(LOG_DEBUG,
		"setsockopt(IP_FAITH) failed at %s(): %m", __func__);
	}
#endif
}
