PRECOREDIRS       := tools lib
PRECOREDIRS_EFI   := efi
PRECOREDIRS_BIOS  :=

POSTCOREDIRS      := libutil gpllib libupload modules mboot \
	  	    menu samples elflink rosh cmenu lua/src
POSTCOREDIRS_EFI  :=
# These tools are no applicable to EFI, or need serious porting
POSTCOREDIRS_BIOS = hdt sysdump chain

SUBDIRS  = $(PRECOREDIRS) $(PRECOREDIRS_$(FWCLASS))
ifndef PRECORE
SUBDIRS += $(POSTCOREDIRS) $(POSTCOREDIRS_$(FWCLASS))
endif

all tidy dist clean spotless install: subdirs

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	@mkdir -p $(OBJ)/$@
	$(MAKE) -C $(OBJ)/$@ SRC='$(SRC)/$@' OBJ='$(OBJ)/$@' \
		-f '$(SRC)/$@/Makefile' $(MAKECMDGOALS)

# Parallel dependencies
LIBDEP_EFI := efi
lib: tools $(LIBDEP_$(FWCLASS))

gpllib libupload libutil: lib
cmenu: lib libutil

chain mboot menu modules rosh: lib libutil gpllib
hdt: lib libutil gpllib libupload cmenu
lua/src: lib libutil gpllib cmenu
samples: lib libutil gpllib
sysdump: lib libutil gpllib libupload
