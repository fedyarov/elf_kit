#include "elf.h"
#include "debug_msgs.h"

#define FILE_HEADER_SIZE_64    sizeof(Elf64_header)
#define PROGRAM_HEADER_SIZE_64 sizeof(Elf64_program_header)
#define DYNAMIC_SIZE_64        sizeof(Elf64_dynamic)

#define PROGRAM_HEADER_DEBUG_PATTERN \
    "file 0807060504030201..0807060504030201 | mem 0807060504030201..0807060504030201 | align 0807060504030201 | RWX\n"
#define PROGRAM_HEADER_DEBUG_LEN_64 sizeof(PROGRAM_HEADER_DEBUG_PATTERN)

int elf64_errno = 0;
  
Elf64 *parse_elf64(char *buf, size_t size) 
{
    Elf64 *file = (Elf64 *) malloc(sizeof(Elf64));
    if (file == NULL) {
        elf64_errno = (Elf_errors) MALLOC_ERROR;        
        goto malloc_error;
    }
    if (size < FILE_HEADER_SIZE_64) {
        elf64_errno == (Elf_errors) NOT_ELF;
        goto not_elf;
    };
    file->header = (Elf64_header *) buf;

    uint64_t e_phoff = file->header->e_phoff;
    size_t pheaders_size = PROGRAM_HEADER_SIZE_64 * file->header->e_phnum;
    if (e_phoff + pheaders_size > size) {
        elf64_errno = (Elf_errors) CORRUPTED_FILE;
        goto corrupted_file;
    }
    file->p_headers = (Elf64_program_header *) &buf[e_phoff];
    
    for (int i=0; i<file->header->e_phnum; i++) {
        Elf64_program_header *pheader = &file->p_headers[i];
        if (pheader->p_type == PT_DYNAMIC) {
            uint64_t dtable_off = pheader->p_offset;
            uint64_t dtable_num = pheader->p_filesz / DYNAMIC_SIZE_64;
            uint64_t dtable_size = dtable_num * DYNAMIC_SIZE_64;
            if (dtable_off + dtable_size > size) {
                elf64_errno = (Elf_errors) CORRUPTED_FILE;
                goto corrupted_file;
            }
            uint64_t dtable_addr = (uint64_t) &buf[pheader->p_offset];
            file->d_table = (Elf64_dynamic *) dtable_addr;
            file->d_table_num = dtable_num;
        }
    }

    return file;

corrupted_file:

not_elf: 
    free(file);

malloc_error: 

    return NULL;
}

Elf64_program_header *program_header_at(Elf64 *file, uint64_t index) 
{
    if (index >= file->header->e_phnum) {
        elf64_errno = (Elf_errors) INVALID_INDEX;
        return NULL;
    }
    return &file->p_headers[index];
}

Elf64_dynamic *dynamic_at(Elf64 *file, uint64_t index) 
{
    if (index >= file->d_table_num) {
        elf64_errno = (Elf_errors) INVALID_INDEX;
        return NULL;
    }
    return &file->d_table[index];
}