#include "expression.h"
#include "macros.h"
#include "symboltable.h"
#include "declaration.h"
#include "interp.h"
#include "statement.h"
#include "x86.h"

#include <graphviz/gvc.h>
#include <iostream>


/*Constructors*/

OpExpression::OpExpression(Expression* exp1, int op, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
    this->op = op;
}

LengthExpression::LengthExpression(Expression* exp)
{
    this->exp = exp;
}

MethodExpression::MethodExpression(Expression* exp, VarIdExpression* id,
                                   std::list<Expression*>* explist)
{
    this->exp = exp;
    this->id = id;
    this->explist = explist;
}

NewIntArrExpression::NewIntArrExpression(Expression* exp)
{
    this->exp = exp;
}


NewMethodExpression::NewMethodExpression(VarIdExpression* id)
{
    this->id = id;
}

NegateExpression::NegateExpression(Expression* exp)
{
    this->exp = exp;
}

VarIdExpression::VarIdExpression(std::string token)
{
    const char* ptr = token.c_str();
    while(*ptr != '\0')
    {
        if (*ptr++ == ' ')
            std::cout << "WARNING: Found space in variable: " << std::endl;
    }
    this->token = token;
}

ParenExpression::ParenExpression(Expression* exp)
{
    this->exp = exp;
}

BrcktExpression::BrcktExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
}

BoolExpression::BoolExpression(bool val)
{
    value = val;
    val_str = val ? "true" : "false";
}

NumExpression::NumExpression(std::string str)
{
    val_str = str;
    val = atoi(str.c_str());
}

/*interpreter*/

struct interp_ret VarIdExpression::interp(SymbolTable* st)
{
    Symbol* symbol = st->table[token];
    struct interp_ret ret;

    if (symbol->type->isInt())
    {
        ret.val.as_int = symbol->val.as_int;
        ret.is = INTERP_INT;
    }
    else if (symbol->type->isBool())
    {
        ret.val.as_bool = symbol->val.as_bool;
        ret.is = INTERP_BOOL;
    }
    else if (symbol->type->isClass())
    {
        ret.val.as_tbl = symbol->val.as_class;
        ret.is = INTERP_TBL;
    }
    else if (symbol->type->isArr())
    {
        ret.val.as_arr = symbol->val.as_arr;
        ret.is = INTERP_ARR;
    }
    else
        std::cout << "ERROR: " << token << " type is unknown" << std::endl;

    return ret;
}

struct interp_ret BoolExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    ret.val.as_bool = value;
    ret.is = INTERP_BOOL;
    return ret;
}

struct interp_ret ThisExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    ret.val.as_tbl = st->table["this"]->val.as_class;
    ret.is = INTERP_TBL;
    return ret;
}

struct interp_ret NumExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    ret.val.as_int = val;
    ret.is = INTERP_INT;
    return ret;
}

struct interp_ret LengthExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    ret.val.as_int = 0;
    ret.is = INTERP_INT;
    std::cout << "WARNING: .length is not implemented!!" << std::endl;
    return ret;
}

