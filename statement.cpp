#include "statement.h"
#include "macros.h"
#include "interp.h"
#include "symboltable.h"
#include "compiler.h"

#include <graphviz/gvc.h>
#include <iostream>

int labels = 0;

BracedStatement::BracedStatement(std::list<Statement*>* stmts)
{
    this->stmts = stmts;
}

Agnode_t* BracedStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    std::list<Statement*>::iterator it;

    EXPAND_LIST_VERTEX(v, it, "BracedStatement", stmts);

    return v;
}

IfElseStatement::IfElseStatement(Expression* cndexp, Statement* ifstmt,
                                 Statement* elsestmt)
{
    this->cndexp = cndexp;
    this->ifstmt = ifstmt;
    this->elsestmt = elsestmt;
}

Agnode_t* IfElseStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "ArrayAssignment", cndexp, ifstmt, elsestmt);

    return v;
}

WhileStatement::WhileStatement(Expression* cndexp, Statement* whilestmt)
{
    this->cndexp = cndexp;
    this->whilestmt = whilestmt;
}

Agnode_t* WhileStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "WhileStatement", cndexp, whilestmt);

    return v;
}

PrintStatement::PrintStatement(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* PrintStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "PrintStatement", exp);

    return v;
}

VarAssignment::VarAssignment(VarIdExpression* id, Expression* exp)
{
    this->id = id;
    this->exp = exp;
}

Agnode_t* VarAssignment::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "VarAssignment", id, exp);
    return v;
}

ArrayAssignment::ArrayAssignment(VarIdExpression* id, Expression* exp1, Expression* exp2)
{
    this->id = id;
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Agnode_t* ArrayAssignment::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "ArrayAssignment", id, exp1, exp2);

    return v;
}

/* Interp */
struct interp_ret BracedStatement::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    std::list<Statement*>::iterator stmt_it;

    for (stmt_it = stmts->begin(); stmt_it != stmts->end(); ++stmt_it)
        (*stmt_it)->interp(st);

    ret.val.as_tbl = st;
    ret.is = INTERP_TBL;

    return ret;
}

struct interp_ret IfElseStatement::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    struct interp_ret cnd = cndexp->interp(st);
    bool cnd_bool;

    switch (cnd.is)
    {
        case INTERP_INT:
            cnd_bool = !!(cnd.val.as_int);
        break;
        case INTERP_BOOL:
            cnd_bool = cnd.val.as_bool;
        break;
        case INTERP_TBL:
            cnd_bool = !!(cnd.val.as_tbl);
        break;
        case INTERP_ARR:
            cnd_bool = !!(cnd.val.as_arr);
        break;
    }

    if (cnd_bool)
        ret = ifstmt->interp(st);
    else
        ret = elsestmt->interp(st);

    return ret;
}

struct interp_ret WhileStatement::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    struct interp_ret cnd;
    bool cnd_bool;

    cnd = cndexp->interp(st);

    switch (cnd.is)
    {
        case INTERP_INT:
            cnd_bool = !!(cnd.val.as_int);
        break;
        case INTERP_BOOL:
            cnd_bool = cnd.val.as_bool;
        break;
        case INTERP_TBL:
            cnd_bool = !!(cnd.val.as_tbl);
        break;
        case INTERP_ARR:
            cnd_bool = !!(cnd.val.as_arr);
        break;
    }

    while (cnd_bool)
    {
        ret = whilestmt->interp(st);
        cnd = cndexp->interp(st);
        switch (cnd.is)
        {
            case INTERP_INT:
                cnd_bool = !!(cnd.val.as_int);
            break;
            case INTERP_BOOL:
                cnd_bool = cnd.val.as_bool;
            break;
            case INTERP_TBL:
                cnd_bool = !!(cnd.val.as_tbl);
            break;
            case INTERP_ARR:
                cnd_bool = !!(cnd.val.as_arr);
            break;
        }
    }

    return ret;
}

struct interp_ret PrintStatement::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    ret = exp->interp(st);

    switch (ret.is)
    {
        case INTERP_INT:
            std::cout << ret.val.as_int << std::endl;
        break;
        case INTERP_BOOL:
            std::cout << ret.val.as_bool << std::endl;
        break;
        case INTERP_TBL:
            std::cout << ret.val.as_tbl << std::endl;
        break;
        case INTERP_ARR:
            std::cout << ret.val.as_arr << std::endl;
        break;
    }

    return ret;
}

struct interp_ret VarAssignment::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;

    if (!st->checkIfDeclared(id->token))
    {
        std::cout <<
            "ERROR: Assignment to undeclared variable " <<
            id->token << std::endl;
    }

    ret = exp->interp(st);

    switch (ret.is)
    {
        case INTERP_INT:
            st->table[id->token]->val.as_int = ret.val.as_int;
        break;
        case INTERP_BOOL:
            st->table[id->token]->val.as_bool = ret.val.as_bool;
        break;
        case INTERP_TBL:
            st->table[id->token]->val.as_class = ret.val.as_tbl;
        break;
        case INTERP_ARR:
            st->table[id->token]->val.as_arr = ret.val.as_arr;
        break;
    }

    return ret;
}

