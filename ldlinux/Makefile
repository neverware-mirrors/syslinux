## -----------------------------------------------------------------------
##
##   Copyright 2011-2019 Intel Corporation - All Rights Reserved
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
##   Boston MA 02111-1307, USA; either version 2 of the License, or
##   (at your option) any later version; incorporated herein by reference.
##
## -----------------------------------------------------------------------

include $(MAKEDIR)/elf.mk

INCLUDE += -I$(topdir)/core/elflinux -I$(topdir)/core/include \
	   -I$(com32)/lib

LDLINUX_LIBS = $(objdir)/com32/lib/libcom32.a $(LIBCORE)

# This makes the core quite a bit smaller, but requires that the proper
# __export tags are in place. It would be very good to add the same things
# to the libraries, especially libcom32, for the same reason.
CFLAGS += -fvisibility=hidden

CFLAGS += -D__LDLINUX__

OBJS = main.o cli.o readconfig.o refstr.o colors.o getadv.o adv.o \
	execute.o chainboot.o kernel.o get_key.o advwrite.o setadv.o \
	loadhigh.o msg.o

# For some reason ldlinux.elf is deleted even with .PRECIOUS, which
# breaks the dependencies later on. List it explicitly in the all target
# to force it to be kept.
all: $(LDLINUX) ldlinux.elf

ldlinux.elf: $(OBJS) $(LDLINUX_LIBS)
	$(LD) $(LDFLAGS) $(SHARED) -soname '$(LDLINUX)' -o $@ $(OBJS) $(LDLINUX_LIBS)

$(LDLINUX): ldlinux.elf
	$(OBJCOPY) --strip-debug --strip-unneeded $< $@

tidy dist:
	rm -f *.o *.lo *.a *.lst .*.d 

clean: tidy
	rm -f *.lss *.lnx *.com

spotless: clean
	rm -f *~ \#* $(BTARGET)

install: all
	mkdir -m 755 -p $(INSTALLROOT)$(AUXDIR)
	install -m 644 $(BTARGET) $(INSTALLROOT)$(AUXDIR)


-include .*.d
