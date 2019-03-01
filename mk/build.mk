## -*- makefile -*- ------------------------------------------------------
##
##   Copyright 2001-2008 H. Peter Anvin - All Rights Reserved
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
##   Boston MA 02111-1307, USA; either version 2 of the License, or
##   (at your option) any later version; incorporated herein by reference.
##
## -----------------------------------------------------------------------

##
## Right now we don't distinguish between "build" system and the "host"
## system, although we really should...
##
include $(MAKEDIR)/syslinux.mk

OPTFLAGS   = -g -O2
INCLUDES   = -I$(SRC)/include
CFLAGS_FOR_BUILD ?= $(WARNFLAGS) -D_FILE_OFFSET_BITS=64  $(OPTFLAGS)
CFLAGS_FOR_BUILD += $(INCLUDES)
LDFLAGS_FOR_BUILD =
LIBS_FOR_BUILD   =

.SUFFIXES: .c .o .S .s .i .elf .com .bin .asm .lst .c32 .lss

%.o: %.c
	$(CC_FOR_BUILD) $(UMAKEDEPS) $(CFLAGS_FOR_BUILD) -c -o $@ $<
%.i: %.c
	$(CC_FOR_BUILD) $(UMAKEDEPS) $(CFLAGS_FOR_BUILD) -E -o $@ $<
%.s: %.c
	$(CC_FOR_BUILD) $(UMAKEDEPS) $(CFLAGS_FOR_BUILD) -S -o $@ $<
%: %.o
	$(CC_FOR_BUILD) $(UMAKEDEPS) $(CFLAGS_FOR_BUILD) \
		$(LDFLAGS_FOR_BUILD) -o $@ $^ $(LIBS_FOR_BUILD)

