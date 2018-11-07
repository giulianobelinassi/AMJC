#include "expression.h"
#include "macros.h"

#include <graphviz/gvc.h>

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

MethodExpression::MethodExpression(Expression* exp, TokenExpression* id, 
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


NewMethodExpression::NewMethodExpression(TokenExpression* id)
{
    this->id = id;
}

NegateExpression::NegateExpression(Expression* exp)
{
    this->exp = exp;
}

TokenExpression::TokenExpression(const char* token)
{
    COPY_STR(this->token, token);
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

NumExpression::NumExpression(const char* str)
{
    COPY_STR(val_str, str);
    val = atoi(str);
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

Agnode_t* TokenExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, this->token);
    
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
    ZERO_CHILD_VERTEX(v, val_str);
    
    return v;
}

Agnode_t* NumExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, val_str);

    return v;
}
