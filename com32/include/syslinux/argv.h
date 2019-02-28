#ifndef _SYSLINUX_ARGV_H
#define _SYSLINUX_ARGV_H

#include <klibc/compiler.h>

__extern int parse_argv(char ***argv, const char *str, int reserve);

#endif
