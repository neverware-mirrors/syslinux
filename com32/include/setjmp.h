/*
 * setjmp.h
 */

#ifndef _SETJMP_H
#define _SETJMP_H

#if defined(__FIRMWARE_EFI32__) || defined(__FIRMWARE_EFI64__)

# include <efi.h>

#else

# include <klibc/extern.h>
# include <klibc/compiler.h>
# include <stddef.h>

# ifdef __i386__
#  include <klibc/i386/archsetjmp.h>
# elif defined(__x86_64__)
#  include <klibc/x86_64/archsetjmp.h>
# else
#  error "unsupported architecture"
# endif

__extern int setjmp(jmp_buf);
__extern __noreturn longjmp(jmp_buf, int);

typedef jmp_buf sigjmp_buf;

# define sigsetjmp(__env, __save) setjmp(__env)
# define siglongjmp(__env, __val) longjmp(__env, __val)

#endif

#endif /* _SETJMP_H */
