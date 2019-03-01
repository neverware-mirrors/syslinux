## -----------------------------------------------------------------------
##
##   Copyright 2011 Intel Corporation; author: Matt Fleming
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
##   Boston MA 02111-1307, USA; either version 2 of the License, or
##   (at your option) any later version; incorporated herein by reference.
##
## -----------------------------------------------------------------------

include $(MAKEDIR)/elf.mk

# The targets to build in this directory
all: libefi

EFISRC  = $(SRC)/gnu-efi
EFIOBJ  = $(OBJ)/gnu-efi

# This is a hack due to missing rules in gnu-efi/Make.rules
# Also note make rather than $(MAKE) below.
unexport MAKEFLAGS

EFIMAKE = make -C '$(EFIOBJ)' \
	  SRCDIR='$(EFISRC)' \
	  HOSTCC='$(CC_FOR_BUILD)' \
	  ARCH='$(EFI_SUBARCH)' PREFIX='$(OBJ)' \
	  -f '$(EFISRC)/Makefile'

libefi: include/efi/$(EFI_SUBARCH)/efibind.h \
	lib/libefi.a lib/libgnuefi.a

include/%/$(EFI_SUBARCH)/efibind.h lib/lib%.a lib/libgnu%.a: 
	@echo Building gnu-efi for $(EFI_SUBARCH)
	if [ -f $(topdir)/.gitmodule ]; then \
		( cd $(topdir) && git submodule update --init ); \
	fi
	mkdir -p $(EFIOBJ)
	$(EFIMAKE)
	$(EFIMAKE) install

install:

strip:

tidy dist:
	rm -f *.so *.o wrapper
	find . \( -name \*.o -o -name \*.a -o -name .\*.d -o -name \*.tmp \) -print0 | \
		xargs -0r rm -f
	if [ -d gnu-efi ]; then $(EFIMAKE) clean; fi

clean: clean-efi tidy

clean-efi:
	rm -rf gnu-efi

spotless: clean
	rm -rf lib include
