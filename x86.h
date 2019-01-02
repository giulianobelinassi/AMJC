#ifndef X86_H
#define X86_H

enum X86_REGISTER
{
    X86_EAX = 0,
    X86_EBX = 1,
    X86_ECX = 2,
    X86_EDX = 3,
    X86_ESI = 4,
    X86_EDI = 5
};


extern const char* X86_REG_STRING[];

struct x86_regs
{
    bool reg[6];

    x86_regs();

    int findFreeRegister();

};

#endif
