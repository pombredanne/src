/*	$OpenBSD: if_ether.h,v 1.18 2002/03/01 05:06:24 kjc Exp $	*/
/*	$NetBSD: if_ether.h,v 1.22 1996/05/11 13:00:00 mycroft Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)if_ether.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_IF_ETHER_H_
#define _NETINET_IF_ETHER_H_

/*
 * Ethernet address - 6 octets
 * this is only used by the ethers(3) functions.
 */
struct ether_addr {
	u_int8_t ether_addr_octet[6];
};

/*
 * Some Ethernet constants.
 */
#define	ETHER_ADDR_LEN	6	/* Ethernet address length		*/
#define ETHER_TYPE_LEN	2	/* Ethernet type field length		*/
#define ETHER_CRC_LEN	4	/* Ethernet CRC lenght			*/
#define ETHER_HDR_LEN	((ETHER_ADDR_LEN * 2) + ETHER_TYPE_LEN)
#define ETHER_MIN_LEN	64	/* Minimum frame length, CRC included	*/
#define ETHER_MAX_LEN	1518	/* Maximum frame length, CRC included	*/

/*
 * The length of the combined header.
 */

struct	ether_header {
	u_int8_t  ether_dhost[ETHER_ADDR_LEN];
	u_int8_t  ether_shost[ETHER_ADDR_LEN];
	u_int16_t ether_type;
};

#define	ETHERTYPE_PUP		0x0200	/* PUP protocol */
#define	ETHERTYPE_IP		0x0800	/* IP protocol */
#define	ETHERTYPE_ARP		0x0806	/* address resolution protocol */
#define	ETHERTYPE_REVARP	0x8035	/* reverse addr resolution protocol */
#define	ETHERTYPE_8021Q		0x8100	/* IEEE 802.1Q VLAN tagging */
#define	ETHERTYPE_IPV6		0x86DD	/* IPv6 protocol */
#define	ETHERTYPE_PPPOEDISC	0x8863	/* PPP Over Ethernet Discovery Stage */
#define	ETHERTYPE_PPPOE		0x8864	/* PPP Over Ethernet Session Stage */
#define	ETHERTYPE_LOOPBACK	0x9000	/* used to test interfaces */

/*
 * The ETHERTYPE_NTRAILER packet types starting at ETHERTYPE_TRAIL have
 * (type-ETHERTYPE_TRAIL)*512 bytes of data followed
 * by an ETHER type (as given above) and then the (variable-length) header.
 */
#define	ETHERTYPE_TRAIL		0x1000		/* Trailer packet */
#define	ETHERTYPE_NTRAILER	16

#define	ETHER_IS_MULTICAST(addr) (*(addr) & 0x01) /* is address mcast/bcast? */

#define	ETHERMTU	(ETHER_MAX_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN)
#define	ETHERMIN	(ETHER_MIN_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN)

#ifdef _KERNEL
/*
 * Macro to map an IP multicast address to an Ethernet multicast address.
 * The high-order 25 bits of the Ethernet address are statically assigned,
 * and the low-order 23 bits are taken from the low end of the IP address.
 */
#define ETHER_MAP_IP_MULTICAST(ipaddr, enaddr)				\
	/* struct in_addr *ipaddr; */					\
	/* u_int8_t enaddr[ETHER_ADDR_LEN]; */				\
{									\
	(enaddr)[0] = 0x01;						\
	(enaddr)[1] = 0x00;						\
	(enaddr)[2] = 0x5e;						\
	(enaddr)[3] = ((u_int8_t *)ipaddr)[1] & 0x7f;			\
	(enaddr)[4] = ((u_int8_t *)ipaddr)[2];				\
	(enaddr)[5] = ((u_int8_t *)ipaddr)[3];				\
}

/*
 * Macro to map an IPv6 multicast address to an Ethernet multicast address.
 * The high-order 16 bits of the Ethernet address are statically assigned,
 * and the low-order 32 bits are taken from the low end of the IPv6 address.
 */
