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

    nodes[X86_EAX] = nullptr;
    nodes[X86_EBX] = nullptr;
    nodes[X86_ECX] = nullptr;
    nodes[X86_EDX] = nullptr;
    nodes[X86_ESI] = nullptr;
    nodes[X86_EDI] = nullptr;
}

Expression* x86_regs::getNodeFromReg(int i)
{
    return nodes[i];
}

bool x86_regs::isRegUsed(int i)
{
    return reg[i];
}

void x86_regs::setReg(int i, Expression* node)
{
    reg[i] = true;
    nodes[i] = node;
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

int x86_regs::availableRegisters()
{
    int i, n = 0;

    for (i = 0; i < 6; ++i)
    {
        if (!reg[i])
            n++;
    }
    return n;

}
