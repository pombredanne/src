/* Vax opcde list.
   Copyright 1989, 1991, 1992, 1995 Free Software Foundation, Inc.

This file is part of GDB and GAS.

GDB and GAS are free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GDB and GAS are distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GDB or GAS; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.  */

#ifndef vax_opcodeT
#define vax_opcodeT int
#endif /* no vax_opcodeT */

struct vot_wot			/* vax opcode table: wot to do with this */
				/* particular opcode */
{
  const char       *args;	/* how to compile said opcode */
  vax_opcodeT       code;	/* op-code (may be > 8 bits!) */
};

struct vot			/* vax opcode text */
{
  const char       *name;	/* opcode name: lowercase string  [key]  */
  struct vot_wot    detail;	/* rest of opcode table          [datum] */
};

#define vot_how args
#define vot_code code
#define vot_detail detail
#define vot_name name

static const struct vot
votstrs[] =
{
{    "halt",	{"",			0x00	} },
{    "nop",	{"",			0x01	} },
{    "rei",	{"",			0x02	} },
{    "bpt",	{"",			0x03	} },
{    "ret",	{"",			0x04	} },
{    "rsb",	{"",			0x05	} },
{    "ldpctx",	{"",			0x06	} },
{    "svpctx",	{"",			0x07	} },
{    "cvtps",	{"rwabrwab",		0x08	} },
{    "cvtsp",	{"rwabrwab",		0x09	} },
{    "index",	{"rlrlrlrlrlwl",	0x0a	} },
{    "crc",	{"abrlrwab",		0x0b	} },
{    "prober",	{"rbrwab",		0x0c	} },
{    "probew",	{"rbrwab",	        0x0d	} },
{    "insque",	{"abab",		0x0e	} },
{    "remque",	{"abwl",		0x0f	} },
{    "bsbb",	{"bb",			0x10	} },
{    "brb",	{"bb",			0x11	} },
{    "bneq",	{"bb",			0x12	} },
{    "bnequ",	{"bb",			0x12	} },
{    "beql",	{"bb",			0x13	} },
{    "beqlu",	{"bb",			0x13	} },
{    "bgtr",	{"bb",			0x14	} },
{    "bleq",	{"bb",			0x15	} },
{    "jsb",	{"ab",			0x16	} },
{    "jmp",	{"ab",			0x17	} },
{    "bgeq",	{"bb",			0x18	} },
{    "blss",	{"bb",			0x19	} },
{    "bgtru",	{"bb",			0x1a	} },
{    "blequ",	{"bb",			0x1b	} },
{    "bvc",	{"bb",			0x1c	} },
{    "bvs",	{"bb",			0x1d	} },
{    "bcc",	{"bb",			0x1e	} },
{    "bgequ",	{"bb",			0x1e	} },
{    "blssu",	{"bb",			0x1f	} },
{    "bcs",	{"bb",			0x1f	} },
{    "addp4",	{"rwabrwab",		0x20	} },
{    "addp6",	{"rwabrwabrwab",	0x21	} },
{    "subp4",	{"rwabrwab",		0x22	} },
{    "subp6",	{"rwabrwabrwab",	0x23	} },
{    "cvtpt",	{"rwababrwab",		0x24	} },
{    "mulp",	{"rwabrwabrwab",	0x25	} },
{    "cvttp",	{"rwababrwab",		0x26	} },
{    "divp",	{"rwabrwabrwab",	0x27	} },
{    "movc3",	{"rwabab",		0x28	} },
{    "cmpc3",	{"rwabab",		0x29	} },
{    "scanc",	{"rwababrb",		0x2a	} },
{    "spanc",	{"rwababrb",		0x2b	} },
{    "movc5",	{"rwabrbrwab",		0x2c	} },
{    "cmpc5",	{"rwabrbrwab",		0x2d	} },
{    "movtc",	{"rwabrbabrwab",	0x2e	} },
{    "movtuc",	{"rwabrbabrwab",	0x2f	} },
{    "bsbw",	{"bw",			0x30	} },
{    "brw",	{"bw",			0x31	} },
{    "cvtwl",	{"rwwl",		0x32	} },
{    "cvtwb",	{"rwwb",		0x33	} },
{    "movp",	{"rwabab",		0x34	} },
{    "cmpp3",	{"rwabab",		0x35	} },
{    "cvtpl",	{"rwabwl",		0x36	} },
{    "cmpp4",	{"rwabrwab",		0x37	} },
{    "editpc",	{"rwababab",		0x38	} },
{    "matchc",	{"rwabrwab",		0x39	} },
{    "locc",	{"rbrwab",		0x3a	} },
{    "skpc",	{"rbrwab",		0x3b	} },
{    "movzwl",	{"rwwl",		0x3c	} },
{    "acbw",	{"rwrwmwbw",		0x3d	} },
{    "movaw",	{"awwl",		0x3e	} },
{    "pushaw",	{"aw",			0x3f	} },
{    "addf2",	{"rfmf",		0x40	} },
{    "addf3",	{"rfrfwf",		0x41	} },
{    "subf2",	{"rfmf",		0x42	} },
{    "subf3",	{"rfrfwf",		0x43	} },
{    "mulf2",	{"rfmf",		0x44	} },
{    "mulf3",	{"rfrfwf",		0x45	} },
{    "divf2",	{"rfmf",		0x46	} },
{    "divf3",	{"rfrfwf",		0x47	} },
{    "cvtfb",	{"rfwb",		0x48	} },
{    "cvtfw",	{"rfww",		0x49	} },
{    "cvtfl",	{"rfwl",		0x4a	} },
{    "cvtrfl",	{"rfwl",		0x4b	} },
{    "cvtbf",	{"rbwf",		0x4c	} },
{    "cvtwf",	{"rwwf",		0x4d	} },
{    "cvtlf",	{"rlwf",		0x4e	} },
{    "acbf",	{"rfrfmfbw",		0x4f	} },
{    "movf",	{"rfwf",		0x50	} },
{    "cmpf",	{"rfrf",		0x51	} },
{    "mnegf",	{"rfwf",		0x52	} },
{    "tstf",	{"rf",			0x53	} },
{    "emodf",	{"rfrbrfwlwf",		0x54	} },
{    "polyf",	{"rfrwab",		0x55	} },
{    "cvtfd",	{"rfwd",		0x56	} },
                                        /* opcode 57 is not defined yet */
{    "adawi",	{"rwmw",		0x58	} },
                                        /* opcode 59 is not defined yet */
                                        /* opcode 5a is not defined yet */
                                        /* opcode 5b is not defined yet */
{    "insqhi",	{"abaq",		0x5c	} },
{    "insqti",	{"abaq",		0x5d	} },
{    "remqhi",	{"aqwl",		0x5e	} },
{    "remqti",	{"aqwl",		0x5f	} },
{    "addd2",	{"rdmd",		0x60	} },
{    "addd3",	{"rdrdwd",		0x61	} },
{    "subd2",	{"rdmd",		0x62	} },
{    "subd3",	{"rdrdwd",		0x63	} },
{    "muld2",	{"rdmd",		0x64	} },
{    "muld3",	{"rdrdwd",		0x65	} },
{    "divd2",	{"rdmd",		0x66	} },
{    "divd3",	{"rdrdwd",		0x67	} },
{    "cvtdb",	{"rdwb",		0x68	} },
{    "cvtdw",	{"rdww",		0x69	} },
{    "cvtdl",	{"rdwl",		0x6a	} },
{    "cvtrdl",	{"rdwl",		0x6b	} },
{    "cvtbd",	{"rbwd",		0x6c	} },
{    "cvtwd",	{"rwwd",		0x6d	} },
{    "cvtld",	{"rlwd",		0x6e	} },
{    "acbd",	{"rdrdmdbw",		0x6f	} },
{    "movd",	{"rdwd",		0x70	} },
{    "cmpd",	{"rdrd",		0x71	} },
{    "mnegd",	{"rdwd",		0x72	} },
{    "tstd",	{"rd",			0x73	} },
{    "emodd",	{"rdrbrdwlwd",		0x74	} },
{    "polyd",	{"rdrwab",		0x75	} },
{    "cvtdf",	{"rdwf",		0x76	} },
                                        /* opcode 77 is not defined yet */
{    "ashl",	{"rbrlwl",		0x78	} },
{    "ashq",	{"rbrqwq",		0x79	} },
{    "emul",	{"rlrlrlwq",		0x7a	} },
{    "ediv",	{"rlrqwlwl",		0x7b	} },
{    "clrd",	{"wd",			0x7c	} },
{    "clrg",	{"wg",			0x7c	} },
{    "clrq",	{"wd",			0x7c	} },
{    "movq",	{"rqwq",		0x7d	} },
{    "movaq",	{"aqwl",		0x7e	} },
{    "movad",	{"adwl",		0x7e	} },
{    "pushaq",	{"aq",			0x7f	} },
{    "pushad",	{"ad",			0x7f	} },
{    "addb2",	{"rbmb",		0x80	} },
{    "addb3",	{"rbrbwb",		0x81	} },
{    "subb2",	{"rbmb",		0x82	} },
{    "subb3",	{"rbrbwb",		0x83	} },
{    "mulb2",	{"rbmb",		0x84	} },
{    "mulb3",	{"rbrbwb",		0x85	} },
{    "divb2",	{"rbmb",		0x86	} },
{    "divb3",	{"rbrbwb",		0x87	} },
{    "bisb2",	{"rbmb",		0x88	} },
{    "bisb3",	{"rbrbwb",		0x89	} },
{    "bicb2",	{"rbmb",		0x8a	} },
{    "bicb3",	{"rbrbwb",		0x8b	} },
{    "xorb2",	{"rbmb",		0x8c	} },
{    "xorb3",	{"rbrbwb",		0x8d	} },
{    "mnegb",	{"rbwb",		0x8e	} },
{    "caseb",	{"rbrbrb",		0x8f	} },
{    "movb",	{"rbwb",		0x90	} },
{    "cmpb",	{"rbrb",		0x91	} },
{    "mcomb",	{"rbwb",		0x92	} },
{    "bitb",	{"rbrb",		0x93	} },
{    "clrb",	{"wb",			0x94	} },
{    "tstb",	{"rb",			0x95	} },
{    "incb",	{"mb",			0x96	} },
{    "decb",	{"mb",			0x97	} },
{    "cvtbl",	{"rbwl",		0x98	} },
{    "cvtbw",	{"rbww",		0x99	} },
{    "movzbl",	{"rbwl",		0x9a	} },
{    "movzbw",	{"rbww",		0x9b	} },
{    "rotl",	{"rbrlwl",		0x9c	} },
{    "acbb",	{"rbrbmbbw",		0x9d	} },
{    "movab",	{"abwl",		0x9e	} },
{    "pushab",	{"ab",			0x9f	} },
{    "addw2",	{"rwmw",		0xa0	} },
{    "addw3",	{"rwrwww",		0xa1	} },
{    "subw2",	{"rwmw",		0xa2	} },
{    "subw3",	{"rwrwww",		0xa3	} },
{    "mulw2",	{"rwmw",		0xa4	} },
{    "mulw3",	{"rwrwww",		0xa5	} },
{    "divw2",	{"rwmw",		0xa6	} },
{    "divw3",	{"rwrwww",		0xa7	} },
{    "bisw2",	{"rwmw",		0xa8	} },
{    "bisw3",	{"rwrwww",		0xa9	} },
{    "bicw2",	{"rwmw",		0xaa	} },
{    "bicw3",	{"rwrwww",		0xab	} },
{    "xorw2",	{"rwmw",		0xac	} },
{    "xorw3",	{"rwrwww",		0xad	} },
{    "mnegw",	{"rwww",		0xae	} },
{    "casew",	{"rwrwrw",		0xaf	} },
{    "movw",	{"rwww",		0xb0	} },
{    "cmpw",	{"rwrw",		0xb1	} },
{    "mcomw",	{"rwww",		0xb2	} },
{    "bitw",	{"rwrw",		0xb3	} },
{    "clrw",	{"ww",			0xb4	} },
{    "tstw",	{"rw",			0xb5	} },
{    "incw",	{"mw",			0xb6	} },
{    "decw",	{"mw",			0xb7	} },
{    "bispsw",	{"rw",			0xb8	} },
{    "bicpsw",	{"rw",			0xb9	} },
{    "popr",	{"rw",			0xba	} },
{    "pushr",	{"rw",			0xbb	} },
{    "chmk",	{"rw",			0xbc	} },
{    "chme",	{"rw",			0xbd	} },
{    "chms",	{"rw",			0xbe	} },
{    "chmu",	{"rw",			0xbf	} },
{    "addl2",	{"rlml",		0xc0	} },
{    "addl3",	{"rlrlwl",		0xc1	} },
{    "subl2",	{"rlml",		0xc2	} },
{    "subl3",	{"rlrlwl",		0xc3	} },
{    "mull2",	{"rlml",		0xc4	} },
{    "mull3",	{"rlrlwl",		0xc5	} },
{    "divl2",	{"rlml",		0xc6	} },
{    "divl3",	{"rlrlwl",		0xc7	} },
{    "bisl2",	{"rlml",		0xc8	} },
{    "bisl3",	{"rlrlwl",		0xc9	} },
{    "bicl2",	{"rlml",		0xca	} },
{    "bicl3",	{"rlrlwl",		0xcb	} },
{    "xorl2",	{"rlml",		0xcc	} },
{    "xorl3",	{"rlrlwl",		0xcd	} },
{    "mnegl",	{"rlwl",		0xce	} },
{    "casel",	{"rlrlrl",		0xcf	} },
{    "movl",	{"rlwl",		0xd0	} },
{    "cmpl",	{"rlrl",		0xd1	} },
{    "mcoml",	{"rlwl",		0xd2	} },
{    "bitl",	{"rlrl",		0xd3	} },
{    "clrf",	{"wf",			0xd4	} },
{    "clrl",	{"wl",			0xd4	} },
{    "tstl",	{"rl",			0xd5	} },
{    "incl",	{"ml",			0xd6	} },
{    "decl",	{"ml",			0xd7	} },
{    "adwc",	{"rlml",		0xd8	} },
{    "sbwc",	{"rlml",		0xd9	} },
{    "mtpr",	{"rlrl",		0xda	} },
{    "mfpr",	{"rlwl",		0xdb	} },
{    "movpsl",	{"wl",			0xdc	} },
{    "pushl",	{"rl",			0xdd	} },
{    "moval",	{"alwl",		0xde	} },
{    "movaf",	{"afwl",		0xde	} },
{    "pushal",	{"al",			0xdf	} },
{    "pushaf",	{"af",			0xdf	} },
{    "bbs",	{"rlvbbb",		0xe0	} },
{    "bbc",	{"rlvbbb",		0xe1	} },
{    "bbss",	{"rlvbbb",		0xe2	} },
{    "bbcs",	{"rlvbbb",		0xe3	} },
{    "bbsc",	{"rlvbbb",		0xe4	} },
{    "bbcc",	{"rlvbbb",		0xe5	} },
{    "bbssi",	{"rlvbbb",		0xe6	} },
{    "bbcci",	{"rlvbbb",		0xe7	} },
{    "blbs",	{"rlbb",		0xe8	} },
{    "blbc",	{"rlbb",		0xe9	} },
{    "ffs",	{"rlrbvbwl",		0xea	} },
{    "ffc",	{"rlrbvbwl",		0xeb	} },
{    "cmpv",	{"rlrbvbrl",		0xec	} },
{    "cmpzv",	{"rlrbvbrl",		0xed	} },
{    "extv",	{"rlrbvbwl",		0xee	} },
{    "extzv",	{"rlrbvbwl",		0xef	} },
{    "insv",	{"rlrlrbvb",		0xf0	} },
{    "acbl",	{"rlrlmlbw",		0xf1	} },
{    "aoblss",	{"rlmlbb",		0xf2	} },
{    "aobleq",	{"rlmlbb",		0xf3	} },
{    "sobgeq",	{"mlbb",		0xf4	} },
{    "sobgtr",	{"mlbb",		0xf5	} },
{    "cvtlb",	{"rlwb",		0xf6	} },
{    "cvtlw",	{"rlww",		0xf7	} },
{    "ashp",	{"rbrwabrbrwab",	0xf8	} },
{    "cvtlp",	{"rlrwab",		0xf9	} },
{    "callg",	{"abab",		0xfa	} },
{    "calls",	{"rlab",		0xfb	} },
{    "xfc",	{"",			0xfc	} },
                                        /* undefined opcodes here */
{    "cvtdh",	{"rdwh",		0x32fd	} },
{    "cvtgf",	{"rgwh",		0x33fd	} },
{    "addg2",	{"rgmg",		0x40fd	} },
{    "addg3",	{"rgrgwg",		0x41fd	} },
{    "subg2",	{"rgmg",		0x42fd	} },
{    "subg3",	{"rgrgwg",		0x43fd	} },
{    "mulg2",	{"rgmg",		0x44fd	} },
{    "mulg3",	{"rgrgwg",		0x45fd	} },
{    "divg2",	{"rgmg",		0x46fd	} },
{    "divg3",	{"rgrgwg",		0x47fd	} },
{    "cvtgb",	{"rgwb",		0x48fd	} },
{    "cvtgw",	{"rgww",		0x49fd	} },
{    "cvtgl",	{"rgwl",		0x4afd	} },
{    "cvtrgl",	{"rgwl",		0x4bfd	} },
{    "cvtbg",	{"rbwg",		0x4cfd	} },
{    "cvtwg",	{"rwwg",		0x4dfd	} },
{    "cvtlg",	{"rlwg",		0x4efd	} },
{    "acbg",	{"rgrgmgbw",		0x4ffd	} },
{    "movg",	{"rgwg",		0x50fd	} },
{    "cmpg",	{"rgrg",		0x51fd	} },
{    "mnegg",	{"rgwg",		0x52fd	} },
{    "tstg",	{"rg",			0x53fd	} },
{    "emodg",	{"rgrwrgwlwg",		0x54fd	} },
{    "polyg",	{"rgrwab",		0x55fd	} },
{    "cvtgh",	{"rgwh",		0x56fd	} },
                                        /* undefined opcodes here */
{    "addh2",	{"rhmh",		0x60fd	} },
{    "addh3",	{"rhrhwh",		0x61fd	} },
{    "subh2",	{"rhmh",		0x62fd	} },
{    "subh3",	{"rhrhwh",		0x63fd	} },
{    "mulh2",	{"rhmh",		0x64fd	} },
{    "mulh3",	{"rhrhwh",		0x65fd	} },
{    "divh2",	{"rhmh",		0x66fd	} },
{    "divh3",	{"rhrhwh",		0x67fd	} },
{    "cvthb",	{"rhwb",		0x68fd	} },
{    "cvthw",	{"rhww",		0x69fd	} },
{    "cvthl",	{"rhwl",		0x6afd	} },
{    "cvtrhl",	{"rhwl",		0x6bfd	} },
{    "cvtbh",	{"rbwh",		0x6cfd	} },
{    "cvtwh",	{"rwwh",		0x6dfd	} },
{    "cvtlh",	{"rlwh",		0x6efd	} },
{    "acbh",	{"rhrhmhbw",		0x6ffd	} },
{    "movh",	{"rhwh",		0x70fd	} },
{    "cmph",	{"rhrh",		0x71fd	} },
{    "mnegh",	{"rhwh",		0x72fd	} },
{    "tsth",	{"rh",			0x73fd	} },
{    "emodh",	{"rhrwrhwlwh",		0x74fd	} },
{    "polyh",	{"rhrwab",		0x75fd	} },
{    "cvthg",	{"rhwg",		0x76fd	} },
                                        /* undefined opcodes here */
{    "clrh",	{"wh",			0x7cfd	} },
{    "clro",	{"wo",			0x7cfd	} },
{    "movo",	{"rowo",		0x7dfd	} },
{    "movah",	{"ahwl",		0x7efd	} },
{    "movao",	{"aowl",		0x7efd	} },
{    "pushah",	{"ah",			0x7ffd	} },
{    "pushao",	{"ao",			0x7ffd	} },
                                        /* undefined opcodes here */
{    "cvtfh",	{"rfwh",		0x98fd	} },
{    "cvtfg",	{"rfwg",		0x99fd	} },
                                        /* undefined opcodes here */
{    "cvthf",	{"rhwf",		0xf6fd	} },
{    "cvthd",	{"rhwd",		0xf7fd	} },
                                        /* undefined opcodes here */
{    "bugl",	{"rl",			0xfdff	} },
{    "bugw",	{"rw",			0xfeff	} },
                                        /* undefined opcodes here */

{      "",	{"", 0} }		/* empty is end sentinel */

};				/* votstrs */

/* end: vax.opcode.h */
