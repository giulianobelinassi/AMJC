#include "statement.h"
#include "macros.h"

#include <graphviz/gvc.h>

Statements::Statements(Statements* rest, Statement* stmt)
{
    this->rest = rest;
    this->stmt = stmt;
}

Agnode_t* Statements::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "Statements", rest, stmt);
    
    return v;
}

BracedStatement::BracedStatement(Statements* stmts)
{
    this->stmts = stmts;
}

Agnode_t* BracedStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "BracedStatement", stmts);
    
    return v;
}

IfElseStatement::IfElseStatement(Expression* cndexp, Statement* ifstmt, Statement* elsestmt)
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
