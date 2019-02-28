	.section ".rodata","a"
	.globl	codepage
	.hidden	codepage
codepage:
	.incbin "codepage.cp"
	.size	codepage, .-codepage
