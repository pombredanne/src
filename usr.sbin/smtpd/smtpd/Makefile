#	$OpenBSD: Makefile,v 1.20 2010/06/01 23:06:25 jacekm Exp $

PROG=		smtpd
SRCS=		aliases.c authenticate.c client.c		\
		config.c control.c dns.c expand.c forward.c		\
		lka.c log.c map.c map_backend.c map_parser.c mda.c	\
		mfa.c mta.c parse.y queue.c queue_backend.c ruleset.c	\
		smtp.c smtp_session.c smtpd.c ssl.c ssl_privsep.c util.c

MAN=		smtpd.8 smtpd.conf.5
BINDIR=		/usr/sbin

LDADD+=		-levent -lutil -lssl -lcrypto -lm
DPADD+=		${LIBEVENT} ${LIBUTIL} ${LIBSSL} ${LIBCRYPTO} ${LIBM}
CFLAGS+=	-g3 -ggdb -I${.CURDIR}/..
CFLAGS+=	-Wall -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=	-Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=	-Wsign-compare -Wbounded
#CFLAGS+=	-Werror # during development phase (breaks some archs)
YFLAGS=

.PATH: ${.CURDIR}/..

.include <bsd.prog.mk>