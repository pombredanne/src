#!/usr/bin/perl -w

require 5.003;

sub readsyms (\%$) {
    my ($syms, $file) = @_;
    %$syms = ();
    local (*FILE, $_);
    open(FILE, "< $file")
	or die "embed.pl: Can't open $file: $!\n";
    while (<FILE>) {
	s/[ \t]*#.*//;		# Delete comments.
	if (/^\s*(\S+)\s*$/) {
	    $$syms{$1} = 1;
	}
    }
    close(FILE);
}

readsyms %global, 'global.sym';
readsyms %interp, 'interp.sym';
readsyms %compat3, 'compat3.sym';

sub hide ($$) {
    my ($from, $to) = @_;
    my $t = int(length($from) / 8);
    "#define $from" . "\t" x ($t < 3 ? 3 - $t : 1) . "$to\n";
}
sub embed ($) {
    my ($sym) = @_;
    hide($sym, "Perl_$sym");
}
sub multon ($) {
    my ($sym) = @_;
    hide($sym, "(curinterp->I$sym)");
}
sub multoff ($) {
    my ($sym) = @_;
    hide("I$sym", $sym);
}

unlink 'embed.h';
open(EM, '> embed.h')
    or die "Can't create embed.h: $!\n";

print EM <<'END';
/* !!!!!!!   DO NOT EDIT THIS FILE   !!!!!!! 
   This file is built by embed.pl from global.sym, interp.sym,
   and compat3.sym.  Any changes made here will be lost!
*/

/* (Doing namespace management portably in C is really gross.) */

/*  EMBED has no run-time penalty, but helps keep the Perl namespace
    from colliding with that used by other libraries pulled in
    by extensions or by embedding perl.  Allow a cc -DNO_EMBED
    override, however, to keep binary compatability with previous
    versions of perl.
*/
#ifndef NO_EMBED
#  define EMBED 1 
#endif

/* Hide global symbols? */

#ifdef EMBED

END

for $sym (sort keys %global) {
    print EM embed($sym) unless $compat3{$sym};
}

print EM <<'END';

/* Hide global symbols that 5.003 revealed? */

#ifndef BINCOMPAT3

END

for $sym (sort keys %global) {
    print EM embed($sym) if $compat3{$sym};
}

print EM <<'END';

#endif /* !BINCOMPAT3 */

#endif /* EMBED */

/* Put interpreter-specific symbols into a struct? */

#ifdef MULTIPLICITY

END

for $sym (sort keys %interp) {
    print EM multon($sym);
}

print EM <<'END';

#else	/* !MULTIPLICITY */

END

for $sym (sort keys %interp) {
    print EM multoff($sym);
}

print EM <<'END';

/* Hide interpreter-specific symbols? */

#ifdef EMBED

END

for $sym (sort keys %interp) {
    print EM embed($sym) if $compat3{$sym};
}

print EM <<'END';

/* Hide interpreter symbols that 5.003 revealed? */

#ifndef BINCOMPAT3

END

for $sym (sort keys %interp) {
    print EM embed($sym) unless $compat3{$sym};
}

print EM <<'END';

#endif /* !BINCOMPAT3 */

#endif /* EMBED */

#endif /* MULTIPLICITY */
END

