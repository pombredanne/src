/*     $OpenBSD: pcap.h,v 1.6 1999/02/05 05:58:47 deraadt Exp $     */
/*
 * Copyright (C) 1993-1998 by Darren Reed.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and due credit is given
 * to the original author and the contributors.
 * $Id: pcap.h,v 1.5 1998/01/26 04:16:43 dgregor Exp $
 */
/*
 * This header file is constructed to match the version described by
 * PCAP_VERSION_MAJ.
 *
 * The structure largely derives from libpcap which wouldn't include
 * nicely without bpf.
 */
typedef	struct	pcap_filehdr	{
	u_int	pc_id;
	u_short	pc_v_maj;
	u_short	pc_v_min;
	u_int	pc_zone;
	u_int	pc_sigfigs;
	u_int	pc_slen;
	u_int	pc_type;
} pcaphdr_t;

#define	TCPDUMP_MAGIC		0xa1b2c3d4

#define	PCAP_VERSION_MAJ	2

typedef	struct	pcap_pkthdr	{
	struct	timeval	ph_ts;
	u_int	ph_clen;
	u_int	ph_len;
} pcappkt_t;

