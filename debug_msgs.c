#include "debug_msgs.h"
#include "debug_const.h"

void debug_elf64(const Elf64_header *file, FILE *stream)
{
    fprintf(
        stream,
        "entry_point: %016x\n\n",
        file->e_entry
    );
    Elf64_rela *rela_table = NULL;
    uint32_t rela_count = 0;
    for (int i=0; i<file->e_phnum; i++) {
        Elf64_program_header *pheader = program_header_at(file, i);
        debug_elf64_program_header(
            pheader, 
            stream,
            tab_0
        );
        if (pheader->p_type == PT_DYNAMIC) {
            fprintf(stream, "%s-> DynamicTable: [\n", tab_1);
            for (int dyn_entry_num=0; dyn_entry_num<dynamic_table_len(pheader); dyn_entry_num++) {
                Elf64_dynamic *dyn = dynamic_at(pheader, dyn_entry_num, (char *) file);
                if (dyn->d_tag == DT_RELA) rela_table = rela_at(dyn, 0, (char *) file);
                else if (dyn->d_tag == DT_RELACOUNT) rela_count = dyn->d_un.d_val;
                else if (dyn->d_tag == DT_NULL) break;
                debug_elf64_dymanic(dyn, stream, tab_2);
            }
            fprintf(stream, "%s]\n", tab_1);
        }
    }
    fprintf(stream, "%s-> RelaTable: [\n", tab_0);
    for (int i=0; i<rela_count; i++) {
        Elf64_rela *rela = &rela_table[i];
        debug_elf64_rela(rela, stream, tab_1);
        fprintf(stream, "%s]\n", tab_1);
    }
}

void debug_elf64_program_header(const Elf64_program_header *pheader, FILE *stream,
    const char *tabs)
{
    char p_flags[4] = "...\0";
    if (pheader->p_flags & PF_R) p_flags[0] = 'R';
    if (pheader->p_flags & PF_W) p_flags[1] = 'W';
    if (pheader->p_flags & PF_X) p_flags[2] = 'X';
    fprintf(stream, "%s- ProgramHeader\n", tabs);
    fprintf(stream, "%s%stype  %s\n", tabs, tab_1, p_type_get_name(pheader->p_type));
    fprintf(stream, "%s%sflags %s\n", tabs, tab_1, p_flags);
    fprintf(stream, "%s%sfile  %016x..%016x\n", tabs, tab_1, 
        pheader->p_offset, pheader->p_offset + pheader->p_filesz);
    fprintf(stream, "%s%smem   %016x..%016x\n", tabs, tab_1,
        pheader->p_vaddr, pheader->p_vaddr + pheader->p_memsz);
    fprintf(stream, "%s%salign %016x\n", tabs, tab_1, pheader->p_align);
    fprintf(stream, "\n");
}

void debug_elf64_dymanic(const Elf64_dynamic *dyn, FILE *stream,
    const char *tabs) 
{
    fprintf(stream, "%s- Dynamic { tag: %s, un: %016x }\n", 
        tabs, d_tag_get_name(dyn->d_tag), dyn->d_un.d_ptr);
}

void debug_elf64_rela(const Elf64_rela *rela, FILE *stream,
    const char *tabs) 
{
    fprintf(stream, "%s- Rela\n", tabs);
    fprintf(stream, "%s%soffset %016x\n", tabs, tab_1, rela->r_offset);
    fprintf(stream, "%s%ssym    %08x\n", tabs, tab_1, ELF64_R_SYM(rela->r_info));
    fprintf(stream, "%s%stype   %08x\n", tabs, tab_1, ELF64_R_TYPE(rela->r_info));
    fprintf(stream, "%s%saddend %016x\n", tabs, tab_1, rela->r_addend);
}

void debug_elf64_sym(Elf64_sym *sym, FILE *stream,
    const char *tabs)
{
    fprintf(stream, "%s- Sym\n", tabs);
    fprintf(stream, "%s%sname  %08x\n", tabs, tab_1, sym->st_name);
    fprintf(stream, "%s%sinfo  %02x\n", tabs, tab_1, sym->st_info);
    fprintf(stream, "%s%sother %02x\n", tabs, tab_1, sym->st_other);
    fprintf(stream, "%s%sshndx %04x\n", tabs, tab_1, sym->st_shndx);
    fprintf(stream, "%s%svalue %016x\n", tabs, tab_1, sym->st_value);
    fprintf(stream, "%s%ssize  %016x\n", tabs, tab_1, sym->st_size);
}