#define ETHER_MAP_IPV6_MULTICAST(ip6addr, enaddr)			\
	/* struct in6_addr *ip6addr; */					\
	/* u_int8_t enaddr[ETHER_ADDR_LEN]; */				\
{									\
	(enaddr)[0] = 0x33;						\
	(enaddr)[1] = 0x33;						\
	(enaddr)[2] = ((u_int8_t *)ip6addr)[12];			\
	(enaddr)[3] = ((u_int8_t *)ip6addr)[13];			\
	(enaddr)[4] = ((u_int8_t *)ip6addr)[14];			\
	(enaddr)[5] = ((u_int8_t *)ip6addr)[15];			\
}
#endif

/*
 * Ethernet Address Resolution Protocol.
 *
 * See RFC 826 for protocol description.  Structure below is adapted
 * to resolving internet addresses.  Field names used correspond to 
 * RFC 826.
 */
struct	ether_arp {
	struct	 arphdr ea_hdr;			/* fixed-size header */
	u_int8_t arp_sha[ETHER_ADDR_LEN];	/* sender hardware address */
	u_int8_t arp_spa[4];			/* sender protocol address */
	u_int8_t arp_tha[ETHER_ADDR_LEN];	/* target hardware address */
	u_int8_t arp_tpa[4];			/* target protocol address */
};
#define	arp_hrd	ea_hdr.ar_hrd
#define	arp_pro	ea_hdr.ar_pro
#define	arp_hln	ea_hdr.ar_hln
#define	arp_pln	ea_hdr.ar_pln
#define	arp_op	ea_hdr.ar_op

/*
 * Structure shared between the ethernet driver modules and
 * the address resolution code.  For example, each ec_softc or il_softc
 * begins with this structure.
 */
struct	arpcom {
	struct	 ifnet ac_if;			/* network-visible interface */
	u_int8_t ac_enaddr[ETHER_ADDR_LEN];	/* ethernet hardware address */
	char	 ac__pad[2];			/* pad for some machines */
	LIST_HEAD(, ether_multi) ac_multiaddrs;	/* list of ether multicast addrs */
	int	 ac_multicnt;			/* length of ac_multiaddrs list */
};

struct llinfo_arp {
	LIST_ENTRY(llinfo_arp) la_list;
	struct	rtentry *la_rt;
	struct	mbuf *la_hold;		/* last packet until resolved/timeout */
	long	la_asked;		/* last time we QUERIED for this addr */
#define la_timer la_rt->rt_rmx.rmx_expire /* deletion time in seconds */
};

struct sockaddr_inarp {
	u_int8_t  sin_len;
	u_int8_t  sin_family;
	u_int16_t sin_port;
	struct	  in_addr sin_addr;
	struct	  in_addr sin_srcaddr;
	u_int16_t sin_tos;
	u_int16_t sin_other;
#define SIN_PROXY 1
};

/*
 * IP and ethernet specific routing flags
 */
#define	RTF_USETRAILERS	  RTF_PROTO1	/* use trailers */
#define	RTF_ANNOUNCE	  RTF_PROTO2	/* announce new arp entry */
#define	RTF_PERMANENT_ARP RTF_PROTO3    /* only manual overwrite of entry */

#ifdef	_KERNEL
u_int8_t etherbroadcastaddr[ETHER_ADDR_LEN];
u_int8_t ether_ipmulticast_min[ETHER_ADDR_LEN];
u_int8_t ether_ipmulticast_max[ETHER_ADDR_LEN];
struct	ifqueue arpintrq;

void	arpwhohas __P((struct arpcom *, struct in_addr *));
void	arpintr __P((void));
int	arpresolve __P((struct arpcom *,
	    struct rtentry *, struct mbuf *, struct sockaddr *, u_char *));
void	arp_ifinit __P((struct arpcom *, struct ifaddr *));
void	arp_rtrequest __P((int, struct rtentry *, struct rt_addrinfo *));

int	ether_addmulti __P((struct ifreq *, struct arpcom *));
int	ether_delmulti __P((struct ifreq *, struct arpcom *));
#endif /* _KERNEL */

