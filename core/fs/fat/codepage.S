	.section ".rodata","a"
	.globl	codepage
	.hidden	codepage
codepage:
	.incbin CODEPAGE
	.size	codepage, .-codepage
