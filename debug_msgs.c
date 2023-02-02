#include "debug_msgs.h"
#include "debug_const.h"

void debug_elf64(Elf64_header *file, FILE *stream)
{
    fprintf(
        stream,
        "entry_point: %016x\n\n",
        file->e_entry
    );
    for (int i=0; i<file->e_phnum; i++) {
        Elf64_program_header *pheader = program_header_at(file, i);
        fprintf(stream, "%s- ProgramHeader\n", tab_0);
        debug_elf64_program_header(
            pheader, 
            stream,
            tab_1
        );
        fprintf(stream, "\n");
        if (pheader->p_type == PT_DYNAMIC) {
            fprintf(stream, "%s-> DynamicTable: [\n", tab_1);
            for (int dyn_entry_num=0; dyn_entry_num<dynamic_table_len(pheader); dyn_entry_num++) {
                Elf64_dynamic *dyn = dynamic_at(pheader, dyn_entry_num, file);
                debug_elf64_dymanic(dyn, stream, tab_2);
            }
            fprintf(stream, "%s]\n", tab_1);
        }
    }
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
    fprintf(stream, "%s- Dynamic { tag: %s, un: %016x }\n", 
        tabs, d_tag_get_name(dyn->d_tag), dyn->d_un.d_ptr);
}
