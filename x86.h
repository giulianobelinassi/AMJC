#ifndef X86_H
#define X86_H

class Expression;

#define X86_NO_REG -1
#define X86_EAX 0
#define X86_EBX 1
#define X86_ECX 2
#define X86_EDX 3
#define X86_ESI 4
#define X86_EDI 5

extern const char* X86_REG_STRING[];

struct x86_regs
{
    bool reg[6];
    Expression* nodes[6];
    x86_regs();

    Expression* getNodeFromReg(int reg);
    bool isRegUsed(int reg);
    
    void setReg(int reg, Expression* node);

    int findFreeRegister();

};

#endif
