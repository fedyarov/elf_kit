#include "x86-64.h"

int apply_relative(Elf64_rela *rela, char *base);

int apply_relocation_x86_64(Elf64_rela *rela, char* base)
{
    // TODO: add check for LE or BE
    switch ELF64_R_TYPE(rela->r_info) {
        case R_TYPE_NONE: return -1;
        case R_TYPE_64: return -1;
        case R_TYPE_PC32: return -1;
        case R_TYPE_GOT32: return -1;
        case R_TYPE_PLT32: return -1;
        case R_TYPE_COPY: return -1;
        case R_TYPE_GLOB_DAT: return -1;
        case R_TYPE_JUMP_SLOT: return -1;
        case R_TYPE_RELATIVE: return apply_relative(rela, base);
        default: return -1;
    }
}

int apply_relative(Elf64_rela *rela, char *base)
{
    uint64_t *reloc_addr = (uint64_t *) (base + rela->r_offset);
    uint64_t base_addr = (uint64_t) &(*base);
    *reloc_addr = base_addr + rela->r_addend;

    return 0;
}