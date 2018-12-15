#include "statement.h"
#include "macros.h"
#include "interp.h"
#include "symboltable.h"

#include <graphviz/gvc.h>
#include <iostream>

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

    while (cnd_bool)
    {
        ret = whilestmt->interp(st);

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

    if (st->checkIfDeclared(id->token))
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
    struct interp_ret ret;

    return ret;
}
