#include "expression.h"
#include "macros.h"
#include "symboltable.h"

#include <graphviz/gvc.h>
#include <iostream>

enum interp {
    INTERP_INT,
    INTERP_BOOL,
    INTERP_TBL
};

struct interp_ret
{
    union {
        int as_int;
        bool as_bool;
        int* as_arr;
        SymbolTable* as_tbl;
    } val;
    enum interp is;
};

/*Constructors*/

OpExpression::OpExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
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

struct interp_ret ThisExpression::interp(SymbolTable* st)
{
    struct interp_ret ret;
    ret.val.as_tbl = st;
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

struct interp_ret MethodExpression::interp(SymbolTable* st)
{
    struct interp_ret exp_res;
    std::list<Expression*>::iterator exp_it;
    std::list<VarDecl*>::iretator param_it;
    std::string func_id = id->token;

    exp_res = exp->iterp(st);
    
    MethodDecl* func = exp_res.as_tbl[func_id]->func_body;


    exp_it = explist->begin();
    var_it = func->formals->begin();

    while (exp_it != explist->end())
    {
        
        
        exp_it++;
        var_it++;
    }

    for (it = explist->begin(); it != explist->end(); ++it)
    {
        
    }
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
    EXPAND_LIST_VERTEX(v, it, "ExpList", explist);

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
