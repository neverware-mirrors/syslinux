## -----------------------------------------------------------------------
##
##   Copyright 1998-2009 H. Peter Anvin - All Rights Reserved
##   Copyright 2009-2016 Intel Corporation; author: H. Peter Anvin
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
##   Boston MA 02111-1307, USA; either version 2 of the License, or
##   (at your option) any later version; incorporated herein by reference.
##
## -----------------------------------------------------------------------

#
# Makefile for the SYSLINUX core
#

include $(MAKEDIR)/elf.mk
-include $(objdir)/version.mk

INCLUDE  += -I$(SRC)/include -I$(com32)/lib \
	    -I$(SRC)/bios/lwip/src/include \
	    -I$(SRC)/bios/lwip/src/include/ipv4 -I$(SRC)/fs/pxe

# This is very similar to cp437; technically it's for Norway and Denmark,
# but it's unlikely the characters that are different will be used in
# filenames by other users.
CODEPAGE = cp865

# The targets to build in this directory...
BTARGET  = ldlinux.bss ldlinux.sys \
	   isolinux.bin isolinux-debug.bin pxelinux.0 lpxelinux.0

# The corresponding ELF files
ETARGET  = $(patsubst $(SRC)/%-c.c,%.elf,$(wildcard $(SRC)/bios/*-c.c))

# Export library files
LIBCORE_FILES = libcore.so libcore.weak

# All primary source files for the main syslinux files
NASMSRC	 := $(sort $(shell find $(SRC) -name '*.asm' -print))
NASMHDR	 := $(sort $(shell find $(SRC) -name '*.inc' -print))
CSRC	 := $(sort $(shell find $(SRC) -name '*.c' -print))
SSRC	 := $(sort $(shell find $(SRC) -name '*.S' -print))
CHDR	 := $(sort $(shell find $(SRC) -name '*.h' -print))
OTHERSRC :=
ALLSRC    = $(NASMSRC) $(NASMHDR) $(CSRC) $(SSRC) $(CHDR) $(OTHERSRC)

COBJ	 := $(subst $(SRC)/,,$(patsubst %.c,%.o,$(CSRC)))
SOBJ	 := $(subst $(SRC)/,,$(patsubst %.S,%.o,$(SSRC)))

# To make this compatible with the following $(filter-out), make sure
# we prefix everything with $(SRC)
CORE_PXE_DIRS  = $(SRC)/bios/pxe
LPXELINUX_DIRS = $(SRC)/bios/lwip
PXELINUX_DIRS  = $(SRC)/bios/legacynet

CORE_PXE_CSRC  = $(sort $(shell find $(CORE_PXE_DIRS)  -name '*.c' -print))
CORE_PXE_SSRC  = $(sort $(shell find $(CORE_PXE_DIRS)  -name '*.S' -print))
LPXELINUX_CSRC = $(sort $(shell find $(LPXELINUX_DIRS) -name '*.c' -print))
LPXELINUX_SSRC = $(sort $(shell find $(LPXELINUX_DIRS) -name '*.S' -print))
PXELINUX_CSRC  = $(sort $(shell find $(PXELINUX_DIRS)  -name '*.c' -print))
PXELINUX_SSRC  = $(sort $(shell find $(PXELINUX_DIRS)  -name '*.S' -print))

CORE_PXE_OBJS  = $(subst $(SRC)/,,$(CORE_PXE_CSRC:%.c=%.o)  $(CORE_PXE_SSRC:%.S=%.o))
LPXELINUX_OBJS = $(subst $(SRC)/,,$(LPXELINUX_CSRC:%.c=%.o) $(LPXELINUX_SSRC:%.S=%.o))
PXELINUX_OBJS  = $(subst $(SRC)/,,$(PXELINUX_CSRC:%.c=%.o)  $(PXELINUX_SSRC:%.S=%.o))

UNITTEST_DIRS = $(sort $(shell find $(SRC) -type d -name tests))

# Don't include network stack specific objects or unit tests
FILTER_DIRS = $(UNITTEST_DIRS) $(CORE_PXE_DIRS) \
	      $(PXELINUX_DIRS) $(LPXELINUX_DIRS)
ifeq ($(FWCLASS),EFI)
FILTER_DIRS += $(SRC)/bios
endif
FILTER_OBJS = $(patsubst $(SRC)/%,%/%,$(FILTER_DIRS))

COBJS	 = $(filter-out $(FILTER_OBJS),$(COBJ))
SOBJS	 = $(filter-out $(FILTER_OBJS),$(SOBJ))

CORE_LIB	= bios/libsyslinux.a
CORE_LIBS	= $(CORE_LIB) $(objdir)/com32/lib/libcom32.a
CORE_LIBOBJS	 = $(COBJS) $(SOBJS)

PREPCORE = $(OBJ)/../lzo/prepcore

# This makes the core quite a bit smaller, but requires that the proper
# __export tags are in place. It would be very good to add the same things
# to the libraries, especially libcom32, for the same reason.
CFLAGS += -fvisibility=hidden

CFLAGS += -D__SYSLINUX_CORE__ -I$(objdir)

# The DATE is set on the make command line when building binaries for
# official release.  Otherwise, substitute a hex string that is pretty much
# guaranteed to be unique to be unique from build to build.
ifndef HEXDATE
HEXDATE := $(shell $(PERL) $(SRC)/../now.pl $(SRCS))
endif
ifndef DATE
DATE    := $(shell sh $(SRC)/../gen-id.sh $(VERSION) $(HEXDATE))
endif

ifeq ($(FWCLASS),EFI)
all: makeoutputdirs $(COBJS) $(SOBJS) codepage.o $(LIBCORE_FILES)
else
all: makeoutputdirs $(BTARGET) $(LIBCORE_FILES)
endif

makeoutputdirs:
	@mkdir -p $(sort $(dir $(COBJ) $(SOBJ)))

ifeq ($(FWCLASS),BIOS)

%.raw: %.elf
	$(OBJCOPY) -O binary -S $< $@

%.bin: %.raw $(PREPCORE)
	$(PREPCORE) $< $@

NASMFLAGS += -DDATE_STR="'$(DATE)'" -DHEXDATE="$(HEXDATE)" -I$(SRC)/bios/ -I$(objdir)/ -I$(OBJ)/

%.o: %.asm kwdhash.gen $(objdir)/version.gen

AUXLIBS = bios/libpxelinux.a bios/liblpxelinux.a
CORELDSCRIPT = $(SRC)/bios/syslinux.ld
CORELDFLAGS = $(MAIN_LDFLAGS) -pie --no-dynamic-linker \
	--unresolved-symbols=report-all \
	-E -M -T $(CORELDSCRIPT)

# Assembly file to force certain symbols to be exported even if
# they are not used by the core.
EXPORT = bios/export.o

$(ETARGET): %.elf: %.o %-c.o $(EXPORT) %.a $(CORE_LIBS) $(CORELDSCRIPT)
	$(LD) $(CORELDFLAGS) -o $@ $< $*-c.o $(EXPORT) \
		--start-group $*.a $(CORE_LIBS) --end-group \
		> $*.map
	if [ `$(NM) -D -u $@ | wc -l` -ne 0 ]; then \
		$(NM) -D -u $@ 1>&2; rm -f $@; false; fi
	$(OBJDUMP) -h $@ > $*.sec
	$(PERL) $(SRC)/lstadjust.pl $*.lsr $*.sec $*.lst

# Legacy network stack
bios/pxelinux.a: $(CORE_PXE_OBJS) $(PXELINUX_OBJS)

# LwIP network stack
bios/lpxelinux.a: $(CORE_PXE_OBJS) $(LPXELINUX_OBJS)

# Otherwise, no special library needed, a null library will be created

bios/%.a:
	rm -f $@
	$(AR) cq $@ $^
	$(RANLIB) $@

%.0: bios/%.bin
	cp -f $< $@

%.bin: bios/%.bin
	cp -f $< $@

ldlinux.bss: bios/ldlinux.bin
	dd if=$< of=$@ bs=512 count=1

ldlinux.sys: bios/ldlinux.bin
	dd if=$< of=$@ bs=512 skip=2

libcore.S: $(ETARGET) $(SRC)/export.pl
	$(PERL) '$(SRC)/export.pl' libcore.S libcore.weak $(ETARGET)

libcore.weak: libcore.S
	: $@ generated by side effect from $<

# Don't use $(SHARED) here: we don't want a SONAME on libcore.so
libcore.so: libcore.o
	$(LD) $(LDFLAGS) -shared -o $@ $<

endif # BIOS

$(CORE_LIB): $(CORE_LIBOBJS)
	rm -f $@
	$(AR) cq $@ $^
	$(RANLIB) $@

codepage.cp: $(OBJ)/../codepage/$(CODEPAGE).cp
	cp -f $< $@

codepage.o: codepage.S codepage.cp

install: installer

install-lib: installer

install-all: install install-lib

netinstall: installer

tidy dist:
	for d in $(find . -type d -print) ; do \
	( cd "$$d" ; \
	  rm -f *.o *.a .*.d *.lst \
	        codepage.cp *.elf stupid.* patch.offset .depend \
	        *.elf.tmp *.sym \
	        *.lsr *.map *.sec *.raw \
	); done
	rm -f $(OBSOLETE) $(LIB)

clean: tidy

spotless: clean
	rm -f $(BTARGET) *.bin *_bin.c

# Include dependencies file
-include $(shell find $(OBJ) -name '.*.d' -print)
