#include "x86.h"

const char* X86_REG_STRING[]
{
    "eax",
    "ebx",
    "ecx",
    "edx",
    "esi",
    "edi"
};

x86_regs::x86_regs()
{
    reg[X86_EAX] = false;
    reg[X86_EBX] = false;
    reg[X86_ECX] = false;
    reg[X86_EDX] = false;
    reg[X86_ESI] = false;
    reg[X86_EDI] = false;
}

int x86_regs::findFreeRegister()
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        if (!reg[i])
            return i;
    }
    return -1;
}
