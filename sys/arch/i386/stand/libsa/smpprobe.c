/*	$OpenBSD: smpprobe.c,v 1.3.8.1 2000/02/19 17:53:11 niklas Exp $	*/

/*
 * Copyright (c) 1997 Tobias Weingartner
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Tobias Weingartner.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/param.h>
#include <machine/biosvar.h>
#include "libsa.h"

extern int debug;

extern u_int cnvmem;
#define	MP_FLOAT_SIG	0x5F504D5F	/* "_MP_" little endian*/
#define	MP_CONF_SIG	0x504D4350	/* "PCMP" little endian */

typedef struct _mp_float {
	u_int32_t signature;
	u_int32_t conf_addr;
	u_int8_t length;
	u_int8_t spec_rev;
	u_int8_t checksum;
	u_int8_t feature[5];
} mp_float_t;


static __inline int
mp_checksum(ptr, len)
	u_int8_t *ptr;
	int len;
{
	register int i, sum = 0;

#ifdef DEBUG
	printf("Checksum %p for %d\n", ptr, len);
#endif

	for (i = 0; i < len; i++)
		sum += *(ptr + i);

	return ((sum & 0xff) == 0);
}


static mp_float_t *
mp_probefloat(ptr, len)
	u_int8_t *ptr;
	int len;
{
	mp_float_t *mpp;
	int i;

#ifdef DEBUG
	if (debug)
		printf("Checking %p for %d\n", ptr, len);
#endif
	for (i = 0, mpp = (mp_float_t *)ptr; i < len;
	    i += sizeof(mp_float_t), mpp++) {
		if (mpp->signature == MP_FLOAT_SIG) {
#ifdef DEBUG
			if (debug)
				printf("Found possible MP signature at: %p\n",
				    mpp);
#endif
			if (mp_checksum(mpp, mpp->length * 16)) {
#ifdef DEBUG
				if (debug)
					printf("Found valid MP signature at: "
					    "%p\n", ptr);
#endif
				break;
			}
		}
	}

	return (i < len ? mpp : NULL);
}


void
smpprobe()
{
	mp_float_t *mp = NULL;

	/* Check EBDA */
	if (!(mp = mp_probefloat((void *)((*((u_int16_t*)0x40e)) * 16), 1024)) &&
		/* Check BIOS ROM 0xF0000 - 0xFFFFF */
	    !(mp = mp_probefloat((void *)(0xF0000), 0xFFFF)) &&
		/* Check last 1K of base RAM */
	    !(mp = mp_probefloat((void *)(cnvmem * 1024), 1024))) {
		/* No valid MP signature found */
#if DEBUG
		if (debug)
			printf("No valid MP signature found.\n");
#endif
		return;
	}

	/* Valid MP signature found */
	printf(" smp");
#if DEBUG
	if (debug)
		printf("Floating Structure:\n"
		"\tSignature: %x\n"
		"\tConfig at: %x\n"
		"\tLength: %d\n"
		"\tRev: 1.%d\n"
		"\tFeature: %x %x %x %x %x\n",
		mp->signature, mp->conf_addr, mp->length, mp->spec_rev,
		mp->feature[0], mp->feature[1], mp->feature[2],
		mp->feature[3], mp->feature[4]);
#endif
}

