/*	$OpenBSD: instat.c,v 1.1.8.1 2001/10/31 03:01:20 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

/* returns 0 if no characters ready to read */
int
mvmeprom_instat()
{
	short ret;

	MVMEPROM_CALL(MVMEPROM_INSTAT);
	asm volatile ("or %0,r0,r2" :  "=r" (ret));
	return (!(ret & 0x4));
}
