#ifndef _ELF_H
#define _ELF_H 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
    uint64_t p_align
} Elf64_program_header;

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

void debug_elf64(Elf64 *file, FILE *stream);

#endif