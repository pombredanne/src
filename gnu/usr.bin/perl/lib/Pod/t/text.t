#!/usr/bin/perl -w
#
# text.t -- Additional specialized tests for Pod::Text.
#
# Copyright 2002, 2004, 2006, 2007, 2008, 2009 Russ Allbery <rra@stanford.edu>
#
# This program is free software; you may redistribute it and/or modify it
# under the same terms as Perl itself.

BEGIN {
    chdir 't' if -d 't';
    if ($ENV{PERL_CORE}) {
        @INC = '../lib';
    } else {
        unshift (@INC, '../blib/lib');
    }
    unshift (@INC, '../blib/lib');
    $| = 1;
    print "1..6\n";
}

END {
    print "not ok 1\n" unless $loaded;
}

use Pod::Text;
use Pod::Simple;

$loaded = 1;
print "ok 1\n";

my $parser = Pod::Text->new or die "Cannot create parser\n";
my $n = 2;
while (<DATA>) {
    next until $_ eq "###\n";
    open (TMP, '> tmp.pod') or die "Cannot create tmp.pod: $!\n";
    while (<DATA>) {
        last if $_ eq "###\n";
        print TMP $_;
    }
    close TMP;
    open (OUT, '> out.tmp') or die "Cannot create out.tmp: $!\n";
    $parser->parse_from_file ('tmp.pod', \*OUT);
    close OUT;
    open (TMP, 'out.tmp') or die "Cannot open out.tmp: $!\n";
    my $output;
    {
        local $/;
        $output = <TMP>;
    }
    close TMP;
    unlink ('tmp.pod', 'out.tmp');
    my $expected = '';
    while (<DATA>) {
        last if $_ eq "###\n";
        $expected .= $_;
    }
    if ($output eq $expected) {
        print "ok $n\n";
    } elsif ($n == 4 && $Pod::Simple::VERSION < 3.06) {
        print "ok $n # skip Pod::Simple S<> parsing bug\n";
    } else {
        print "not ok $n\n";
        print "Expected\n========\n$expected\nOutput\n======\n$output\n";
    }
    $n++;
}

# Below the marker are bits of POD and corresponding expected text output.
# This is used to test specific features or problems with Pod::Text.  The
# input and output are separated by lines containing only ###.

__DATA__

###
=head1 PERIODS

This C<.> should be quoted.
###
PERIODS
    This "." should be quoted.

###

###
=head1 CE<lt>E<gt> WITH SPACES

What does C<<  this.  >> end up looking like?
###
C<> WITH SPACES
    What does "this." end up looking like?

###

###
=head1 Test of SE<lt>E<gt>

This is some S<  > whitespace.
###
Test of S<>
    This is some    whitespace.

###

###
=head1 Test of =for

=for comment
This won't be seen.

Yes.

=for text
This should be seen.

=for TEXT As should this.

=for man
But this shouldn't.

Some more text.
###
Test of =for
    Yes.

This should be seen.
As should this.
    Some more text.

###

###
=pod

text

  line1
  
  line3
###
    text

      line1
  
      line3

###
