## -*- makefile -*- ------------------------------------------------------
##
##   Copyright 2019 Intel Corporation; author: H. Peter Anvin
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
##   Boston MA 02111-1307, USA; either version 2 of the License, or
##   (at your option) any later version; incorporated herein by reference.
##
## -----------------------------------------------------------------------

include $(MAKEDIR)/build.mk

INCLUDES += -I$(SRC)/include

BINS    = prepcore

all : $(BINS)

tidy dist clean spotless:
	rm -f $(BINS)
	rm -f *.o *.a .*.d
	rm -f */*.o */*.a */.*.d

installer:

prepcore: prepcore.o

-include .*.d */.*.d
