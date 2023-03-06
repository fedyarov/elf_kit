#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <errno.h>
#include <sys/mman.h>
#include <sys/queue.h>
#include <string.h>
#include <search.h>

#include "debug_msgs.h"
#include "elf.h"
#include "x86-64.h"

#define BASE_LOAD_ADDRESS (uint64_t) 0x400000

struct Loaded_segment {
    uint64_t aligned_vaddr;
    uint64_t padding;
    void     *region;
    uint64_t size;
    Elf64_program_header *pheader;
    SLIST_ENTRY(Loaded_segment) entry;
};

SLIST_HEAD(Loaded_segments_list, Loaded_segment);

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
    Elf64_header *file = parse_elf64_header(data, size);
    if (file == NULL) {
        fprintf(stderr, "Failed to parse Elf64: %d\n", elf64_errno);
        return -1;
    };
    debug_elf64(file, stdout);

    char input;
    printf("Press enter to continue ...\n");
    scanf("%c", &input);
    fflush(stdin);

    Elf64_program_header *pheaders = program_header_at(file, 0);
    struct Loaded_segments_list loaded_segments;
    SLIST_INIT(&loaded_segments);
    Elf64_dynamic *dtable = NULL;
    Elf64_rela *rela_table = NULL;
    Elf64_sym *sym_table = NULL;
    uint64_t rela_count = 0;
    for (int i=0; i<file->e_phnum; i++) {
        Elf64_program_header *pheader = &pheaders[i];
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
            struct Loaded_segment *loaded_segment = (struct Loaded_segment *) malloc(sizeof(struct Loaded_segment));
            loaded_segment->aligned_vaddr = aligned_vaddr;
            loaded_segment->padding = padding;
            loaded_segment->region = region;
            loaded_segment->size = region_size;
            loaded_segment->pheader = pheader;
            SLIST_INSERT_HEAD(&loaded_segments, loaded_segment, entry);
        }
        else if (pheader->p_type == PT_DYNAMIC) {
            dtable = dynamic_at(pheader, 0, (char *) BASE_LOAD_ADDRESS);
            uint64_t rela_sz = 0;
            uint64_t rela_ent = 18;
            for (int dyn_num=0; dyn_num<dynamic_table_len(pheader); dyn_num++) {
                Elf64_dynamic *dyn = &dtable[dyn_num];
                if (dyn->d_tag == DT_RELA) {
                    rela_table = rela_at(dyn, 0, (char *) BASE_LOAD_ADDRESS);
                }
                else if (dyn->d_tag == DT_RELASZ) {
                    rela_count = dyn->d_un.d_val / rela_ent;
                }
                else if (dyn->d_tag == DT_SYMTAB) {
                    sym_table = sym_at(dyn, 2, (char *) BASE_LOAD_ADDRESS);
                }
                else {
                    fprintf(stderr, "Unimplemented d_tab: %0x\n", dyn->d_tag);
                }
            }
        }
    }
    printf("All segments loaded.\n");
    printf("Press enter to continue ...\n");
    scanf("%c", &input);
    fflush(stdin);

    for (int i=0; i<rela_count; i++) {
        // TODO: add checks for processor arch
        Elf64_rela *rela = &rela_table[i];
        ret = apply_relocation_x86_64(rela, (char *) BASE_LOAD_ADDRESS);
        if (ret != 0) {
            fprintf(stderr, "failed to apply relocation: d_tag: %d, err: %d\n", ELF64_R_TYPE(rela->r_info), ret);
            return -1;
        }
    }

    struct Loaded_segment* temp_loaded_segment = NULL;
    SLIST_FOREACH(temp_loaded_segment, &loaded_segments, entry) {
        ret = mprotect(
            temp_loaded_segment->region,
            temp_loaded_segment->size,
            map_p_flags_to_mprotect_flags(temp_loaded_segment->pheader->p_flags)
        );
        if (ret != 0) {
            fprintf(stderr, "mprotect failed: %d\n", errno);
            return -1;
        }
    }

    uint64_t entry_point = file->e_entry + BASE_LOAD_ADDRESS;    
    void (*code)(void) = (void *) entry_point;
    code();

    fclose(fd);
    return 0;
}