struct interp_ret OpExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret, ret1, ret2;
    ret1 = exp1->interp(st);
    ret2 = exp2->interp(st);

    if (ret1.is != ret2.is)
    {
        std::cout << "WARNING: Binary operations of incompatible type!" << std::endl;
    }

    switch (op)
    {
        case OP_PLUS:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {   ret.val.as_int = ret1.val.as_int + ret2.val.as_int;
                ret.is = INTERP_INT;
            }
            else
                std::cout << "WARNING: Sum of incompatible types!" << std::endl;
        break;
        case OP_MINUS:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {
                ret.val.as_int = ret1.val.as_int - ret2.val.as_int;
                ret.is = INTERP_INT;
            }
            else
                std::cout << "WARNING: Subtraction of incompatible types!" << std::endl;
        break;
        case OP_TIMES:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {
                ret.val.as_int = ret1.val.as_int * ret2.val.as_int;
                ret.is = INTERP_INT;
            }
            else
                std::cout << "WARNING: Multiplication of incompatible types!" << std::endl;
        break;
        case OP_DIV:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {   ret.val.as_int = ret1.val.as_int / ret2.val.as_int;
                ret.is = INTERP_INT;
            }
            else
                std::cout << "WARNING: Division of incompatible types!" << std::endl;
        break;
        case OP_GT:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {   ret.val.as_bool = (ret1.val.as_int > ret2.val.as_int);
                ret.is = INTERP_BOOL;
            }
            else
                std::cout << "WARNING: Comparison of incompatible types!" << std::endl;
        break;
        case OP_GE:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {   ret.val.as_bool = (ret1.val.as_int >= ret2.val.as_int);
                ret.is = INTERP_BOOL;
            }
            else
                std::cout << "WARNING: Comparison of incompatible types!" << std::endl;
        break;
        case OP_LT:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {   ret.val.as_bool = (ret1.val.as_int < ret2.val.as_int);
                ret.is = INTERP_BOOL;
            }
            else
                std::cout << "WARNING: Comparison of incompatible types!" << std::endl;
        break;
        case OP_LE:
            if (ret1.is == INTERP_INT && ret2.is == INTERP_INT)
            {   ret.val.as_bool = (ret1.val.as_int <= ret2.val.as_int);
                ret.is = INTERP_BOOL;
            }
            else
                std::cout << "WARNING: Comparison of incompatible types!" << std::endl;
        break;
        case OP_EQ:
            if (ret1.is == ret2.is && ret1.is == INTERP_INT)
            {
                ret.val.as_bool = (ret1.val.as_int == ret2.val.as_int);
                ret.is = INTERP_BOOL;
            }
            else if (ret1.is == ret2.is && ret1.is == INTERP_BOOL)
            {
                ret.val.as_bool = (ret1.val.as_bool == ret2.val.as_bool);
                ret.is = INTERP_BOOL;
            }
            else if (ret1.is == ret2.is && ret1.is == INTERP_TBL)
            {
                ret.val.as_bool = (ret1.val.as_tbl == ret2.val.as_tbl);
                ret.is = INTERP_TBL;
            }
            else
                std::cout << "WARNING: Comparison of incompatible types!" << std::endl;
        break;
        case OP_NE:
            if (ret1.is == ret2.is && ret1.is == INTERP_INT)
            {
                ret.val.as_bool = !(ret1.val.as_int == ret2.val.as_int);
                ret.is = INTERP_BOOL;
            }
            else if (ret1.is == ret2.is && ret1.is == INTERP_BOOL)
            {
                ret.val.as_bool = !(ret1.val.as_bool == ret2.val.as_bool);
                ret.is = INTERP_BOOL;
            }
            else if (ret1.is == ret2.is && ret1.is == INTERP_TBL)
            {
                ret.val.as_bool = !(ret1.val.as_tbl == ret2.val.as_tbl);
                ret.is = INTERP_TBL;
            }
            else
                std::cout << "WARNING: Comparison of incompatible types!" << std::endl;
        break;
        default:
            std::cout << "WARNING: Binary operation of incompatible types!" << std::endl;
    }

    return ret;
}

struct interp_ret BrcktExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret1, ret2, ret;

    ret1 = exp1->interp(st);
    ret2 = exp2->interp(st);

    if (ret1.is != INTERP_ARR)
        std::cout << "WARNING: Attempt to access non-array variable as an array!" << std::endl;

    if (ret2.is != INTERP_INT)
        std::cout << "WARNING: Non-integer value in array brackets!" << std::endl;

    ret.val.as_int = ret1.val.as_arr[ret2.val.as_int];
    ret.is = INTERP_INT;

    return ret;
}

struct interp_ret ParenExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;

    ret = exp->interp(st);

    return ret;
}

