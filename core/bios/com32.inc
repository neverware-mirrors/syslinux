;; -----------------------------------------------------------------------
;;
;;   Copyright 1994-2009 H. Peter Anvin - All Rights Reserved
;;   Copyright 2009-2010 Intel Corporation; author: H. Peter Anvin
;;
;;   This program is free software; you can redistribute it and/or modify
;;   it under the terms of the GNU General Public License as published by
;;   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
;;   Boston MA 02111-1307, USA; either version 2 of the License, or
;;   (at your option) any later version; incorporated herein by reference.
;;
;; -----------------------------------------------------------------------

;;
;; com32.inc
;;
;; Common code for running a COM32 image
;;
		section .uibss
Com32Name	resb FILENAME_MAX

		section .bss16
		alignb 4
		hidden HighMemSize:data
HighMemSize	resd 1			; End of memory pointer (bytes)
%ifndef HAVE_CURRENTDIRNAME
		hidden CurrentDirName:data
CurrentDirName	resb FILENAME_MAX
%endif

		section .text16
