/* ====================================================================
 * Copyright (c) 2008 The OpenSSL Project.  All rights reserved.
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
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */

#include "modes.h"
#include <string.h>

#ifndef MODES_DEBUG
# ifndef NDEBUG
#  define NDEBUG
# endif
#endif
#include <assert.h>

#define STRICT_ALIGNMENT 1
#if defined(__i386) || defined(__i386__) || \
    defined(__x86_64) || defined(__x86_64__) || \
    defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64) || \
    defined(__s390__) || defined(__s390x__)
#  undef STRICT_ALIGNMENT
#  define STRICT_ALIGNMENT 0
#endif

void CRYPTO_cbc128_encrypt(const unsigned char *in, unsigned char *out,
			size_t len, const void *key,
			unsigned char ivec[16], block128_f block)
{
	size_t n;
	const unsigned char *iv = ivec;

	assert(in && out && key && ivec);

#if !defined(OPENSSL_SMALL_FOOTPRINT)
	if (STRICT_ALIGNMENT &&
	    ((size_t)in|(size_t)out|(size_t)ivec)%sizeof(size_t) != 0) {
		while (len>=16) {
			for(n=0; n<16; ++n)
				out[n] = in[n] ^ iv[n];
			(*block)(out, out, key);
			iv = out;
			len -= 16;
			in  += 16;
			out += 16;
		}
	} else {
		while (len>=16) {
			for(n=0; n<16; n+=sizeof(size_t))
				*(size_t*)(out+n) =
				*(size_t*)(in+n) ^ *(size_t*)(iv+n);
			(*block)(out, out, key);
			iv = out;
			len -= 16;
			in  += 16;
			out += 16;
		}
	}
#endif
	while (len) {
		for(n=0; n<16 && n<len; ++n)
			out[n] = in[n] ^ iv[n];
		for(; n<16; ++n)
			out[n] = iv[n];
		(*block)(out, out, key);
		iv = out;
		if (len<=16) break;
		len -= 16;
		in  += 16;
		out += 16;
	}
	memcpy(ivec,iv,16);
}

void CRYPTO_cbc128_decrypt(const unsigned char *in, unsigned char *out,
			size_t len, const void *key,
			unsigned char ivec[16], block128_f block)
{
	size_t n;
	union { size_t align; unsigned char c[16]; } tmp;

	assert(in && out && key && ivec);

#if !defined(OPENSSL_SMALL_FOOTPRINT)
	if (in != out) {
		const unsigned char *iv = ivec;

		if (STRICT_ALIGNMENT &&
		    ((size_t)in|(size_t)out|(size_t)ivec)%sizeof(size_t) != 0) {
			while (len>=16) {
				(*block)(in, out, key);
				for(n=0; n<16; ++n)
					out[n] ^= iv[n];
				iv = in;
				len -= 16;
				in  += 16;
				out += 16;
			}
		}
		else {
			while (len>=16) {
				(*block)(in, out, key);
				for(n=0; n<16; n+=sizeof(size_t))
					*(size_t *)(out+n) ^= *(size_t *)(iv+n);
				iv = in;
				len -= 16;
				in  += 16;
				out += 16;
			}
		}
		memcpy(ivec,iv,16);
	} else {
		if (STRICT_ALIGNMENT &&
		    ((size_t)in|(size_t)out|(size_t)ivec)%sizeof(size_t) != 0) {
			unsigned char c;
			while (len>=16) {
				(*block)(in, tmp.c, key);
				for(n=0; n<16; ++n) {
					c = in[n];
					out[n] = tmp.c[n] ^ ivec[n];
					ivec[n] = c;
				}
				len -= 16;
				in  += 16;
				out += 16;
			}
		}
		else {
			size_t c;
			while (len>=16) {
				(*block)(in, tmp.c, key);
				for(n=0; n<16; n+=sizeof(size_t)) {
					c = *(size_t *)(in+n);
					*(size_t *)(out+n) =
					*(size_t *)(tmp.c+n) ^ *(size_t *)(ivec+n);
					*(size_t *)(ivec+n) = c;
				}
				len -= 16;
				in  += 16;
				out += 16;
			}
		}
	}
#endif
	while (len) {
		unsigned char c;
		(*block)(in, tmp.c, key);
		for(n=0; n<16 && n<len; ++n) {
			c = in[n];
			out[n] = tmp.c[n] ^ ivec[n];
			ivec[n] = c;
		}
		if (len<=16) {
			for (; n<16; ++n)
				ivec[n] = in[n];
			break;
		}
		len -= 16;
		in  += 16;
		out += 16;
	}
}
