; -*- fundamental -*- (asm-mode sucks)
; ****************************************************************************
;
;  btrlinux.asm
;
;  A program to boot Linux kernels off an btrfs filesystem.
;
;   Copyright 1994-2009 H. Peter Anvin - All Rights Reserved
;   Copyright 2009 Intel Corporation; author: H. Peter Anvin
;
;  This program is free software; you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation, Inc., 53 Temple Place Ste 330,
;  Boston MA 02111-1307, USA; either version 2 of the License, or
;  (at your option) any later version; incorporated herein by reference.
;
; ****************************************************************************

%define IS_BTRLINUX 1
%include "head.inc"

;
; Some semi-configurable constants... change on your own risk.
;
my_id		equ btrlinux_id

		extern btrfs_fs_ops
ROOT_FS_OPS	equ btrfs_fs_ops

%include "diskfs.inc"
