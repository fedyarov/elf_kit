#include "elf.h"
#include "debug_msgs.h"

#define PROGRAM_HEADER_DEBUG_PATTERN \
    "file 0807060504030201..0807060504030201 | mem 0807060504030201..0807060504030201 | align 0807060504030201 | RWX\n"
#define PROGRAM_HEADER_DEBUG_LEN_64 sizeof(PROGRAM_HEADER_DEBUG_PATTERN)
  
int parse_elf64(Elf64 *dest, char *buf, size_t size) 
{
    if (size < FILE_HEADER_SIZE_64) {
        return (Elf_errors) NOT_ELF;
    };
    dest->header = (Elf64_header *) buf;
    if (size < PROGRAM_HEADER_SIZE_64 * dest->header->e_phnum) {
        return (Elf_errors) CORRUPTED_FILE;
    }
    uint64_t e_phoff = dest->header->e_phoff;
    dest->p_headers = &buf[e_phoff];

    return 0;
}

int find_pheader_with_addr_64(Elf64 *file, uint64_t addr, 
    Elf64_program_header *pheader) 
{
    int e_phnum = file->header->e_phnum;
    for (int i=0; i<e_phnum; i++) {
        Elf64_program_header *temp_pheader = PROGRAM_HEADER_AT(file->p_headers, i);
        uint64_t seg_begin = temp_pheader->p_vaddr;
        uint64_t seg_end = seg_begin + temp_pheader->p_memsz;
        if (addr >= seg_begin && addr < seg_end) {
            *pheader = *temp_pheader;
            return 0;
        }
    } 
    return (Elf_errors) ADDR_NOT_FOUND_IN_PHEADERS;
}