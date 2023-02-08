#ifndef _ELF_H
#define _ELF_H 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Elf64_header Elf64_header;
typedef struct Elf64_program_header Elf64_program_header;
typedef struct Elf64_dynamic Elf64_dynamic;
typedef struct Elf64_rela Elf64_rela;

typedef uint64_t elf64_addr;

extern int elf64_errno;

typedef enum {
    MALLOC_ERROR = 1,

    NOT_ELF,
    CORRUPTED_FILE,

    INVALID_INDEX,

    NOT_DYNAMIC,
    NOT_RELA,
} Elf_errors;

struct Elf64_header {
    char       e_ident[16];
    uint16_t   e_type;
    uint16_t   e_machine;
    uint32_t   e_version;
    elf64_addr e_entry;
    uint64_t   e_phoff;
    uint64_t   e_shoff;
    uint32_t   e_flags;
    uint16_t   e_ehsize;
    uint16_t   e_phentsize;
    uint16_t   e_phnum;
    uint16_t   e_shentsize;
    uint16_t   e_shnum;
    uint16_t   e_shstrndx;
};

Elf64_header *parse_elf64_header(char *buf, size_t size);
Elf64_program_header *program_header_at(Elf64_header *file, uint64_t index);
/**
 * @brief get Elf64_dynamic at position index
 * 
 * @param pheader_dyn 
 * @param index num of ELf64_dynamic at dynamic table
 * @param mem abstract pointer to region when data is mapped at moment of call. 
 * It can be pointer to the all elf file that just readed to the linker buffer 
 * or it can be pointer to the already mapped segment.
 */
Elf64_dynamic *dynamic_at(Elf64_program_header *pheader_dyn, uint64_t index, char *mem);
int dynamic_table_len(Elf64_program_header *pheader_dyn);
/**
 * @brief get Elf64_rela at position index
 * 
 * @param pheader_dyn 
 * @param index num of Elf64_rela at relocations table
 * @param mem abstract pointer to region when data is mapped at moment of call. 
 * It can be pointer to the all elf file that just readed to the linker buffer 
 * or it can be pointer to the already mapped segment.
 */
Elf64_rela *rela_at(Elf64_dynamic *dyn, uint64_t index, char *mem);

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

struct Elf64_program_header {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    elf64_addr p_vaddr;
    elf64_addr p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};

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
#define PT_SUNWBSS	0x6ffffffa	/* Sun Specific segment */
#define PT_SUNWSTACK	0x6ffffffb	/* Stack segment */
#define PT_HIOS		0x6fffffff	/* End of OS-specific */
#define PT_LOPROC	0x70000000	/* Start of processor-specific */
#define PT_HIPROC	0x7fffffff	/* End of processor-specific */

/* Legal values for p_flags (segment flags) */

#define PF_X (1 << 0)
#define PF_W (1 << 1)
#define PF_R (1 << 2)

struct Elf64_dynamic {
    int64_t d_tag;
    union {
        uint64_t   d_val;
        elf64_addr d_ptr;
    } d_un;
};

/* Legal values for d_tag (dynamic entry type).  */

#define DT_NULL		0		/* Marks end of dynamic section */
#define DT_NEEDED	1		/* Name of needed library */
#define DT_PLTRELSZ	2		/* Size in bytes of PLT relocs */
#define DT_PLTGOT	3		/* Processor defined value */
#define DT_HASH		4		/* Address of symbol hash table */
#define DT_STRTAB	5		/* Address of string table */
#define DT_SYMTAB	6		/* Address of symbol table */
#define DT_RELA		7		/* Address of Rela relocs */
#define DT_RELASZ	8		/* Total size of Rela relocs */
#define DT_RELAENT	9		/* Size of one Rela reloc */
#define DT_STRSZ	10		/* Size of string table */
#define DT_SYMENT	11		/* Size of one symbol table entry */
#define DT_INIT		12		/* Address of init function */
#define DT_FINI		13		/* Address of termination function */
#define DT_SONAME	14		/* Name of shared object */
#define DT_RPATH	15		/* Library search path (deprecated) */
#define DT_SYMBOLIC	16		/* Start symbol search here */
#define DT_REL		17		/* Address of Rel relocs */
#define DT_RELSZ	18		/* Total size of Rel relocs */
#define DT_RELENT	19		/* Size of one Rel reloc */
#define DT_PLTREL	20		/* Type of reloc in PLT */
#define DT_DEBUG	21		/* For debugging; unspecified */
#define DT_TEXTREL	22		/* Reloc might modify .text */
#define DT_JMPREL	23		/* Address of PLT relocs */
#define	DT_BIND_NOW	24		/* Process relocations of object */
#define	DT_INIT_ARRAY	25  /* Array with addresses of init fct */
#define	DT_FINI_ARRAY	26	/* Array with addresses of fini fct */
#define	DT_INIT_ARRAYSZ	27	/* Size in bytes of DT_INIT_ARRAY */
#define	DT_FINI_ARRAYSZ	28	/* Size in bytes of DT_FINI_ARRAY */
#define DT_RUNPATH	29		/* Library search path */
#define DT_FLAGS	30		/* Flags for the object being loaded */
#define DT_ENCODING	31		/* Start of encoded range */
#define DT_PREINIT_ARRAY 32	    /* Array with addresses of preinit fct*/
#define DT_PREINIT_ARRAYSZ 33	/* size in bytes of DT_PREINIT_ARRAY */
#define DT_SYMTAB_SHNDX	34		/* Address of SYMTAB_SHNDX section */
#define DT_RELRSZ	35		    /* Total size of RELR relative relocations */
#define DT_RELR		36		    /* Address of RELR relative relocations */
#define DT_RELRENT	37		    /* Size of one RELR relative relocaction */
#define	DT_NUM		38		    /* Number used */
#define DT_LOOS		0x6000000d	/* Start of OS-specific */
#define DT_HIOS		0x6ffff000	/* End of OS-specific */
#define DT_LOPROC	0x70000000	/* Start of processor-specific */
#define DT_HIPROC	0x7fffffff	/* End of processor-specific */
#define DT_GNU_HASH	0x6ffffef5	/* GNU-style hash table.  */
#define DT_RELACOUNT 0x6ffffff9
#define DT_FLAGS_1	0x6ffffffb	/* State flags, see DF_1_* below.  */

struct Elf64_rela {
    elf64_addr r_offset;
    uint64_t   r_info;
    int64_t    r_addend;
};

#define ELF64_R_SYM(i) ((i) >> 32)
#define ELF64_R_TYPE(i) ((i) & 0xffffffffL)
#define ELF64_R_INFO(s, t) (((s) << 32) + ((t) & 0xffffffffL))

#endif