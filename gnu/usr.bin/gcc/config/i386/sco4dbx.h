/* Definitions for Intel 386 running SCO Unix System V 3.2 Version 4.s,
   using dbx-in-coff encapsulation.
   Copyright (C) 1992 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* Mostly it's like earlier SCO UNIX.  */

#include "i386/scodbx.h"

/* Use crt1.o as a startup file and crtn.o as a closing file.  */

#undef STARTFILE_SPEC
#define STARTFILE_SPEC \
 "%{!r:%{!z:gcc.ifile%s}%{z:gccz.ifile%s}}\
  %{scoxpg3:%{p:mcrt1X.o%s}%{!p:crt1X.o%s}} \
  %{!scoxpg3:\
   %{posix:%{p:mcrt1P.o%s}%{!p:crt1P.o%s}} \
   %{!posix:\
    %{ansi:%{p:mcrt1A.o%s}%{!p:crt1A.o%s}} \
    %{!ansi:%{pg:gcrt1.o%s}%{!pg:%{p:mcrt1.o%s}%{!p:crt1.o%s}}}}}"

#undef ENDFILE_SPEC
#define ENDFILE_SPEC \
 "%{scoxpg3:crtnX.o%s} \
  %{!scoxpg3:\
   %{posix:crtnP.o%s} \
   %{!posix:\
    %{ansi:crtnA.o%s} \
    %{!ansi:crtn.o%s}}}"

/* Library spec.  */

#undef LIB_SPEC
#define LIB_SPEC \
 "%{p:-L/usr/lib/libp}%{pg:-L/usr/lib/libp} \
 %{scoxpg3:-lcX -lcP -lcA} \
 %{!scoxpg3:\
  %{posix:-lcP -lcA} \
  %{!posix:\
   %{ansi:-lcA} \
   %{!ansi:%{scointl:-lintl} -lc}}}"

/* Macros, macros everywhere:
   Specify predefined symbols in preprocessor.  */

#undef CPP_PREDEFINES
#define CPP_PREDEFINES "-Dunix -Asystem(svr3)"

#undef CPP_SPEC
#define CPP_SPEC "%(cpp_cpu) %[cpp_cpu] \
  -D_M_I386 -D_M_I86 -D_M_I86SM -D_M_SDATA -D_M_STEXT \
  -D_M_UNIX -D_M_XENIX \
  -D_M_SYS5 -D_M_SYSV -D_M_SYS3 -D_M_SYSIII \
  -D_M_COFF -D_M_BITFIELDS -D_M_WORDSWAP \
  %{scoxpg3:-D_XOPEN_SOURCE -D_STRICT_NAMES} \
  %{!scoxpg3:%{posix:-D_POSIX_SOURCE -D_STRICT_NAMES}} \
  %{!scoxpg3:%{!posix:\
    %{ansi:-D_STRICT_NAMES}%{!ansi:\
    -DM_I386 -DM_I86 -DM_I86SM -DM_SDATA -DM_STEXT \
    -DM_UNIX -DM_XENIX \
    -DM_SYS5 -DM_SYSV -DM_SYS3 -DM_SYSIII \
    -DM_COFF -DM_BITFIELDS -DM_WORDSWAP \
    %{scointl:-D_M_INTERNAT -DM_INTERNAT} \
    %{traditional:-D_KR -D_SVID -D_NO_PROTOTYPE}}}}"
