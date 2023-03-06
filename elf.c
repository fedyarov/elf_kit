#include "elf.h"
#include "debug_msgs.h"

#define FILE_HEADER_SIZE_64    sizeof(Elf64_header)
#define PROGRAM_HEADER_SIZE_64 sizeof(Elf64_program_header)
#define DYNAMIC_SIZE_64        sizeof(Elf64_dynamic)

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

Elf64_program_header *program_header_at(const Elf64_header *file, uint64_t index) 
{
    if (index >= file->e_phnum) {
        elf64_errno = (Elf_errors) INVALID_INDEX;
        return NULL;
    }
    uint64_t pheaders_addr = (uint64_t) ((char *) file + file->e_phoff);
    Elf64_program_header *pheaders = (Elf64_program_header *) pheaders_addr; 
    return &pheaders[index];
}

Elf64_section_header *section_header_at(const Elf64_header *file, uint64_t index)
{
    if (index >= file->e_shnum) {
        elf64_errno = (Elf_errors) INVALID_INDEX;
        return NULL;
    }
    uint64_t sheaders_addr = (uint64_t) ((char *) file + file->e_shoff);
    Elf64_section_header *sheaders = (Elf64_section_header *) sheaders_addr;
    return &sheaders[index];
}

Elf64_dynamic *dynamic_at(const Elf64_program_header *pheader_dyn, uint64_t index, char *mem) 
{
    if (pheader_dyn->p_type != PT_DYNAMIC) {
        elf64_errno = (Elf_errors) NOT_DYNAMIC;
        return NULL;
    }
    uint64_t dtable_addr = (uint64_t) (mem + pheader_dyn->p_offset);
    Elf64_dynamic *d_table = (Elf64_dynamic *) dtable_addr;
    return &d_table[index];
}

int dynamic_table_len(const Elf64_program_header *pheader_dyn) 
{
    if (pheader_dyn->p_type != PT_DYNAMIC) {
        elf64_errno = (Elf_errors) NOT_DYNAMIC;
        return -1;
    }
    return pheader_dyn->p_filesz / DYNAMIC_SIZE_64;
}

Elf64_rela *rela_at(const Elf64_dynamic *dyn, uint64_t index, char *mem)
{
    if (dyn->d_tag != DT_RELA) {
        elf64_errno = (Elf_errors) NOT_RELA;
        return NULL;
    }
    uint64_t rela_table_addr = (uint64_t) (mem + dyn->d_un.d_ptr);
    Elf64_rela *rela_table = (Elf64_rela *) rela_table_addr;
    return &rela_table[index];
}

Elf64_sym *sym_at(const Elf64_dynamic *dyn, uint64_t index, char *mem)
{
    if (dyn->d_tag != DT_SYMTAB) {
        elf64_errno = (Elf_errors) NOT_SYM;
        return NULL;
    }
    uint64_t symtab_addr = (uint64_t) (mem + dyn->d_un.d_ptr);
    Elf64_sym *symtab = (Elf64_sym *) symtab_addr;
    return &symtab[index];
}