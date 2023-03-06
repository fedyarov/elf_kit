#ifndef _DEBUG_MSGS_H
#define _DEBUG_MSGS_H

#include "elf.h"

void debug_elf64(const Elf64_header *file, FILE *stream);
void debug_elf64_program_header(const Elf64_program_header *pheader, FILE *stream,
    const char *tabs);
void debug_elf64_dymanic(const Elf64_dynamic *dyn, FILE *stream,
    const char *tabs);
void debug_elf64_rela(const Elf64_rela *rela, FILE *stream,
    const char *tabs);
void debug_elf64_sym(Elf64_sym *sym, FILE *stream,
    const char *tabs);

#endif