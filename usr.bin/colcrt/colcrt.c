/*	$OpenBSD: colcrt.c,v 1.9 2008/07/06 13:36:09 sobrado Exp $	*/
/*	$NetBSD: colcrt.c,v 1.3 1995/03/26 05:31:00 glass Exp $	*/

/*
 * Copyright (c) 1980, 1993
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
 * 3. Neither the name of the University nor the names of its contributors
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
 */

#ifndef lint
static char copyright[] =
"@(#) Copyright (c) 1980, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)colcrt.c	8.1 (Berkeley) 6/6/93";
#else
static char rcsid[] = "$OpenBSD: colcrt.c,v 1.9 2008/07/06 13:36:09 sobrado Exp $";
#endif
#endif /* not lint */

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <err.h>

/*
 * colcrt - replaces col for crts with new nroff esp. when using tbl.
 * Bill Joy UCB July 14, 1977
 *
 * This filter uses a screen buffer, 267 half-lines by 132 columns.
 * It interprets the up and down sequences generated by the new
 * nroff when used with tbl and by \u \d and \r.
 * General overstriking doesn't work correctly.
 * Underlining is split onto multiple lines, etc.
 *
 * Option - suppresses all underlining.
 * Option -2 forces printing of all half lines.
 */

char	page[267][132];

int	outline = 1;
int	outcol;

char	suppresul;
char	printall;

FILE	*f;

void	pflush(int);
int	plus(char, char);
void	move(int, int);

int
main(int argc, char *argv[])
{
	extern char *__progname;
	char *cp, *dp;
	int c;

	argc--;
	argv++;
	while (argc > 0 && argv[0][0] == '-') {
		switch (argv[0][1]) {
			case 0:
				suppresul = 1;
				break;
			case '2':
				printall = 1;
				break;
			default:
				fprintf(stderr,
				    "usage: %s [-] [-2] [file ...]\n",
				    __progname);
				fflush(stdout);
				exit(1);
		}
		argc--;
		argv++;
	}
	do {
		if (argc > 0) {
			close(0);
			if (!(f = fopen(argv[0], "r"))) {
				fflush(stdout);
				err(1, "fopen: %s", argv[0]);
			}
			argc--;
			argv++;
		}
		for (;;) {
			c = getc(stdin);
			if (c == -1) {
				pflush(outline);
				fflush(stdout);
				break;
			}
			switch (c) {
				case '\n':
					if (outline >= 265)
						pflush(62);
					outline += 2;
					outcol = 0;
					continue;
				case '\016':
					case '\017':
					continue;
				case 033:
					c = getc(stdin);
					switch (c) {
						case '9':
							if (outline >= 266)
								pflush(62);
							outline++;
							continue;
						case '8':
							if (outline >= 1)
								outline--;
							continue;
						case '7':
							outline -= 2;
							if (outline < 0)
								outline = 0;
							continue;
						default:
							continue;
					}
				case '\b':
					if (outcol)
						outcol--;
					continue;
				case '\t':
					outcol += 8;
					outcol &= ~7;
					outcol--;
					c = ' ';
				default:
					if (outcol >= 132) {
						outcol++;
						continue;
					}
					cp = &page[outline][outcol];
					outcol++;
					if (c == '_') {
						if (suppresul)
							continue;
						cp += 132;
						c = '-';
					}
					if (*cp == 0) {
						*cp = c;
						dp = cp - outcol;
						for (cp--; cp >= dp && *cp == 0; cp--)
							*cp = ' ';
					} else
						if (plus(c, *cp) || plus(*cp, c))
							*cp = '+';
						else if (*cp == ' ' || *cp == 0)
							*cp = c;
					continue;
			}
		}
	} while (argc > 0);
	fflush(stdout);
	exit(0);
}

int
plus(char c, char d)
{

	return ((c == '|' && d == '-') || d == '_');
}

int first;

void
pflush(int ol)
{
	int i;
	char *cp;
	char lastomit;
	int l;

	l = ol;
	lastomit = 0;
	if (l > 266)
		l = 266;
	else
		l |= 1;
	for (i = first | 1; i < l; i++) {
		move(i, i - 1);
		move(i, i + 1);
	}
	for (i = first; i < l; i++) {
		cp = page[i];
		if (printall == 0 && lastomit == 0 && *cp == 0) {
			lastomit = 1;
			continue;
		}
		lastomit = 0;
		printf("%s\n", cp);
	}
	bcopy(page[ol], page, (267 - ol) * 132);
	bzero(page[267- ol], ol * 132);
	outline -= ol;
	outcol = 0;
	first = 1;
}

void
move(int l, int m)
{
	char *cp, *dp;

	for (cp = page[l], dp = page[m]; *cp; cp++, dp++) {
		switch (*cp) {
			case '|':
				if (*dp != ' ' && *dp != '|' && *dp != 0)
					return;
				break;
			case ' ':
				break;
			default:
				return;
		}
	}
	if (*cp == 0) {
		for (cp = page[l], dp = page[m]; *cp; cp++, dp++)
			if (*cp == '|')
				*dp = '|';
			else if (*dp == 0)
				*dp = ' ';
		page[l][0] = 0;
	}
}
