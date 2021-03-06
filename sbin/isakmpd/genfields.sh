#	$OpenBSD: genfields.sh,v 1.9 2004/04/15 18:39:25 deraadt Exp $
#	$EOM: genfields.sh,v 1.5 1999/04/02 01:15:55 niklas Exp $

#
# Copyright (c) 1998, 1999, 2001 Niklas Hallqvist.  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
# This code was written under funding by Ericsson Radio Systems.
#

base=`basename $1`
upcased_name=`echo $base |tr a-z A-Z`

awk=${AWK:-awk}

locase_function='function locase (str) {
  cmd = "echo " str " |tr A-Z a-z"
  cmd | getline retval;
  close (cmd);
  return retval;
}'

$awk "
$locase_function
"'
BEGIN {
  print "/* DO NOT EDIT-- this file is automatically generated.  */\n"
  print "#ifndef _'$upcased_name'_H_"
  print "#define _'$upcased_name'_H_\n"

  print "#include \"sysdep.h\"\n"
  print "#include \"field.h\"\n"

  print "struct constant_map;\n"
}

/^#/ {
  next
}

/^\./ {
  printf ("#define %s_SZ %d\n", prefix, off)
  size[prefix] = off
  next
}

/^[^ 	]/ {
  prefix = $1
  printf ("extern struct field %s_fld[];\n\n", locase(prefix));
  if ($3)
    {
      off = size[$3]
    }
  else
    {
      off = 0
    }
  i = 0
  next
}

/^[ 	]/ && $1 {
  printf ("#define %s_%s_OFF %d\n", prefix, $1, off)
  if ($3)
    {
      printf ("#define %s_%s_LEN %d\n", prefix, $1, $3)
    }
  if ($4)
    {
      printf ("extern struct constant_map *%s_%s_maps[];\n", locase(prefix),
              locase($1))
    }
  if ($2 == "raw")
    {
      printf ("#define GET_%s_%s(buf, val) ", prefix, $1)
      printf ("field_get_raw (%s_fld + %d, buf, val)\n", locase(prefix), i)
      printf ("#define SET_%s_%s(buf, val) ", prefix, $1)
      printf ("field_set_raw (%s_fld + %d, buf, val)\n", locase(prefix), i)
    }
  else
    {
      printf ("#define GET_%s_%s(buf) field_get_num (%s_fld + %d, buf)\n",
              prefix, $1, locase(prefix), i)
      printf ("#define SET_%s_%s(buf, val) ", prefix, $1)
      printf ("field_set_num (%s_fld + %d, buf, val)\n", locase(prefix), i)
    }
  off += $3
  i++
  next
}

{
    print
}

END {
  printf ("\n")
  print "#endif /* _'$upcased_name'_H_ */"
}
' <$1.fld >$base.h

$awk "
$locase_function
"'
BEGIN {
  print "/* DO NOT EDIT-- this file is automatically generated.  */\n"
  print "#include \"sysdep.h\"\n"
  print "#include \"constants.h\""
  print "#include \"field.h\""
  print "#include \"'$base'.h\""
  print "#include \"isakmp_num.h\""
  print "#include \"ipsec_num.h\"\n"
}

/^#/ {
  next
}

/^\./ {
  print "	{ 0, 0, 0, 0, 0 }\n};\n"
  size[prefix] = off
  for (map in maps)
    {
      printf ("struct constant_map *%s_%s_maps[] = {\n", locase(prefix),
             locase(map))
      printf ("	%s, 0\n};\n", maps[map])
    }
  next
}

/^[^ 	]/ {
  prefix = $1
  printf ("struct field %s_fld[] = {\n", locase(prefix))
  if ($3)
    {
      off = size[$3]
    }
  else
    {
      off = 0
    }
  delete maps
  next
}

/^[ 	]/ && $1 {
  if ($4)
    {
      maps_name = locase(prefix)"_"locase($1)"_maps"
      maps[$1] = $4
    }
  else
    {
      maps_name = "0"
    }
  printf ("	{ \"%s\", %d, %d, %s, %s },\n", $1, off, $3, $2, maps_name)
  off += $3
  next
}

{
  print
}
' <$1.fld >$base.c
