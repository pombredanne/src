/*
 * Copyright (C) 2004  Internet Systems Consortium, Inc. ("ISC")
 * Copyright (C) 1999-2001  Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef IN_1_WKS_11_H
#define IN_1_WKS_11_H 1

/* $ISC: wks_11.h,v 1.20 2004/03/05 05:10:25 marka Exp $ */

typedef	struct dns_rdata_in_wks {
	dns_rdatacommon_t	common;
	isc_mem_t		*mctx;
	struct in_addr		in_addr;
	isc_uint16_t		protocol;
	unsigned char		*map;
	isc_uint16_t		map_len;
} dns_rdata_in_wks_t;

#endif /* IN_1_WKS_11_H */
