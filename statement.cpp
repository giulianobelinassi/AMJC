#include "statement.h"
#include "macros.h"

#include <graphviz/gvc.h>

BracedStatement::BracedStatement(std::list<Statement*>* stmts)
{
    this->stmts = stmts;
}

Agnode_t* BracedStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v = NULL;
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
    return ret;
}

struct interp_ret IfElseStatement::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    return ret;
}

struct interp_ret WhileStatement::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    return ret;
}

struct interp_ret PrintStatement::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    return ret;
}

struct interp_ret VarAssignment::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    return ret;
}

struct interp_ret ArrayAssignment::interp(SymbolTable* st=NULL)
{
    struct interp_ret ret;
    return ret;
}
