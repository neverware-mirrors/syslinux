#
# LGPL/GPL code library
#

# Include configuration rules
include $(MAKEDIR)/elf.mk

REQFLAGS += -I$(SRC)/../gplinclude -I$(SRC)/../gplinclude/zzjson

GPLDIRS := $(SRC) $(addprefix $(SRC)/,disk dmi vpd acpi zzjson)
LIBOBJS := $(subst $(SRC)/,,$(foreach dir,$(GPLDIRS),$(patsubst %.c,%.o,$(sort $(wildcard $(dir)/*.c)))))

BINDIR   = /usr/bin
LIBDIR   = /usr/lib
DATADIR  = /usr/share
AUXDIR   = $(DATADIR)/syslinux
INCDIR   = /usr/include
COM32DIR = $(AUXDIR)/com32

LIBS     = $(LIBCOM32) $(LIBLDLINUX) $(LIBCORE)

all: makeoutputdirs libgpl.c32

.PHONY: makeoutputdirs
makeoutputdirs:
	mkdir -p $(foreach b, \
		$(addprefix $(OBJ)/,$(sort $(dir $(LIBOBJS)))),$(b))

libgpl.elf: $(LIBOBJS) $(LIBS)
	$(LD) $(LDFLAGS) $(SHARED) -o $@ $(LIBOBJS) --no-whole-archive $(LIBS)

tidy dist clean:
	find . \( -name \*.o -o -name .\*.d -o -name \*.tmp \) -print0 | \
		xargs -0r rm -f

spotless: clean
	rm -f *.c32
	rm -f *~ \#* */*~ */\#*

# Mixing in the GPL include files is suboptimal, but I'm not sure
# there is a better way to do it.
install: all
	mkdir -m 755 -p $(INSTALLROOT)$(COM32DIR)
	install -m 644 libgpl.c32 $(INSTALLROOT)$(COM32DIR)
	mkdir -p $(INSTALLROOT)$(COM32DIR)/include/
	cp -r $(SRC)/../gplinclude $(INSTALLROOT)$(COM32DIR)/include/

-include .*.d */.*.d */*/.*.d
