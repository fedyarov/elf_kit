#include "debug_msgs.h"

const char* p_type_names[] = {
    "NULL",
    "LOAD",
    "DYNAMIC",
    "INTERP",
    "NOTE",
    "SHLIB",
    "PHDR",
    "TLS",
    "NUM",
    "LOOS",
    "GNU_EH_FRAME",
    "GNU_STACK",
    "GNU_RELRO",
    "GNU_PROPERY",
    "LOSUNW",
    "SUNWBSS",
    "SUNWSTACK",
    "HISUNW",
    "HIOS",
    "LOPROC",
    "HIPROC",
};
const char* undefined = "UNDEFINED";

const char *tab = "\x20\x20";
const char *tab_2 = "\x20\x20\x20\x20";

const char *p_type_get_name(const uint32_t p_type) {
    if (p_type == PT_NULL) return p_type_names[0];
    else if (p_type == PT_LOAD) return p_type_names[1];
    else if (p_type == PT_DYNAMIC) return p_type_names[2]; 
    else if (p_type == PT_INTERP) return p_type_names[3]; 
    else if (p_type == PT_NOTE) return p_type_names[4]; 
    else if (p_type == PT_SHLIB) return p_type_names[5]; 
    else if (p_type == PT_PHDR) return p_type_names[6]; 
    else if (p_type == PT_TLS) return p_type_names[7]; 
    else if (p_type == PT_NUM) return p_type_names[8];
    else if (p_type == PT_LOOS) return p_type_names[9]; 
    else if (p_type == PT_GNU_EH_FRAME) return p_type_names[10]; 
    else if (p_type == PT_GNU_STACK) return p_type_names[11]; 
    else if (p_type == PT_GNU_RELRO) return p_type_names[12];
    else if (p_type == PT_GNU_PROPERTY) return p_type_names[13]; 
    else if (p_type == PT_LOSUNW) return p_type_names[14]; 
    else if (p_type == PT_SUNWBSS) return p_type_names[15]; 
    else if (p_type == PT_HISUNW) return p_type_names[16]; 
    else if (p_type == PT_HIOS) return p_type_names[17]; 
    else if (p_type == PT_LOPROC) return p_type_names[18]; 
    else if (p_type == PT_HIPROC) return p_type_names[19]; 
    else return undefined;
}

void debug_elf64(Elf64 *file, FILE *stream)
{
    fprintf(
        stream,
        "entry_point: %016x\n",
        file->header->e_entry
    );
    fprintf(stream, "program_headers: [\n");
    for (int i=0; i<file->header->e_phnum; i++){
        Elf64_program_header *pheader = program_header_at(file, i);
        fprintf(stream, "%s{\n", tab);
        debug_elf64_program_header(
            pheader, 
            stream,
            tab_2
        );
        fprintf(stream, "%s},\n", tab);
    }
    fprintf(stream, "]\n");
}

void debug_elf64_program_header(Elf64_program_header *pheader, FILE *stream,
    const char *tabs)
{
    char p_flags[4] = "...\0";
    if (pheader->p_flags & PF_R) p_flags[0] = 'R';
    if (pheader->p_flags & PF_W) p_flags[1] = 'W';
    if (pheader->p_flags & PF_X) p_flags[2] = 'X';
    fprintf(stream, "%stype  %s\n", tabs, p_type_get_name(pheader->p_type));
    fprintf(stream, "%sflags %s\n", tabs, p_flags);
    fprintf(stream, "%sfile  %016x..%016x\n", tabs, 
        pheader->p_offset, pheader->p_offset + pheader->p_filesz);
    fprintf(stream, "%smem   %016x..%016x\n", tabs,
        pheader->p_vaddr, pheader->p_vaddr + pheader->p_memsz);
    fprintf(stream, "%salign %016x\n", tabs, pheader->p_align);
}