struct interp_ret ArrayAssignment::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret1, ret2;

    if (!st->checkIfDeclared(id->token))
    {
        std::cout <<
            "ERROR: Assignment to undeclared variable " <<
            id->token << std::endl;
    }

    ret1 = exp1->interp(st);
    ret2 = exp2->interp(st);

    if (ret1.is != INTERP_INT)
        std::cout << "WARNING: Expected integer in array brackets!" << std::endl;

    if (ret2.is != INTERP_INT)
        std::cout << "WARNING: Only integer arrays are supported!" << std::endl;

    st->table[id->token]->val.as_arr[ret1.val.as_int] = ret2.val.as_int;

    return ret2;
}

/*Compile*/

struct compiler_ret BracedStatement::compile(SymbolTable* st)
{
    struct compiler_ret ret;
    std::list<Statement*>::iterator it;

    for (it = stmts->begin(); it != stmts->end(); ++it)
    {
        ret = (*it)->compile(st);
    }

    return ret;
}

struct compiler_ret IfElseStatement::compile(SymbolTable* st)
{
    struct compiler_ret ret;
    struct x86_regs used_regs = x86_regs();
    int iflabel = labels++;
    int returnlabel = labels++;

    cndexp->compute_cost();
    ret = cndexp->compile(st, &used_regs, X86_EAX);
    std::cout << "pop eax" << std::endl;
    std::cout << "test eax, eax" << std::endl;
    //std::cout << "test " << X86_REG_STRING[ret.aws] << ", " << X86_REG_STRING[ret.aws] << std::endl;
    std::cout << "je L" << iflabel << std::endl;
    elsestmt->compile(st);
    std::cout << "jmp L" << returnlabel << std::endl;
    std::cout << "L" << iflabel << ":" << std::endl;
    ifstmt->compile(st);
    std::cout << "jmp L" << returnlabel << std::endl;
    std::cout << "L" << returnlabel << ":" << std::endl;

    return ret;
}

struct compiler_ret WhileStatement::compile(SymbolTable* st)
{
    struct compiler_ret ret;
    struct x86_regs used = x86_regs();
    int cndlabel = labels++;
    int scapelabel = labels++;

    std::cout << "L" << cndlabel << ":" << std::endl;
    ret = cndexp->compile(st, &used, X86_NO_REG);
    std::cout << "pop eax" << std::endl;
    std::cout << "test eax, eax" << std::endl;
    std::cout << "jne L" << scapelabel << std::endl;
    ret = whilestmt->compile(st);
    std::cout << "jmp L" << cndlabel << std::endl;
    std::cout << "L" << scapelabel << ":" << std::endl;

    return ret;
}

struct compiler_ret PrintStatement::compile(SymbolTable* st)
{
    struct compiler_ret ret;
    struct x86_regs used = x86_regs();

    ret = exp->compile(st, &used, X86_NO_REG);
    std::cout << "push int_fmt" << std::endl;
    std::cout << "call printf"  << std::endl;
    std::cout << "add esp, 8" << std::endl;

    return ret;
}

struct compiler_ret VarAssignment::compile(SymbolTable* st)
{
    struct compiler_ret ret;
    struct x86_regs used_regs = x86_regs();
    Symbol* symbol = st->table[id->token];

    ret = exp->compile(st, &used_regs, X86_NO_REG);

    std::cout << "pop eax" << std::endl;

    if (symbol->where == ST_CLASS)
    {
        std::cout << "mov ebx, [ebp+8]" << std::endl;
        std::cout << "mov [ebx+" << symbol->offset << "], eax" << std::endl;
    }
    else
    {
        if (symbol->offset >= 0)
            std::cout << "mov [ebp+" << symbol->offset << "], eax" << std::endl;
        else
            std::cout << "mov [ebp" << symbol->offset << "], eax" << std::endl;
    }
    return ret;
}

struct compiler_ret ArrayAssignment::compile(SymbolTable* st)
{
    struct compiler_ret ret1, ret2;
    struct x86_regs used = x86_regs();

    ret2 = exp2->compile(st, &used, X86_NO_REG);
    ret1 = exp1->compile(st, &used, X86_NO_REG);

    Symbol* symbol = st->table[id->token];
    if (symbol->where == ST_CLASS)
    {
        std::cout << "mov ebx, [ebp+8]" << std::endl;
        std::cout << "mov ebx, [ebx+" << symbol->offset << "]" << std::endl;
    }
    else
    {
        if (symbol->offset >= 0)
            std::cout << "mov ebx, [ebp+" << symbol->offset << "]" << std::endl;
        else
            std::cout << "mov ebx, [ebp" << symbol->offset << "]" << std::endl;
    }
    std::cout << "pop eax" << std::endl;
    std::cout << "pop ecx" << std::endl;
    std::cout << "mov [ebx + 4*eax], ecx" << std::endl;

    return ret1;
}
