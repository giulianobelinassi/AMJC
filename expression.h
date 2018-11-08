#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "tree.h"

class Expression
{
    public:
    virtual ~Expression() {}

    virtual struct Agnode_s* buildGVNode(struct Agraph_s*) {return NULL;}
};

class VarIdExpression : public Expression
{
    public:
    const char* token;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    VarIdExpression(const char* token);
};

class BoolExpression : public Expression
{
    public:
    bool value;
    const char* val_str;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    BoolExpression(bool);
};

class ThisExpression : public Expression
{

    public:

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    ThisExpression(void);
};

class NumExpression : public Expression
{
    public:

    int val;
    const char* val_str;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    NumExpression(const char*);
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

class MethodExpression : public Expression
{
    public:
    Expression* exp;
    VarIdExpression* id;
    std::list<Expression*>* explist;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    MethodExpression(Expression*, VarIdExpression*, std::list<Expression*>*);
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
    VarIdExpression* id;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    NewMethodExpression(VarIdExpression*);
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
