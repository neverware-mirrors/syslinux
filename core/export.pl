#!/usr/bin/perl
#
# Script to generate an export library .a file and list of symbols to
# be weakened from a set of executables or shared libraries (with a
# dynamic symbol section.)
#

use strict;

sub getint($) {
    my($s) = @_;

    return ($s =~ /^0/) ? oct $s : $s+0;
}

my ($outfile, $weakfile, @infiles) = @ARGV;

my %nsyms;			# Number of files in which this symbol occurs
my %syms;			# Symbol information
my %fsyms;			# Files in which this symbol occurs

my $readelf = $ENV{'READELF'} || 'readelf';

foreach my $infile (@infiles) {
    open(my $in, '-|', $readelf, '--dyn-syms', $infile)
	or die "$0: $infile: $!\n";

    while (my $line = <$in>) {
	$line =~ s/^\s+//;
	$line =~ s/\s+$//;

	next unless ($line =~ /^[0-9]+\:/);
	
	my ($n,$addr,$size,$type,$link,$vis,$sec,$name) = split(/\s+/, $line);

	next if ($name eq '');	# Null symbol

	$addr = hex $addr;
	$size = getint($size);
	if ($nsyms{$name}++ == 0) {
	    $fsyms{$name} = [$infile];
	} else {
	    push(@{$fsyms{$name}}, $infile);
	}
	$syms{$name} = [$addr,$size,$type,$link,$vis];
    }

    close($in);
}
	
my $nfiles = scalar(@infiles);

open(my $out, '>', $outfile)
    or die "$0: $outfile: $!\n";
open(my $weak, '>', $weakfile)
    or die "$0: $weakfile: $!\n";

foreach my $sym (sort(keys(%nsyms))) {
    if ($nsyms{$sym} == $nfiles) {
	# It is an exportable symbol common to all inputs
	
	my($addr,$size,$type,$link,$vis) = @{$syms{$sym}};
	printf $out "\t.globl\t%s\n", $sym;
	printf $out "%s\t= 0x%08x\n", $sym, $addr;
	printf $out "\t.type\t%s, STT_%s\n", $sym, $type;
	printf $out "\t.size\t%s, %u\n", $sym, $size;
	if ($vis ne 'DEFAULT') {
	    printf $out "\t.%s\t%s\n", "\L$vis", $sym;
	}
    } else {
	# It is a symbol present in at least one input, should
	# be made weak in the dynamic library if present

	print $weak '# ', join(' ', @{$fsyms{$sym}}), "\n";
	print $weak $sym, "\n";
    }
}

close($out);
close($weak);

exit 0;

