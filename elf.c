#include "elf.h"
#include "debug_msgs.h"

#define FILE_HEADER_SIZE_64    sizeof(Elf64_header)
#define PROGRAM_HEADER_SIZE_64 sizeof(Elf64_program_header)
#define DYNAMIC_SIZE_64        sizeof(Elf64_dynamic)

#define PROGRAM_HEADER_DEBUG_PATTERN \
    "file 0807060504030201..0807060504030201 | mem 0807060504030201..0807060504030201 | align 0807060504030201 | RWX\n"
#define PROGRAM_HEADER_DEBUG_LEN_64 sizeof(PROGRAM_HEADER_DEBUG_PATTERN)

int elf64_errno = 0;
  
Elf64_header *parse_elf64_header(char *buf, size_t size) 
{
    Elf64_header *file = (Elf64_header *) malloc(sizeof(Elf64_header));
    if (file == NULL) {
        elf64_errno = (Elf_errors) MALLOC_ERROR;        
        goto malloc_error;
    }
    if (size < FILE_HEADER_SIZE_64) {
        elf64_errno == (Elf_errors) NOT_ELF;
        goto not_elf;
    };
    // TODO: add validation of MAG and others
    file = (Elf64_header *) buf;

    return file;

not_elf: 
    free(file);

malloc_error: 

    return NULL;
}

Elf64_program_header *program_header_at(Elf64_header *file, uint64_t index) 
{
    if (index >= file->e_phnum) {
        elf64_errno = (Elf_errors) INVALID_INDEX;
        return NULL;
    }
    uint64_t pheaders_addr = (uint64_t) ((char *) file + file->e_phoff);
    Elf64_program_header *pheaders = (Elf64_program_header *) pheaders_addr; 
    return &pheaders[index];
}

Elf64_dynamic *dynamic_at(Elf64_program_header *pheader, uint64_t index, Elf64_header *file) 
{
    uint64_t dtable_addr = (uint64_t) ((char *) file + pheader->p_offset);
    Elf64_dynamic *d_table = (Elf64_dynamic *) dtable_addr;
    return &d_table[index];
}

int dynamic_table_len(Elf64_program_header *pheader_dyn) 
{
    return pheader_dyn->p_filesz / DYNAMIC_SIZE_64;
}