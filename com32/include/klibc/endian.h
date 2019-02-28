/*
 * klibc/endian.h
 *
 * Like <endian.h>, but export only double-underscore symbols
 */

#ifndef _KLIBC_ENDIAN_H
#define _KLIBC_ENDIAN_H

/* Graciously provided by gcc */
#define __LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#define __BIG_ENDIAN    __ORDER_BIG_ENDIAN__
#define __PDP_ENDIAN    __ORDER_PDP_ENDIAN__
#define __BYTE_ORDER    __BYTE_ORDER__

#endif /* _KLIBC_ENDIAN_H */
