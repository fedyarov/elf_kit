#ifndef _DEBUG_MSGS_H
#define _DEBUG_MSGS_H

#include "elf.h"

void debug_elf64(Elf64_header *file, FILE *stream);
void debug_elf64_program_header(Elf64_program_header *pheader, FILE *stream,
    const char *tabs);
void debug_elf64_dymanic(Elf64_dynamic *dyn, FILE *stream,
    const char *tabs);
void debug_elf64_rela(Elf64_rela *rela, FILE *stream,
    const char *tabs);

#endif