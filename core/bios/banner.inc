;
; Syslinux banner and copyright string
;
		section .data16

		align 2, db 0
		export __syslinux_core_version:data 2
__syslinux_core_version:
		db VERSION_MINOR, VERSION_MAJOR

		export syslinux_banner:data (late_banner.end - syslinux_banner)
syslinux_banner:
		db CR, LF, MY_NAME, ' ', VERSION_STR
late_banner	db ' ', DATE_STR, 0
.end:

		export copyright_str:data (copyright_str.end - copyright_str)
copyright_str:
		db ' Copyright (C) 1994-'
		asciidec YEAR
		db ' H. Peter Anvin et al', CR, LF, 0
.end:

		section .text16
