/*
 * <elfutils.h>
 *
 * Native ELF utilities
 */

#ifndef ELF_UTILS_H_
#define ELF_UTILS_H_

#include <elf.h>
#include <stdlib.h>

#ifndef ELF_MACHINE
# ifdef __x86_64__
#  define ELF_MACHINE EM_X86_64
# elif defined(__i386__)
#  define ELF_MACHINE EM_386
# else
#  error "Unknown ELF architecture"
# endif
#endif

#ifndef ELF_CLASS
# ifndef ELF_CLASS_SIZE
#  define ELF_CLASS_SIZE __SIZE_WIDTH__
# endif
# if ELF_CLASS_SIZE == 32
#  define ELF_CLASS ELFCLASS32
# else
#  define ELF_CLASS ELFCLASS64
# endif
#endif
#undef ELF_CLASS_SIZE		/* Redefined below to match ELF_CLASS */

#ifndef ELF_DATA
# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define ELF_DATA ELFDATA2LSB
# else
#  define ELF_DATA ELFDATA2MSB
# endif
#endif

#ifndef ELF_VERSION
# define ELF_VERSION EV_CURRENT
#endif

#if ELF_CLASS == ELFCLASS32
# define ELF_CLASS_SIZE		32
typedef Elf32_Addr		Elf_Addr;
typedef Elf32_Dyn		Elf_Dyn;
typedef Elf32_Word		Elf_Word;
typedef Elf32_Off		Elf_Off;
typedef Elf32_Sym		Elf_Sym;
typedef Elf32_Ehdr		Elf_Ehdr;
typedef Elf32_Phdr		Elf_Phdr;
typedef Elf32_Rel		Elf_Rel;
typedef Elf32_Bword		Elf_Bword;
# define ELF_R_SYM(x)		ELF32_R_SYM(x)
# define ELF_R_TYPE(x)		ELF32_R_TYPE(x)
#elif ELF_CLASS == ELFCLASS64
# define ELF_CLASS_SIZE		64
typedef Elf64_Addr		Elf_Addr;
typedef Elf64_Dyn		Elf_Dyn;
typedef Elf64_Word		Elf_Word;
typedef Elf64_Off		Elf_Off;
typedef Elf64_Sym		Elf_Sym;
typedef Elf64_Ehdr		Elf_Ehdr;
typedef Elf64_Phdr		Elf_Phdr;
typedef Elf64_Rel		Elf_Rel;
typedef Elf64_Bwword		Elf_Bword;
# define ELF_R_SYM(x)		ELF64_R_SYM(x)
# define ELF_R_TYPE(x)		ELF64_R_TYPE(x)
#else
# error "Invalid ELF_CLASS"
#endif

/**
 * elf_get_header - Returns a pointer to the ELF header structure.
 * @elf_image: pointer to the ELF file image in memory
 */
static inline Elf_Ehdr *elf_get_header(void *elf_image)
{
    return (Elf_Ehdr *) elf_image;
}

/**
 * elf_get_pht - Returns a pointer to the first entry in the PHT.
 * @elf_image: pointer to the ELF file image in memory
 */
static inline Elf_Phdr *elf_get_pht(void *elf_image)
{
    Elf_Ehdr *elf_hdr = elf_get_header(elf_image);

    return (Elf_Phdr *) ((Elf_Off) elf_hdr + elf_hdr->e_phoff);
}

//
/**
 * elf_get_ph - Returns the element with the given index in the PTH
 * @elf_image: pointer to the ELF file image in memory
 * @index: the index of the PHT entry to look for
 */
static inline Elf_Phdr *elf_get_ph(void *elf_image, int index)
{
    Elf_Phdr *elf_pht = elf_get_pht(elf_image);
    Elf_Ehdr *elf_hdr = elf_get_header(elf_image);

    return (Elf_Phdr *) ((Elf_Off) elf_pht + index * elf_hdr->e_phentsize);
}

/**
 * elf_hash - Returns the index in a SysV hash table for the symbol name.
 * @name: the name of the symbol to look for
 */
extern unsigned long elf_hash(const unsigned char *name);

/**
 * elf_gnu_hash - Returns the index in a GNU hash table for the symbol name.
 * @name: the name of the symbol to look for
 */
extern unsigned long elf_gnu_hash(const unsigned char *name);

#endif /*ELF_UTILS_H_ */