/*
 * Ethernet multicast address structure.  There is one of these for each
 * multicast address or range of multicast addresses that we are supposed
 * to listen to on a particular interface.  They are kept in a linked list,
 * rooted in the interface's arpcom structure.  (This really has nothing to
 * do with ARP, or with the Internet address family, but this appears to be
 * the minimally-disrupting place to put it.)
 */
struct ether_multi {
	u_int8_t enm_addrlo[ETHER_ADDR_LEN]; /* low  or only address of range */
	u_int8_t enm_addrhi[ETHER_ADDR_LEN]; /* high or only address of range */
	struct	 arpcom *enm_ac;	/* back pointer to arpcom */
	u_int	 enm_refcount;		/* no. claims to this addr/range */
	LIST_ENTRY(ether_multi) enm_list;
};

/*
 * Structure used by macros below to remember position when stepping through
 * all of the ether_multi records.
 */
struct ether_multistep {
	struct ether_multi  *e_enm;
};

/*
 * Macro for looking up the ether_multi record for a given range of Ethernet
 * multicast addresses connected to a given arpcom structure.  If no matching
 * record is found, "enm" returns NULL.
 */
#define ETHER_LOOKUP_MULTI(addrlo, addrhi, ac, enm)			\
	/* u_int8_t addrlo[ETHER_ADDR_LEN]; */				\
	/* u_int8_t addrhi[ETHER_ADDR_LEN]; */				\
	/* struct arpcom *ac; */					\
	/* struct ether_multi *enm; */					\
{									\
	for ((enm) = (ac)->ac_multiaddrs.lh_first;			\
	    (enm) != NULL &&						\
	    (bcmp((enm)->enm_addrlo, (addrlo), ETHER_ADDR_LEN) != 0 ||	\
	     bcmp((enm)->enm_addrhi, (addrhi), ETHER_ADDR_LEN) != 0);	\
		(enm) = (enm)->enm_list.le_next);			\
}

/*
 * Macro to step through all of the ether_multi records, one at a time.
 * The current position is remembered in "step", which the caller must
 * provide.  ETHER_FIRST_MULTI(), below, must be called to initialize "step"
 * and get the first record.  Both macros return a NULL "enm" when there
 * are no remaining records.
 */
#define ETHER_NEXT_MULTI(step, enm) \
	/* struct ether_multistep step; */  \
	/* struct ether_multi *enm; */  \
{ \
	if (((enm) = (step).e_enm) != NULL) \
		(step).e_enm = (enm)->enm_list.le_next; \
}

#define ETHER_FIRST_MULTI(step, ac, enm) \
	/* struct ether_multistep step; */ \
	/* struct arpcom *ac; */ \
	/* struct ether_multi *enm; */ \
{ \
	(step).e_enm = (ac)->ac_multiaddrs.lh_first; \
	ETHER_NEXT_MULTI((step), (enm)); \
}

#ifdef _KERNEL

extern struct ifnet *myip_ifp;

void arp_rtrequest __P((int, struct rtentry *, struct rt_addrinfo *));
int arpresolve __P((struct arpcom *, struct rtentry *, struct mbuf *,
		    struct sockaddr *, u_char *));
void arpintr __P((void));
int arpioctl __P((u_long, caddr_t));
void arp_ifinit __P((struct arpcom *, struct ifaddr *));
void arprequest __P((struct ifnet *, u_int32_t *, u_int32_t *, u_int8_t *));
void revarpinput __P((struct mbuf *));
void in_revarpinput __P((struct mbuf *));
void revarprequest __P((struct ifnet *));
int revarpwhoarewe __P((struct ifnet *, struct in_addr *, struct in_addr *));
int revarpwhoami __P((struct in_addr *, struct ifnet *));
int db_show_arptab __P((void));

#else

char *ether_ntoa __P((struct ether_addr *));
struct ether_addr *ether_aton __P((char *));
int ether_ntohost __P((char *, struct ether_addr *));
int ether_hostton __P((char *, struct ether_addr *));
int ether_line __P((char *, struct ether_addr *, char *));

#endif /* _KERNEL */
#endif /* _NETINET_IF_ETHER_H_ */
