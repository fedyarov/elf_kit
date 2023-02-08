#ifndef _X86_64_H
#define _X86_64_H

#include "elf.h"

#define R_TYPE_NONE      0 
#define R_TYPE_64        1
#define R_TYPE_PC32      2
#define R_TYPE_GOT32     3
#define R_TYPE_PLT32     4
#define R_TYPE_COPY      5
#define R_TYPE_GLOB_DAT  6
#define R_TYPE_JUMP_SLOT 7
#define R_TYPE_RELATIVE  8

int apply_relocation_x86_64(Elf64_rela *rela, char* base);

#endif