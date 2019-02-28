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
	movl  %eax,%esi
	push  %edx
	mull  %ecx
	imull 8(%esp),%esi
	addl  %esi,%edx
	pop   %esi
	imull %esi,%ecx
	addl  %ecx,%edx
	pop   %esi
	ret
	.size __muldi3,.-__muldi3
