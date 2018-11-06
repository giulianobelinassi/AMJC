#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "tree.h"

class Expression
{ 
    public: 
    virtual ~Expression() {}

    virtual struct Agnode_s* buildGVNode(struct Agraph_s*) {return NULL;}
};

class TokenExpression : public Expression
{
    public:
    const char* token;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    TokenExpression(const char* token);
};

class OpExpression : public Expression
{
    public: 
    Expression* exp1;
    Expression* exp2;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    OpExpression(Expression*, Expression*);
};

class LengthExpression : public Expression
{
    public: 
    Expression* exp;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    LengthExpression(Expression*);
};

class ExpList
{
    public:
    Expression* exp;
    ExpList* rest;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*);

    ExpList(Expression*, ExpList*);
};

class MethodExpression : public Expression
{
    public: 
    Expression* exp;
    TokenExpression* id;
    ExpList* explist;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    MethodExpression(Expression*, TokenExpression*, ExpList*);
};

class NewIntArrExpression : public Expression
{
    public:
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    
    NewIntArrExpression(Expression*);
};

class NewMethodExpression : public Expression
{
    public:
    TokenExpression* id;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    NewMethodExpression(TokenExpression*);
};

class NegateExpression : public Expression
{
    public:
    Expression* exp;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    NegateExpression(Expression*);
};

class ParenExpression : public Expression
{
    public:
    Expression* exp;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    ParenExpression(Expression*);
};

class BrcktExpression : public Expression
{
    public:
    Expression* exp1;
    Expression* exp2;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    BrcktExpression(Expression*, Expression*);
};

#endif