struct interp_ret NegateExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;

    ret = exp->interp(st);

    switch (ret.is)
    {
        case INTERP_INT:
            ret.val.as_int = !ret.val.as_int;
            return ret;

        case INTERP_BOOL:
            ret.val.as_bool = !ret.val.as_bool;
            return ret;

        default:
            std::cout << "WARNING: Negating an non-numeric value!" << std::endl;
            return ret;
    }
}
struct interp_ret NewMethodExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    Type* type;
    SymbolTable* tbl;

    type = Type::getDeclaredType(id->token);

    if (!type)
    {
        std::cout << id->token << " is not a declared!" << std::endl;
        return ret;
    }

    tbl = new SymbolTable(type, type->class_def);

    ret.val.as_tbl = tbl;
    ret.is = INTERP_TBL;

    return ret;
}
struct interp_ret NewIntArrExpression::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret, ret1;


    ret1 = exp->interp(st);

    if (ret1.is == INTERP_INT)
    {
        ret.val.as_arr = new int[ret1.val.as_int];
        ret.is = INTERP_ARR;
    }
    else
        std::cout << "WARNING: Expected integer on size of array!" << std::endl;


    std::cout << "Allocated array at address: " << ret.val.as_arr << std::endl;

    return ret;
}

struct interp_ret MethodExpression::interp(SymbolTable* st)
{
    struct interp_ret exp_res, exp_ret;

    std::list<Expression*>::iterator exp_it;
    std::list<VarDecl*>::iterator var_it;
    std::list<VarDecl*>::iterator param_it;
    std::list<Statement*>::iterator stmt_it;

    std::string func_id = id->token;

    SymbolTable* instance_symt;
    SymbolTable* frame_tbl;

    exp_res = exp->interp(st);

    if (exp_res.is != INTERP_TBL)
        std::cout << "ERROR: Non-class instance called!" << std::endl;

    instance_symt = exp_res.val.as_tbl;

    frame_tbl = new SymbolTable(instance_symt);

    instance_symt->printTable();

    if (!instance_symt->checkIfDeclared(func_id))
        std::cout << "WARNING: Called nonexisting function!" << std::endl;

    MethodDecl* func = instance_symt->table[func_id]->func_body;
    if (!func)
        std::cout << "WARNING: Called nonexisting function!" << std::endl;

    exp_it = explist->begin();
    var_it = func->formals->begin();

    st->printTable();

    while (exp_it != explist->end())
    {
        std::string var_id = (*var_it)->id->token;
        exp_ret = (*exp_it)->interp(st);

        switch (exp_ret.is)
        {
            case INTERP_INT: //Assume types are OK
                frame_tbl->table[var_id] =
                  new Symbol((*var_it)->type, exp_ret.val.as_int);
            break;

            case INTERP_BOOL:
                frame_tbl->table[var_id] =
                  new Symbol((*var_it)->type, exp_ret.val.as_bool);
            break;

            case INTERP_ARR:
                frame_tbl->table[var_id] =
                  new Symbol((*var_it)->type, exp_ret.val.as_arr);

            case INTERP_TBL:
                frame_tbl->table[var_id] =
                  new Symbol((*var_it)->type, exp_ret.val.as_tbl);
            break;
        }

        exp_it++;
        var_it++;
    }

    frame_tbl->parseVars(func->decls);
    st->printTable();

    for (stmt_it = func->stmts->begin(); stmt_it != func->stmts->end(); ++stmt_it)
        (*stmt_it)->interp(frame_tbl);

    frame_tbl->printTable();
    exp_ret = func->exp->interp(frame_tbl);
    return exp_ret;
}

/*Compiler*/

struct compiler_ret VarIdExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    Symbol* symbol = st->table[token];
    struct compiler_ret ret;
    uint32_t offset = symbol->offset;

    if (offset > 0)
        std::cout << "mov " << X86_REG_STRING[X86_EAX] << ", [ebp+" << offset << "]" << std::endl;
    else
        std::cout << "mov " << X86_REG_STRING[X86_EAX] << ", [ebp" << offset << "]" << std::endl;
    ret.aws = X86_NO_REG;
    std::cout << "push " << X86_REG_STRING[X86_EAX] << std::endl;
    //used->setReg(free_reg, this);
    
    if (symbol->type->isInt())
    {
        ret.is = INTERP_INT;
    }
    else if (symbol->type->isBool())
    {
        ret.is = INTERP_BOOL;
    }
    else if (symbol->type->isClass())
    {
        ret.is = INTERP_TBL;
    }
    else if (symbol->type->isArr())
    {
        ret.is = INTERP_ARR;
    }
    else
        std::cerr << "ERROR: " << token << " type is unknown" << std::endl;
    return ret;
}

