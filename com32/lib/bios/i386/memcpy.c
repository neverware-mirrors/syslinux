/*
 * memcpy.c
 */

#include <string.h>
#include <stdint.h>

void *memcpy(void *dst, const void *src, size_t n)
{
	const char *p = src;
	char *q = dst;
	size_t nl = n >> 2;

	asm volatile ("rep movsl ; movl %3,%0 ; rep movsb"
		      : "+c" (nl), "+S" (p), "+D" (q)
		      : "r" (n & 3));

	return dst;
}
