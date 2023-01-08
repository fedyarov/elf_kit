#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "elf.h"

char copied_code[0x1000] = {0};

static inline int check_args(int argc, char const *argv[])
{
    if (argc < 2) {
        return 1;
    }

    return 0;
}

static inline int get_file_size(FILE *fd) 
{
    fseek(fd, 0, SEEK_END);
    int size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    return size;
}

int main(int argc, char const *argv[])
{
    int ret = check_args(argc, argv);
    if (ret != 0) {
        fprintf(stderr, "Check args err: %d\n", ret);
        return -1;
    }

    FILE* fd = fopen(argv[1], "r");
    if (!fd) {
        fprintf(stderr, "File open error: ", errno);
        return -1;
    }
    int size = get_file_size(fd);
    char *data = (char *) malloc(size);
    if (fread(data, 1, size, fd) != size) {
        fprintf(stderr, "Failed to read file");
        return -1;
    }
    Elf64 *file = (Elf64*) malloc(sizeof(Elf64)); 
    if (!file) {
        fprintf(stderr, "Malloc error for Elf_header: ", errno);
        return -1;
    }
    ret = parse_elf64(file, data, size);
    if (ret != 0) {
        fprintf(stderr, "Parse elf error: %d\n", ret);
        return -1;
    };
    debug_elf64(file, stdout);
    Elf64_program_header *code_pheader = (Elf64_program_header *) malloc(sizeof(Elf64_program_header));
    uint64_t entry_point = file->header->e_entry;
    ret = find_pheader_with_addr_64(file, entry_point, code_pheader);
    if (ret != 0) {
        fprintf("Segment with entry point not found: %d\n", ret);
        return -1;
    }
    char *copied_code = (char *) malloc(code_pheader->p_filesz);
    //memcpy(copied_code, &data[code_pheader->p_offset], code_pheader->p_filesz);
    uint64_t code_region = (uint64_t) &data[code_pheader->p_offset];
    //uint64_t code_region = (uint64_t) copied_code;
    code_region = code_region & (~0xFFF);
    printf("code region @%p\n", code_region);
    ret = mprotect(
        (void *) code_region,
        0x1000,
        PROT_READ | PROT_EXEC
    );
    if (ret != 0) {
        fprintf(stderr, "mprotect failed: %d\n", errno);
        return -1;
    }
    uint64_t entry_offset = entry_point - code_pheader->p_vaddr;
    void (*code)(void) = (void *) &data[entry_offset];
    //void (*code)(void) = (void *) copied_code;
    code();

    fclose(fd);
    return 0;
}
