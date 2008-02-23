; -*- fundamental -*- ---------------------------------------------------
;
;   Copyright 2004-2008 H. Peter Anvin - All Rights Reserved
;
;   This program is free software; you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
;   Boston MA 02111-1307, USA; either version 2 of the License, or
;   (at your option) any later version; incorporated herein by reference.
;
; -----------------------------------------------------------------------

;
; rllpack.inc
;
; Very simple RLL compressor/decompressor, used to pack binary structures
; together.
;
; Format of leading byte
; 1-128		= x verbatim bytes follow
; 129-223	= (x-126) times subsequent byte
; 224-255	= (x-224)*256+(next byte) times the following byte
; 0		= end of data
;
; These structures are stored *in reverse order* in high memory.
; High memory pointers point to one byte beyond the end.
;

		section .text

;
; rllpack:
;	Pack CX bytes from SI into EDI.
;	Returns updated SI and EDI.
;
rllpack:
		push cx
		push edx
		mov dx,.pm
		call pm_call
		pop edx
		pop cx
		ret

		bits 32
.pm:
.startseq:
		movzx esi,si
		xor eax,eax		; Zero byte
		xor ebx,ebx		; Run length zero
		movzx ecx,cx
		dec edi
		mov edx,edi		; Pointer to header byte
		mov [edi],al		; Create header byte
		jecxz .done		; If done, this was the terminator
.stdbyte:
		lodsb
		dec edi
		mov [edi],al
		dec ecx
		cmp ah,al
		je .same
.diff:
		mov ah,al
		xor ebx,ebx
.plainbyte:
		inc ebx
		inc byte [edx]
		jecxz .startseq
		jns .stdbyte
		jmp .startseq
.same:
		cmp bl,2
		jb .plainbyte
		; 3 bytes or more in a row, time to convert sequence
		sub [edx],bl
		jnz .normal
		inc edi			; We killed a whole stretch,
					; drop start byte
.normal:
		inc ebx
		add edi,ebx		; Remove the stored run bytes
.getrun:
		jcxz .nomatch
		lodsb
		cmp al,ah
		jne .nomatch
		cmp bx,(256-224)*256-1	; Maximum run size
		jae .nomatch
		inc ebx
		dec ecx
		jmp .getrun
.nomatch:
		cmp ebx,224-126
		jae .twobyte
.onebyte:
		add bl,126
		dec edi
		mov [edi],bl
		jmp .storebyte
.twobyte:
		add bh,224
		sub edi,2
		mov [edi],bx
.storebyte:
		dec edi
		mov [edi],ah
		dec esi			; Reload subsequent byte
		jmp .startseq
.done:
		ret

;
; rllunpack:
;	Unpack bytes from ESI into DI
;	On return ESI, DI are updated and CX contains number of bytes output.
;
		bits 16
rllunpack:
		push edx
		mov dx,.pm
		call pm_call
		pop edx
		ret

		bits 32
.pm:
		movzx edi,di
		push edi
		xor ecx,ecx
.header:
		dec esi
		mov cl,[esi]
		jecxz .done
		cmp cl,129
		jae .isrun
		; Not a run
.copy:
		dec esi
		mov al,[esi]
		stosb
		loop .copy
		jmp .header
.isrun:
		cmp cl,224
		jae .longrun
		sub cl,126
.dorun:
		dec esi
		mov al,[esi]
		rep stosb
		jmp .header
.longrun:
		sub cl,224
		mov ch,cl
		dec esi
		mov cl,[esi]
		jmp .dorun
.done:
		pop ecx
		sub ecx,edi
		neg ecx
		ret

		bits 16
