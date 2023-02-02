#ifndef _DEBUG_CONST_H
#define _DEBUG_CONST_H

const char* undefined = "UNDEFINED";

const char *p_type_get_name(const uint32_t p_type) {
    switch (p_type) {
        case PT_NULL: return "NULL";
        case PT_LOAD: return "LOAD";
        case PT_DYNAMIC: return "DYNAMIC"; 
        case PT_INTERP: return "INTERP"; 
        case PT_NOTE: return "NOTE"; 
        case PT_SHLIB: return "SHLIB"; 
        case PT_PHDR: return "PHDR"; 
        case PT_TLS: return "TLS"; 
        case PT_NUM: return "NUM";
        case PT_LOOS: return "LOOS"; 
        case PT_GNU_EH_FRAME: return "GNU_EH_FRAME"; 
        case PT_GNU_STACK: return "GNU_STACK"; 
        case PT_GNU_RELRO: return "GNU_RELRO";
        case PT_GNU_PROPERTY: return "GNU_PROPERTY"; 
        case PT_SUNWBSS: return "SUNWBSS"; 
        case PT_HIOS: return "HIOS"; 
        case PT_LOPROC: return "LOPROC"; 
        case PT_HIPROC: return "HIPROC"; 
        default: return undefined;
    }
}

const char *d_tag_get_name(const uint32_t d_tag) {
    switch (d_tag) {
        case DT_NULL: return "NULL";
        case DT_NEEDED: return "NEEDED";
        case DT_PLTRELSZ: return "PLTRELSZ";
        case DT_PLTGOT: return "PLTGOT";
        case DT_HASH: return "HASH";
        case DT_STRTAB: return "STRTAB";
        case DT_SYMTAB: return "SYMTAB";
        case DT_RELA: return "RELA";
        case DT_RELASZ: return "RELASZ";
        case DT_RELAENT: return "RELAENT";
        case DT_STRSZ: return "STRSZ";
        case DT_SYMENT: return "SYMENT";
        case DT_INIT: return "INIT";
        case DT_FINI: return "FINI";
        case DT_SONAME: return "SONAME";
        case DT_RPATH: return "RPATH";
        case DT_SYMBOLIC: return "SYMBOLIC";
        case DT_REL: return "REL";
        case DT_RELSZ: return "RELSZ";
        case DT_RELENT: return "RELENT";
        case DT_PLTREL: return "PLTREL";
        case DT_DEBUG: return "DEBUG";
        case DT_TEXTREL: return "TEXTREL";
        case DT_JMPREL: return "JMPREL";
        case DT_BIND_NOW: return "BIND_NOW";
        case DT_INIT_ARRAY: return "INIT_ARRAY";
        case DT_FINI_ARRAY: return "FINI_ARRAY";
        case DT_INIT_ARRAYSZ: return "INIT_ARRAYSZ";
        case DT_FINI_ARRAYSZ: return "FINI_ARRAYSZ";
        case DT_RUNPATH: return "RUNPATH";
        case DT_FLAGS: return "FLAGS";
        case DT_ENCODING: return "ENCODING";
        case DT_PREINIT_ARRAY: return "PREINIT_ARRAY";
        case DT_SYMTAB_SHNDX: return "SYMTAB_SHDX";
        case DT_RELRSZ: return "RELRSZ";
        case DT_RELR: return "RELR";
        case DT_RELRENT: return "RELRENT";
        case DT_NUM: return "NUM";
        case DT_LOOS: return "LOOS";
        case DT_HIOS: return "HIOS";
        case DT_LOPROC: return "LOPROC";
        case DT_HIPROC: return "HIPROC";
        case DT_GNU_HASH: return "GNU_HASH";
        case DT_RELACOUNT: return "RELACOUNT";
        case DT_FLAGS_1: return "FLAGS_1";
        default: return undefined;
    }
}

const char *tab_0 = "";
const char *tab_1 = "\x20\x20";
const char *tab_2 = "\x20\x20\x20\x20";
const char *tab_3 = "\x20\x20\x20\x20\x20\x20";

#endif