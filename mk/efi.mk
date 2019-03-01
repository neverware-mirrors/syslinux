# Support IA32 and x86_64 platforms with one build
# Set up architecture specifics; for cross compilation, set ARCH as apt
# gnuefi sets up architecture specifics in ia32 or x86_64 sub directories
# set up the LIBDIR and EFIINC for building for the appropriate architecture
GCCOPT += -DEFI_FUNCTION_WRAPPER
GCCOPT += -mno-red-zone
GCCOPT += -fshort-wchar

FWFLAGS = -Wno-unused-parameter -Wno-strict-prototypes

FWLIBS  = 

EFIINCDIR  = $(objdir)/com32/efi/include/efi
EFILIBDIR  = $(objdir)/com32/efi/lib

EFI_SUBARCH := $(ARCH:i386=ia32)
FORMAT=efi-app-$(EFI_SUBARCH)

INCLUDE += -I$(EFIINCDIR) -I$(EFIINCDIR)/$(EFI_SUBARCH)

CRT0     = $(LIBDIR)/crt0-efi-$(EFI_SUBARCH).o
LDSCRIPT = $(LIBDIR)/elf_$(EFI_SUBARCH)_efi.lds

MAIN_LDFLAGS += -T $(EFILIBSRC)/$(ARCH)/syslinux.ld -Bsymbolic -pie -nostdlib -znocombreloc \
		-L$(LIBDIR) --hash-style=gnu -m elf_$(ARCH) $(CRT0) -E

SFLAGS     = $(CFLAGS) -D__ASSEMBLY__

LIBEFI = $(com32)/efi/lib/libefi.a

%.o: %.S	# Cancel old rule

%.o: %.c

%.o: %.S $(LIBEFI)
	$(CC) $(SFLAGS) -c -o $@ $<

%.o: %.c $(LIBEFI)
	$(CC) $(CFLAGS) -c -o $@ $<
