#include <stdio.h>

#include <errno.h>
#include <sys/mman.h>
#include <string.h>

#include "elf.h"
#include "debug_msgs.h"

#define BASE_LOAD_ADDRESS (uint64_t) 0x400000

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

int map_p_flags_to_mprotect_flags(uint32_t p_flags) {
    int mprotect_flags = 0;
    if (p_flags & PF_X) mprotect_flags |= PROT_EXEC;
    if (p_flags & PF_W) mprotect_flags |= PROT_WRITE;
    if (p_flags & PF_R) mprotect_flags |= PROT_READ;
    return mprotect_flags;
}

uint64_t align_low(uint64_t val) {
    return val & (~0xFFF);
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
    Elf64 *file = parse_elf64(data, size);
    if (file == NULL) {
        fprintf(stderr, "Failed to parse Elf64: %d\n", elf64_errno);
        return -1;
    };
    debug_elf64(file, stdout);

    char input;
    printf("Press enter to continue ...\n");
    scanf("%c", &input);
    fflush(stdin);

    for (int i=0; i<file->header->e_phnum; i++) {
        Elf64_program_header *pheader = program_header_at(file, i);
        if (pheader->p_type == PT_LOAD && pheader->p_memsz != 0 ) {
            printf("Load segment: %p\n", pheader->p_vaddr);
            uint64_t aligned_vaddr = align_low(pheader->p_vaddr);
            uint64_t padding = pheader->p_vaddr - aligned_vaddr;
            uint64_t region_size = pheader->p_memsz + padding;
            void *region = mmap(
                (void *) (aligned_vaddr + BASE_LOAD_ADDRESS),
                region_size,
                PROT_WRITE,
                MAP_ANONYMOUS | MAP_PRIVATE,
                -1,
                0
            );
            if (region == MAP_FAILED) {
                fprintf(stderr, "Failed to mmap region #%d: %d", i, errno);
                return -1;
            }
            void *segment_data = (void *) &data[pheader->p_offset];
            memcpy((void *) (pheader->p_vaddr + BASE_LOAD_ADDRESS), segment_data, pheader->p_memsz);
            ret = mprotect(
                region,
                region_size,
                map_p_flags_to_mprotect_flags(pheader->p_flags)
            );
            if (ret != 0) {
                fprintf(stderr, "mprotect failed: %d\n", errno);
                return -1;
            }
        }
    }
    printf("All segments loaded.\n");
    printf("Press enter to continue ...\n");
    scanf("%c", &input);

    uint64_t entry_point = file->header->e_entry + BASE_LOAD_ADDRESS;    
    void (*code)(void) = (void *) entry_point;
    code();

    fclose(fd);
    return 0;
}