struct compiler_ret BoolExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;

    ret.st = st;
    ret.is = INTERP_BOOL;
    //used->setReg(free_reg, this);

    if (value)
    {
        std::cout << "mov " << X86_REG_STRING[X86_EAX] << ", 1" << std::endl;
    }
    else
    {
        std::cout << "mov " << X86_REG_STRING[X86_EAX] << ", 0" << std::endl;
    }

    std::cout << "push " << X86_REG_STRING[X86_EAX] << std::endl;

    return ret;
}

struct compiler_ret ThisExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    std::cout << "mov eax, [ebp+8]" << std::endl;//'this' must be the first argument in stack
    ret.st = st->table["this"]->val.as_class;
    ret.is = INTERP_TBL;
    return ret;
}

struct compiler_ret NumExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;

    //int free_reg = used->findFreeRegister();

    std::cout << "mov " << X86_REG_STRING[X86_EAX] << ", " << val_str << std::endl;
    //used->setReg(free_reg, this);
    std::cout << "push " << X86_REG_STRING[X86_EAX] << std::endl;
    ret.is = INTERP_INT;
    ret.aws = X86_NO_REG;


    return ret;
}

struct compiler_ret OpExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    struct compiler_ret ret1, ret2;
    //int free_reg = used->findFreeRegister();

    /*
    if (exp1->cost <= exp2->cost)
    {
        ret1 = exp1->compile(st, used, pref_reg);
        ret2 = exp2->compile(st, used, free_reg);
    }
    else
    {
        ret2 = exp2->compile(st, used, free_reg);
        ret1 = exp1->compile(st, used, pref_reg); 
    }
    if (ret1.is != ret2.is)
    {
        std::cerr << "WARNING: Binary operations of incompatible type!" << std::endl;
    }
    */

    ret1 = exp1->compile(st, used, X86_NO_REG);
    ret2 = exp2->compile(st, used, X86_NO_REG);

    switch (op)
    {
        case OP_PLUS:
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            //std::cout << "add" << X86_REG_STRING[ret1.aws] << ", " << X86_REG_STRING[ret2.aws] << std::endl;
            std::cout << "add eax, ebx" << std::endl;
            std::cout << "push eax" << std::endl;
            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;
            
        break;
        case OP_MINUS:
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            std::cout << "sub eax, ebx" << std::endl;
            std::cout << "push eax" << std::endl;
            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;
        
        break;
        case OP_TIMES:
            std::cout << "xor edx, edx" << std::endl;
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            std::cout << "imul eax, ebx" << std::endl;
            std::cout << "push eax" << std::endl;
            
            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;
        break;
        case OP_DIV:
            std::cout << "xor edx, edx" << std::endl;
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            std::cout << "idiv ebx" << std::endl;
            
            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;

        break;
        case OP_GT:
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            std::cout << "sub ebx, eax" << std::endl;
            std::cout << "and ebx, 2147483648" << std::endl;
            std::cout << "push ebx" << std::endl;
            
            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;
        
        break;
        case OP_GE:
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            std::cout << "sub ebx, eax" << std::endl;
            std::cout << "dec ebx" << std::endl;
            std::cout << "and ebx, 2147483648" << std::endl;
            std::cout << "push ebx" << std::endl;
            
            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;
            
        break;
        case OP_LT:
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            std::cout << "sub eax, ebx" << std::endl;
            std::cout << "and eax, 2147483648" << std::endl;
            std::cout << "push eax" << std::endl;

            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;
        break;
        case OP_LE:
            std::cout << "pop eax" << std::endl;
            std::cout << "pop ebx" << std::endl;
            std::cout << "sub eax, ebx" << std::endl;
            std::cout << "dec eax" << std::endl;
            std::cout << "and eax, 2147483648" << std::endl;
            std::cout << "push eax" << std::endl;

            ret.aws = X86_NO_REG;
            ret.is = INTERP_INT;
        
        break;
        //case OP_EQ:
        //break;
        //case OP_NE:
        //break;
        //default:
        //    std::cerr << "WARNING: Binary operation of incompatible types!" << std::endl;
    }

    
    return ret;
}

