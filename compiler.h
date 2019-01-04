#ifndef COMPILER_H
#define COMPILER_H

#include "x86.h"
#include "interp.h"

class SymbolTable;

struct compiler_ret
{
    SymbolTable* st;
    enum interp is;
    int aws;
};

struct compiler_args
{
    SymbolTable* st;
    struct x86_regs used;
};

#endif
