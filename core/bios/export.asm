;  List of symbols which must be exported, but otherwise might
;  have been omitted from the core library.
; 
; This creates an object file which only contains a set of undefined
; symbols, forcing those symbols to be included in the output.

%macro export 1-*.nolist
    %rep %0
        	extern %1
__dummy_%1	equ %1		; Force symbol reference
	%rotate 1
    %endrep
%endmacro

	export local_boot
	export __syslinux_shuffler_size
	export __intcall, ___farcall, ___cfarcall
	export copyright_str
	export syslinux_banner
	export __syslinux_core_version
