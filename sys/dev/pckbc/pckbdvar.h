/* $OpenBSD: pckbdvar.h,v 1.1.8.1 2002/06/11 03:42:27 art Exp $ */
/* $NetBSD: pckbdvar.h,v 1.3 2000/03/10 06:10:35 thorpej Exp $ */

int	pckbd_cnattach(pckbc_tag_t, int);
void	pckbd_hookup_bell(void (*fn)(void *, u_int, u_int, u_int, int),
	    void *);
