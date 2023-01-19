#ifndef _ELF_H
#define _ELF_H 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_HEADER_SIZE_64    sizeof(Elf64_header)
#define PROGRAM_HEADER_SIZE_64 sizeof(Elf64_program_header)

#define PROGRAM_HEADER_AT(buf, index) \
    ((Elf64_program_header *) &buf[PROGRAM_HEADER_SIZE_64*index])

typedef enum {
    NOT_ELF = 1,
    CORRUPTED_FILE,

    ADDR_NOT_FOUND_IN_PHEADERS,
} Elf_errors;

typedef struct {
    char     e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} Elf64_header;

#define ELFMAG "\177ELF"

#define EI_CLASS    4 // EI_CLASS byte number
#define EI_CLASS_32 1 
#define EI_CLASS_64 2

#define EI_DATA    5
#define EI_DATA_LE 1
#define EI_DATA_BE 2 

#define EI_VERSION 6

#define EI_OSABI 7
/*

*/

#define EI_ABIVERSION 8

#define EI_PAD 9 

#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3 
#define ET_CORE   4
#define ET_LOOS   0xfe00
#define ET_HIOS   0xfeff
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xfff

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_program_header;

/* Legal valued for p_type (segment type) */

#define	PT_NULL		0		/* Program header table entry unused */
#define PT_LOAD		1		/* Loadable program segment */
#define PT_DYNAMIC	2		/* Dynamic linking information */
#define PT_INTERP	3		/* Program interpreter */
#define PT_NOTE		4		/* Auxiliary information */
#define PT_SHLIB	5		/* Reserved */
#define PT_PHDR		6		/* Entry for header table itself */
#define PT_TLS		7		/* Thread-local storage segment */
#define	PT_NUM		8		/* Number of defined types */
#define PT_LOOS		0x60000000	/* Start of OS-specific */
#define PT_GNU_EH_FRAME	0x6474e550	/* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK	0x6474e551	/* Indicates stack executability */
#define PT_GNU_RELRO	0x6474e552	/* Read-only after relocation */
#define PT_GNU_PROPERTY	0x6474e553	/* GNU property */
#define PT_LOSUNW	0x6ffffffa
#define PT_SUNWBSS	0x6ffffffa	/* Sun Specific segment */
#define PT_SUNWSTACK	0x6ffffffb	/* Stack segment */
#define PT_HISUNW	0x6fffffff
#define PT_HIOS		0x6fffffff	/* End of OS-specific */
#define PT_LOPROC	0x70000000	/* Start of processor-specific */
#define PT_HIPROC	0x7fffffff	/* End of processor-specific */

/* Legal values for p_flags (segment flags) */

#define PF_X (1 << 0)
#define PF_W (1 << 1)
#define PF_R (1 << 2)

typedef struct {
    Elf64_header *header;
    char         *p_headers;
} Elf64;

int parse_elf64(Elf64 *dest, char *buf, size_t size);

int find_pheader_with_addr_64(Elf64 *file, uint64_t addr, 
    Elf64_program_header *pheader);

#endif