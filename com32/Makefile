ifdef PRECORE
SUBDIRS = tools lib
else
SUBDIRS = tools lib libutil gpllib libupload modules mboot \
	  menu samples elflink rosh cmenu lua/src

ifneq ($(FWCLASS),EFI)
# These tools are no applicable to EFI, or need serious porting
SUBDIRS += hdt sysdump chain
endif
endif

all tidy dist clean spotless install: subdirs

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	@mkdir -p $(OBJ)/$@
	$(MAKE) -C $(OBJ)/$@ SRC='$(SRC)/$@' OBJ='$(OBJ)/$@' \
		-f '$(SRC)/$@/Makefile' $(MAKECMDGOALS)

# Parallel dependencies 
lib: tools

gpllib libupload libutil: lib
cmenu: lib libutil

chain mboot menu modules rosh: lib libutil gpllib
hdt: lib libutil gpllib libupload cmenu
lua/src: lib libutil gpllib cmenu
samples: lib libutil gpllib
sysdump: lib libutil gpllib libupload
