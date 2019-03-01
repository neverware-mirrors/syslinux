/*
 * Allocate and zero
 */

#include <core.h>
#include <stdlib.h>
#include <string.h>

__export void *zalloc(size_t size)
{
    void *ptr;

    ptr = malloc(size);
    if (ptr)
	memset(ptr, 0, size);

    return ptr;
}
