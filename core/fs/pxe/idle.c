/* ----------------------------------------------------------------------- *
 *   
 *   Copyright 2008 H. Peter Anvin - All Rights Reserved
 *   Copyright 2009 Intel Corporation; author: H. Peter Anvin
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 *   Boston MA 02110-1301, USA; either version 2 of the License, or
 *   (at your option) any later version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */

/*
 * Fallback empty idle routine, marked weak so it can be overridden
 */

#include <stdio.h>
#include <string.h>
#include <core.h>
#include <fs.h>
#include <minmax.h>
#include <x86/cpu.h>
#include "core_pxe.h"

void __attribute__((weak)) pxe_idle_init(void)
{
}

void __attribute__((weak)) pxe_idle_cleanup(void)
{
    idle_hook_func = NULL;
}
