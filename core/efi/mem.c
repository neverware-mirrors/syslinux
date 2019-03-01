/*
 * Copyright 2012-2014 Intel Corporation - All Rights Reserved
 */

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "efi.h"

__export void *malloc(size_t size)
{
    void *p = AllocatePool(size);
    if (!p)
	errno = ENOMEM;
    return p;
}

__export void *lmalloc(size_t) __attribute__((alias("malloc")));

__export void *realloc(void *ptr, size_t size)
{
	void *newptr;

	newptr = AllocatePool(size);
	if (!newptr) {
	    errno = ENOMEM;
	    return NULL;
	}
	if (ptr) {
	    memcpy(newptr, ptr, size);
	    FreePool(ptr);
	}
	return newptr;
}

__export void free(void *ptr)
{
	FreePool(ptr);
}
