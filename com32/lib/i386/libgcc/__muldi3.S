/*
 * arch/i386/libgcc/__muldi3.S
 *
 * 64*64 = 64 bit unsigned multiplication
 */

	.text
	.align 4
	.globl __muldi3
	.type __muldi3,@function
__muldi3:
	/* i386 */
	push  %esi
	movl   8(%esp),%esi
	movl  12(%esp),%ecx
	imul  %esi,%edx
	imul  %eax,%ecx
	add   %edx,%ecx
	mul   %esi
	pop   %esi
	add   %ecx,%edx
	ret
	.size __muldi3,.-__muldi3
