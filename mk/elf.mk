## -*- makefile -*- -------------------------------------------------------
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

##
## ELF common configurables
##

# Needs to start out as null otherwise we get a recursive error
GCCOPT   :=

include $(MAKEDIR)/syslinux.mk
include $(MAKEDIR)/$(fwclass).mk

# Support IA32 and x86_64 platforms with one build
# Set up architecture specifics; for cross compilation, set ARCH as apt
GCCOPT += $(call gcc_ok,-std=gnu99,)
ifeq ($(ARCH),i386)
	GCCOPT += -m32 -march=i386
	GCCOPT += $(call gcc_ok,-mpreferred-stack-boundary=2,)
	NASMFLAGS += -f elf
endif
ifeq ($(ARCH),x86_64)
	GCCOPT += -m64 -march=x86-64
	GCCOPT += $(call gcc_ok,-march=x86-64)
# let preferred-stack-boundary be default (=4)
	NASMFLAGS += -f elf64
endif
GCCOPT += -Os -fomit-frame-pointer
GCCOPT += $(call gcc_ok,-mtune=generic,)
GCCOPT += $(call gcc_ok,-fno-stack-protector,)
GCCOPT += $(call gcc_ok,-fwrapv,)
GCCOPT += $(call gcc_ok,-freg-struct-return,)
GCCOPT += $(call gcc_ok,-fno-exceptions,)
GCCOPT += $(call gcc_ok,-fno-asynchronous-unwind-tables,)
# Note -fPIE does not work with ld on x86_64, try -fPIC instead
# Does BIOS build depend on -fPIE?
GCCOPT += $(call gcc_ok,-fPIC)
GCCOPT += $(call gcc_ok,-falign-functions=0,-malign-functions=0)
GCCOPT += $(call gcc_ok,-falign-jumps=0,-malign-jumps=0)
GCCOPT += $(call gcc_ok,-falign-labels=0,-malign-labels=0)
GCCOPT += $(call gcc_ok,-falign-loops=0,-malign-loops=0)
GCCOPT += $(call gcc_ok,-DNO_PLT -fno-plt -fvisibility=protected)

com32 = $(topdir)/com32
core = $(topdir)/core

STRIP	= strip --strip-all -R .comment -R .note

ifneq ($(NOGPL),1)
LIBGPL     = $(objdir)/com32/gpllib/libgpl.c32
GPLINCLUDE = -I$(com32)/gplinclude
else
LIBGPL     =
GPLINCLUDE =
endif
# Core symbol export library
LIBCORE = $(objdir)/core/libcore.so
# ldlinux.* library
LIBLDLINUX = $(objdir)/ldlinux/$(LDLINUX)
# Libraries used for most things
LIBCOM32  = $(objdir)/com32/lib/libcom32.c32
LIBUTIL   = $(objdir)/com32/libutil/libutil.c32

INCLUDE += -nostdinc -iwithprefix include -I$(SRC) \
	     -I$(com32)/libutil/include -I$(com32)/include \
	     -I$(com32)/include/sys $(GPLINCLUDE) \
	     -I$(core)/include -I$(core)/$(fwclass)/include -I$(objdir)

OPTFLAGS  = -Os -march=$(ARCH) -falign-functions=0 -falign-jumps=0 \
	    -falign-labels=0 -ffast-math -fomit-frame-pointer
WARNFLAGS = $(GCCWARN) -Wpointer-arith -Wwrite-strings \
	    -Wstrict-prototypes -Winline

REQFLAGS  = $(GCCOPT) -g -D__COM32__ -D__FIRMWARE_$(FIRMWARE)__ \
	     -DLDLINUX=\"$(LDLINUX)\" $(INCLUDE)

CFLAGS     = $(REQFLAGS) $(OPTFLAGS) $(WARNFLAGS) $(FWFLAGS)

SFLAGS     = $(REQFLAGS) -D__ASSEMBLY__
LDSCRIPT   	= $(com32)/lib/$(ARCH)/elf.ld
MAIN_LDFLAGS	= -m elf_$(ARCH) -Bsymbolic --hash-style=gnu \
		  -z combreloc -z now -z defs \
		  --no-allow-shlib-undefined \
		  --as-needed -nostdlib --copy-dt-needed-entries
LDFLAGS    = $(MAIN_LDFLAGS) -T $(LDSCRIPT)

ifeq ($(ARCH),i386)
else
endif
NASMDEBUG  = -g -F dwarf
NASMFLAGS += $(NASMDEBUG) -D__$(ARCH)__ -I$(SRC)/ $(filter -D%,$(CFLAGS))

LNXCFLAGS  = -I$(com32)/libutil/include -W -Wall -O -g -D_GNU_SOURCE
LNXSFLAGS  = -g
LNXLDFLAGS = -g

C_LIBS	   += $(LIBUTIL) $(LIBGPL) $(LIBCOM32) $(LIBLDLINUX) $(LIBCORE)
C_LNXLIBS  = $(objdir)/com32/libutil/libutil_lnx.a \
	     $(objdir)/com32/elflink/ldlinux/ldlinux_lnx.a

# Add to ldflags when creating a shared library
# The \ are escaped so their expansion is deferred
SHARED = -shared -soname '$(patsubst %.elf,%.c32,$(@F))'

%.o: %.asm
	$(NASM) $(NASMFLAGS) -l $*.lsr -o $@ -MP -MD $(@D)/.$(@F).d $<

%.o: %.S
	$(CC) $(SFLAGS) -c -o $@ $<

%.s: %.c
	$(CC) $(CFLAGS) -S -o $@ $<

%.i: %.c
	$(CC) $(CFLAGS) -E -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.elf: %.o $(EXTRALIBS) $(C_LIBS)
	$(LD) $(LDFLAGS) $(SHARED) -o $@ $< $(OBJS_$(*F)) $(LIBS_$(*F)) \
		$(EXTRALIBS) $(C_LIBS)

%.c32: %.elf
	$(OBJCOPY) --strip-debug --strip-unneeded $< $@
