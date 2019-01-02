#ifndef COMPILER_H
#define COMPILER_H

#include "x86.h"

class SymbolTable;

struct compiler_ret
{
    x86_regs in_use;
    SymbolTable* st;
    enum interp is;
    X86_REGISTER aws;
};

struct compiler_args
{
    SymbolTable* st;
    struct x86_regs used;
};

#endif
