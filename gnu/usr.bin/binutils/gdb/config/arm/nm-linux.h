/* Definitions to make GDB run on an ARM based machine under GNU/Linux.
   Copyright 1999, 2000, 2001 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef NM_ARMLINUX_H
#define NM_ARMLINUX_H

#include "config/nm-linux.h"

/* ptrace register ``addresses'' are absolute.  */

#define U_REGS_OFFSET 0

/* Return sizeof user struct to callers in less machine dependent routines */
extern int kernel_u_size (void);
#define KERNEL_U_SIZE	arm_linux_kernel_u_size()

/* This is the amount to subtract from u.u_ar0
   to get the offset in the core file of the register values.  */
#define KERNEL_U_ADDR 0x0

/* Override copies of {fetch,store}_inferior_registers in infptrace.c.  */
#define FETCH_INFERIOR_REGISTERS

#endif /* NM_ARMLINUX_H */
