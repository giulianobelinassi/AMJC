#include "macros.h"
#include "tree.h"

#include <graphviz/gvc.h>

OpExpression::OpExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Agnode_t* OpExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "OpExpression", exp1, exp2);
    
    return v;
}

LengthExpression::LengthExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* LengthExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "LengthExpression", exp);
    
    return v;
}

MethodExpression::MethodExpression(Expression* exp, TokenExpression* id, ExpList* explist)
{
    this->exp = exp;
    this->id = id;
    this->explist = explist;
}

Agnode_t* MethodExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "MethodExpression", exp, id, explist);
    
    return v;
}

NewIntArrExpression::NewIntArrExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* NewIntArrExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NewIntArrExpression", exp);
    
    return v;
}

NewMethodExpression::NewMethodExpression(TokenExpression* id)
{
    this->id = id;
}

Agnode_t* NewMethodExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NewMethodExpression", id);
    
    return v;
}

NegateExpression::NegateExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* NegateExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NegateExpression", exp);
    
    return v;
}

TokenExpression::TokenExpression(const char* token)
{
    COPY_STR(this->token, token);
}

Agnode_t* TokenExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, this->token);
    
    
    return v;
}

ParenExpression::ParenExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* ParenExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "ParenExpression", exp);
    
    return v;
}

BrcktExpression::BrcktExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Agnode_t* BrcktExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "BrcktExpression", exp1, exp2);
    
    return v;
}

ExpList::ExpList(Expression* exp, ExpList* rest)
{
    this->exp  = exp;
    this->rest = rest;
}

Agnode_t* ExpList::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "ExpList", exp, rest);
    
    return v;
}
