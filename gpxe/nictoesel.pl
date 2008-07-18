#!/usr/bin/perl
#
# Script to convert src/bin/NIC to Ethersel format
#

print "default ethersel.c32\n";

undef $nicname;
%nic = ();

while (<>) {
    if (m:^family\s.*/([^/\s]+)\s*$:) {
	$nicname = $1;
    } elsif (m:^(\S+)\s+([0-9a-f]+)\,([0-9a-f]+)\s+:) {
	$vid = hex $2;
	$did = hex $3;
	$dev = sprintf("%04x:%04x", $vid, $did);
	unless (($vid == 0 || $vid == 0xffff) && $did == $vid) {
	    if (defined($nicname) && !defined($nic{$dev})) {
		print "# DEV DID $dev $nicname.lkrn\n";
		$nic{$dev} = $nicname;
	    }
	}
    }
}

