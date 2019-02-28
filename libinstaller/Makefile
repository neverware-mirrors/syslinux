# _bin.c files required by both BTARGET and ITARGET installers
BINFILES = bootsect_bin.c ldlinux_bin.c \
	   mbr_bin.c gptmbr_bin.c ldlinuxc32_bin.c

PERL	 = perl

VPATH = $(SRC)

all: installer

installer: $(BINFILES)

bootsect_bin.c: $(objdir)/core/ldlinux.bss bin2c.pl
	$(PERL) $(SRC)/bin2c.pl syslinux_bootsect < $< > $@

ldlinux_bin.c: $(objdir)/core/ldlinux.sys bin2c.pl
	$(PERL) $(SRC)/bin2c.pl syslinux_ldlinux 512 < $< > $@

mbr_bin.c: $(objdir)/mbr/mbr.bin bin2c.pl
	$(PERL) $(SRC)/bin2c.pl syslinux_mbr < $< > $@

gptmbr_bin.c: $(objdir)/mbr/gptmbr.bin bin2c.pl
	$(PERL) $(SRC)/bin2c.pl syslinux_gptmbr < $< > $@

ldlinuxc32_bin.c: $(objdir)/ldlinux/ldlinux.c32 bin2c.pl
	$(PERL) $(SRC)/bin2c.pl syslinux_ldlinuxc32 < $< > $@

tidy:
	rm -f $(BINFILES)

clean: tidy

dist: tidy

spotless: clean

strip:
