/*	$OpenBSD: rtl8225reg.h,v 1.4 2009/08/16 18:21:57 jsg Exp $	*/

/*
 * Copyright (c) 2005 Reyk Floeter <reyk@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _DEV_IC_RTL8225REG_H_
#define	_DEV_IC_RTL8225REG_H_

/*
 * Serial bus format for the Realtek RTL8225 Single-chip Transceiver.
 */

#define RTL8225_TWI_DATA_MASK	0xfffffff0
#define RTL8225_TWI_ADDR_MASK	0x1f

#define RTL8225_ANAPARM_0_ON	0xa0000b59
#define RTL8225_ANAPARM_0_OFF	0xa000eb59
#define RTL8225_ANAPARM_1_ON	0x860dec11
#define RTL8225_ANAPARM_1_OFF	0x840dec11

#define RTL8225_INIT_GAIN	{					\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			\
	0x33, 0x80, 0x6c, 0xc5, 0x00, 0x49, 0x06, 0xb5,	/* -78dbm */	\
	0x43, 0x78, 0x69, 0xc5, 0x00, 0x45, 0x06, 0xb1,	/* -74dbm */	\
	0x53, 0x60, 0x66, 0xc5, 0x00, 0x41, 0x06, 0xab,	/* -70dbm */	\
	0x63, 0x58, 0x63, 0xc5, 0x00, 0x3d, 0x06, 0xa5,	/* -66dbm */	\
	0x73, 0x50, 0x62, 0xc5, 0x00, 0x39, 0x06, 0xa1,	/* -62dbm */	\
	0x83, 0x43, 0x61, 0xc5, 0x00, 0x35, 0x06, 0x9b,	/* -58dbm */	\
	0x93, 0x38, 0x5a, 0xc5, 0x00, 0x31, 0x06, 0x99	/* -54dbm */	\
}

#define RTL8225_AGC	{						\
	0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e,	\
	0x9d, 0x9c, 0x9b, 0x9a, 0x99, 0x98, 0x97, 0x96,	0x95, 0x94,	\
	0x93, 0x92, 0x91, 0x90, 0x8f, 0x8e, 0x8d, 0x8c, 0x8b, 0x8a,	\
	0x89, 0x88, 0x87, 0x86,	0x85, 0x84, 0x83, 0x82, 0x81, 0x80,	\
	0x3f, 0x3e, 0x3d, 0x3c, 0x3b, 0x3a, 0x39, 0x38, 0x37, 0x36,	\
	0x35, 0x34, 0x33, 0x32, 0x31, 0x30, 0x2f, 0x2e, 0x2d, 0x2c,	\
	0x2b, 0x2a, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22,	\
	0x21, 0x20, 0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18,	\
	0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0f, 0x0e,	\
	0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06,	0x05, 0x04,	\
	0x03, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,	\
	0x01, 0x01, 0x01, 0x01,	0x01, 0x01, 0x01, 0x01, 0x01, 0x01,	\
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01			\
}

#define RTL8225_CHANNELS	{					\
	0x085c,	0x08dc,	0x095c,	0x09dc,	0x0a5c,	0x0adc,	0x0b5c,	0x0bdc,	\
	0x0c5c,	0x0cdc,	0x0d5c,	0x0ddc,	0x0e5c,	0x0f5c			\
}

#define RTL8225_RXGAIN_REV_A	{					\
	0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0408, 0x0409,	\
	0x040a, 0x040b, 0x0502, 0x0503, 0x0504, 0x0505, 0x0540, 0x0541,	\
	0x0542, 0x0543, 0x0544, 0x0545, 0x0580, 0x0581, 0x0582, 0x0583,	\
	0x0584, 0x0585, 0x0588, 0x0589, 0x058a, 0x058b, 0x0643, 0x0644,	\
	0x0645, 0x0680, 0x0681, 0x0682, 0x0683, 0x0684, 0x0685, 0x0688,	\
	0x0689, 0x068a, 0x068b, 0x068c, 0x0742, 0x0743, 0x0744, 0x0745,	\
	0x0780, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x0788, 0x0789,	\
	0x078a, 0x078b, 0x078c, 0x078d, 0x0790, 0x0791, 0x0792, 0x0793,	\
	0x0794, 0x0795, 0x0798, 0x0799, 0x079a, 0x079b, 0x079c, 0x079d,	\
	0x07a0, 0x07a1, 0x07a2, 0x07a3, 0x07a4, 0x07a5, 0x07a8, 0x07a9,	\
	0x07aa, 0x07ab, 0x07ac, 0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad,	\
	0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad		\
}

