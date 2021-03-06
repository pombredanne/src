/*	$OpenBSD: s_ilogb.S,v 1.5 2005/08/02 11:17:31 espie Exp $ */
/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

#include <machine/asm.h>

ENTRY(ilogb)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$4,%esp

	fldl	8(%ebp)
	fxtract
	fstp	%st

	fistpl	-4(%ebp)
	movl	-4(%ebp),%eax

	leave
	ret
