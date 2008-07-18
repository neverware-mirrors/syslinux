## -----------------------------------------------------------------------
##   
##   Copyright 2008 H. Peter Anvin - All Rights Reserved
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
##   Boston MA 02110-1301, USA; either version 2 of the License, or
##   (at your option) any later version; incorporated herein by reference.
##
## -----------------------------------------------------------------------

#
# Makefile for gpxe/gpxelinux.0
#
# Very simple, really...
#

PERL    = perl
MKISOFS = mkisofs

TARGETS	= gpxelinux.0 gpxeboot.iso

all: $(TARGETS)

tidy:
	rm -rf gpxeboot

clean: tidy

dist:
	$(MAKE) -C src veryclean > /dev/null 2>&1
	rm -f gpxeboot.iso

spotless: clean dist
	rm -f $(TARGETS)

installer:

src/bin/undionly.kpxe: ../core/pxelinux.0
	$(MAKE) -C src EMBEDDED_IMAGE=../$< bin/undionly.kpxe

gpxelinux.0: src/bin/undionly.kpxe
	cp -f $< $@

src/bin/NIC:
	$(MAKE) -C src bin/NIC

allnics: src/bin/NIC
	$(MAKE) -C src `$(PERL) -ne 'chomp; \
	if (m:^family\s.*/([^/\s]+)\s*$$:) { print "bin/$$1.lkrn\n"; }' $<`

gpxeboot.iso: allnics nictoesel.pl
	rm -rf gpxeboot
	mkdir -p gpxeboot
	cp -f ../core/isolinux.bin gpxeboot/
	$(PERL) nictoesel.pl src/bin/NIC > gpxeboot/isolinux.cfg
	cp -f ../com32/modules/ethersel.c32 gpxeboot/
	cp -f src/bin/*.lkrn gpxeboot/
	$(MKISOFS) -q -l -r -J -o $@ \
		-b isolinux.bin -c boot.cat \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		gpxeboot