#define RTL8225_RXGAIN_REV_BC	{					\
	0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0408, 0x0409,	\
	0x040a, 0x040b, 0x0502, 0x0503, 0x0504, 0x0505, 0x0540, 0x0541,	\
	0x0542, 0x0543, 0x0544, 0x0545, 0x0580, 0x0581, 0x0582, 0x0583,	\
	0x0584, 0x0585, 0x0588, 0x0589, 0x058a, 0x058b, 0x0643, 0x0644,	\
	0x0645, 0x0680, 0x0681, 0x0682, 0x0683, 0x0684, 0x0685, 0x0688,	\
	0x0689, 0x068a, 0x068b, 0x068c, 0x0742, 0x0743, 0x0744, 0x0745,	\
	0x0780, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x0788, 0x0789,	\
	0x078a, 0x078b, 0x078c, 0x078d, 0x0790, 0x0791, 0x0792, 0x0793,	\
	0x0794, 0x0795, 0x0798, 0x0799, 0x039a, 0x039b, 0x039c, 0x039d,	\
	0x03a0, 0x03a1, 0x03a2, 0x03a3, 0x03a4, 0x03a5, 0x03a8, 0x03a9,	\
	0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03b0, 0x03b1, 0x03b2, 0x03b3,	\
	0x03b4, 0x03b5, 0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bb		\
}

#define RTL8225_RXGAIN_REV_D	{					\
	0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0408, 0x0409,	\
	0x040a, 0x040b, 0x0502, 0x0503, 0x0504, 0x0505, 0x0540, 0x0541,	\
	0x0542, 0x0543, 0x0544, 0x0545, 0x0580, 0x0581, 0x0582, 0x0583,	\
	0x0584, 0x0585, 0x0588, 0x0589, 0x058a, 0x058b, 0x0643, 0x0644,	\
	0x0645, 0x0680, 0x0681, 0x0682, 0x0683, 0x0684, 0x0685, 0x0688,	\
	0x0689, 0x068a, 0x068b, 0x068c, 0x0742, 0x0743, 0x0744, 0x0745,	\
	0x0780, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x0788, 0x0789,	\
	0x078a, 0x078b, 0x078c, 0x078d, 0x0790, 0x0791, 0x0792, 0x0793,	\
	0x0794, 0x0795, 0x0798, 0x0799, 0x079a, 0x079b, 0x079c, 0x079d,	\
	0x07a0, 0x07a1, 0x07a2, 0x07a3, 0x07a4, 0x07a5, 0x07a8, 0x07a9,	\
	0x07aa, 0x07ab, 0x07ac, 0x07ad, 0x07b0, 0x07b1, 0x07b2, 0x07b3,	\
	0x07b4, 0x07b5, 0x07b8, 0x07b9, 0x07ba, 0x07bb, 0x07bb		\
}

#define RTL8225_TXGAIN	{						\
	0x02, 0x06, 0x0e, 0x1e, 0x3e, 0x7e				\
}

#define RTL8225_TXPOWER_OFDM	{					\
	0x80, 0x90, 0xa2, 0xb5, 0xcb, 0xe4				\
}

#define RTL8225_TXPOWER_CCK	{					\
	0x18, 0x17, 0x15, 0x11, 0x0c, 0x08, 0x04, 0x02,	0x1b, 0x1a,	\
	0x17, 0x13, 0x0e, 0x09, 0x04, 0x02, 0x1f, 0x1e, 0x1a, 0x15,	\
	0x10, 0x0a, 0x05, 0x02,	0x22, 0x21, 0x1d, 0x18, 0x11, 0x0b,	\
	0x06, 0x02, 0x26, 0x25, 0x21, 0x1b, 0x14, 0x0d, 0x06, 0x03,	\
	0x2b, 0x2a, 0x25, 0x1e, 0x16, 0x0e, 0x07, 0x03			\
}

#define RTL8225_TXPOWER_CCK14	{					\
	0x18, 0x17, 0x15, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x1b, 0x1a,	\
	0x17, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1e, 0x1a, 0x0f,	\
	0x00, 0x00, 0x00, 0x00,	0x22, 0x21, 0x1d, 0x11, 0x00, 0x00,	\
	0x00, 0x00, 0x26, 0x25, 0x21, 0x13, 0x00, 0x00, 0x00, 0x00,	\
	0x2b, 0x2a, 0x25, 0x15, 0x00, 0x00, 0x00, 0x00			\
}

#endif /* _DEV_IC_RTL8225REG_H_ */
