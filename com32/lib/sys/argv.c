/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2004-2009 H. Peter Anvin - All Rights Reserved
 *   Copyright 2009 Intel Corporation; author: H. Peter Anvin
 *
 *   Permission is hereby granted, free of charge, to any person
 *   obtaining a copy of this software and associated documentation
 *   files (the "Software"), to deal in the Software without
 *   restriction, including without limitation the rights to use,
 *   copy, modify, merge, publish, distribute, sublicense, and/or
 *   sell copies of the Software, and to permit persons to whom
 *   the Software is furnished to do so, subject to the following
 *   conditions:
 *
 *   The above copyright notice and this permission notice shall
 *   be included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------- */

/*
 * argv.c
 *
 * Parse a single C string into argc and argv (argc is return value.)
 * memptr points to available memory.
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <syslinux/argv.h>

int parse_argv(char ***argv, const char *str, int reserve)
{
    char dummy_argv0[] = "";
    char *mem = NULL;
    const char *p = str;
    char *q = mem;
    char *r;
    char **arg = NULL;
    int wasspace = 1;
    int argc = reserve;

    /* First copy the string, turning whitespace runs into nulls */
    mem = malloc(strlen(str) + 1);
    if (!mem)
	goto fail;

    for (p = str;; p++) {
	if (*p <= ' ') {
	    if (!wasspace) {
		wasspace = 1;
		*q++ = '\0';
	    }
	} else {
	    if (wasspace) {
		argc++;
		wasspace = 0;
	    }
	    *q++ = *p;
	}

	/* This test is AFTER we have processed the null byte;
	   we treat it as a whitespace character so it terminates
	   the last argument */
	if (!*p)
	    break;
    }

    /* Now create argv */
    arg = malloc((argc+1) * sizeof(*arg));
    if (!arg)
	goto fail;

    memset(arg, 0, reserve * sizeof(*arg));
    arg += reserve;

    q--;			/* Point q to final null */
    if (mem < q)
	*arg++ = mem;		/* argv[1] */

    for (r = mem; r < q; r++) {
	if (*r == '\0') {
	    *arg++ = r + 1;
	}
    }

    *arg++ = NULL;		/* Null pointer at the end */
    *argv = arg;

    return argc;

fail:
    if (arg)
	free(arg);
    if (mem)
	free(mem);
    return -1;
}