struct compiler_ret LengthExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    return ret;
}

struct compiler_ret NewIntArrExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    return ret;
}

struct compiler_ret NewMethodExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    return ret;
}

struct compiler_ret MethodExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret, ret_it;
    std::list<Expression*>::iterator it_exp;

    ret = exp->compile(st, used, X86_NO_REG);

    for (it_exp = explist->begin(); it_exp != explist->end(); it_exp++)
    {
        ret_it = (*it_exp)->compile(st, used, X86_NO_REG);
    }

    std::cout << "call " << id->token << std::endl;
    std::cout << "push eax" << std::endl;
    ret.is = INTERP_INT;
    ret.aws = X86_NO_REG;

    return ret;
}

struct compiler_ret NegateExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    return ret;
}

struct compiler_ret ParenExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    ret = exp->compile(st, used, pref_reg);
    return ret;
}

struct compiler_ret BrcktExpression::compile(SymbolTable* st, struct x86_regs* used, int pref_reg)
{
    struct compiler_ret ret;
    return ret;
}


/*Cost computing*/
int VarIdExpression::compute_cost()
{
    cost = 1;
    return cost;
}

int BoolExpression::compute_cost()
{
    cost = 1;
    return cost;
}

int ThisExpression::compute_cost()
{
    cost = 1;
    return cost;
}

int NumExpression::compute_cost()
{
    cost = 1;
    return cost;
}

int OpExpression::compute_cost()
{
    int c1 = exp1->compute_cost();
    int c2 = exp2->compute_cost();
    int c;

    if (c1 != c2)
        c = std::max(c1, c2);
    else c = c1 + 1;

    cost = c;
    return cost;
}


int MethodExpression::compute_cost()
{
    int max_cost = 0;
    std::list<Expression*>::iterator it;

    for (it = explist->begin(); it != explist->end(); ++it)
    {
        int c = (*it)->compute_cost();
        max_cost = std::max(max_cost, c);
    }

    cost = max_cost + 1;
    return cost; //Store result in EAX
}

int NewIntArrExpression::compute_cost()
{
    cost = 1;
    return cost; //Não sei ainda
}

int NewMethodExpression::compute_cost()
{
    cost = 1;
    return cost; //Tem que chamar o malloc.
}

int NegateExpression::compute_cost()
{
    cost = 1;
    return cost;
}

int ParenExpression::compute_cost()
{
    cost = exp->compute_cost();
    return cost; 
}

int LengthExpression::compute_cost()
{
    cost = 1;
    return cost;
}
int BrcktExpression::compute_cost()
{
    int c1 = exp1->compute_cost();
    int c2 = exp2->compute_cost();
    int c;

    if (c1 != c2)
        c = std::max(c1, c2);
    else c = c1 + 1;

    cost = c;
    return cost;
}

/*GraphViz*/

Agnode_t* OpExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "OpExpression", exp1, exp2);

    return v;
}

Agnode_t* LengthExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "LengthExpression", exp);

    return v;
}

Agnode_t* MethodExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    std::list<Expression*>::iterator it;
    TWO_CHILD_VERTEX(v, "MethodExpression", exp, id);

    for (it = explist->begin(); it != explist->end(); ++it)
    {
        Agnode_t* _c1 = (*it)->buildGVNode(g);
        agedge(g, v, _c1, 0, 1);
    }

    return v;
}

Agnode_t* NewIntArrExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NewIntArrExpression", exp);

    return v;
}

Agnode_t* NewMethodExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NewMethodExpression", id);

    return v;
}

Agnode_t* NegateExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NegateExpression", exp);

    return v;
}

Agnode_t* VarIdExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, this->token.c_str());

    return v;
}

Agnode_t* ParenExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "ParenExpression", exp);

    return v;
}

Agnode_t* BrcktExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "BrcktExpression", exp1, exp2);

    return v;
}

Agnode_t* BoolExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, val_str.c_str());

    return v;
}

Agnode_t* NumExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, val_str.c_str());

    return v;
}
