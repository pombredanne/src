/*	$OpenBSD: delay.c,v 1.1.2.1 2001/10/31 03:01:21 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

/* BUG - timing routine */
void
mvmeprom_delay(msec)
	int msec; /* This is r3 */
{
	asm volatile ("mr 3, %0" :: "r"(msec));
	MVMEPROM_CALL(MVMEPROM_DELAY);
}
