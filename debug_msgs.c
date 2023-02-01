#include "debug_msgs.h"
#include "debug_const.h"

void debug_elf64(Elf64 *file, FILE *stream)
{
    fprintf(
        stream,
        "entry_point: %016x\n",
        file->header->e_entry
    );
    fprintf(stream, "Program_headers: [\n");
    for (int i=0; i<file->header->e_phnum; i++) {
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
    fprintf(stream, "Dynamic entries: [\n");
    for (int i=0; i<file->d_table_num; i++) {
        Elf64_dynamic *dyn = dynamic_at(file, i);
        debug_elf64_dymanic(dyn, stream, tab_2);
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

void debug_elf64_dymanic(Elf64_dynamic *dyn, FILE *stream,
    const char *tabs) 
{
    fprintf(stream, "%s- Dynamic { tag: %s, addr: %016x }\n", 
        tabs, d_tag_get_name(dyn->d_tag), dyn->d_un.d_ptr);
}
