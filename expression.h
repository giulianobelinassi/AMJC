#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "tree.h"
#include "interp.h"

class SymbolTable;

enum operations {
    OP_PLUS,
    OP_MINUS,
    OP_TIMES,
    OP_DIV,
    OP_GT,
    OP_GE,
    OP_EQ,
    OP_LT,
    OP_LE,
    OP_NE
};

class Expression
{
    public:
    virtual ~Expression() {}

    virtual struct Agnode_s* buildGVNode(struct Agraph_s*) {return NULL;}
    virtual struct interp_ret interp(SymbolTable*) = 0;

};

class VarIdExpression : public Expression
{
    public:
    std::string token;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    VarIdExpression(std::string token);
};

class BoolExpression : public Expression
{
    public:
    bool value;
    std::string val_str;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    BoolExpression(bool);
};

class ThisExpression : public Expression
{

    public:

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    ThisExpression(void);
};

class NumExpression : public Expression
{
    public:

    int val;
    std::string val_str;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    NumExpression(std::string);
};

class OpExpression : public Expression
{
    public:
    Expression* exp1;
    Expression* exp2;
    int op;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    OpExpression(Expression*, int, Expression*);
};

class LengthExpression : public Expression
{
    public:
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    LengthExpression(Expression*);
};

class MethodExpression : public Expression
{
    public:
    Expression* exp;
    VarIdExpression* id;
    std::list<Expression*>* explist;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    MethodExpression(Expression*, VarIdExpression*, std::list<Expression*>*);
};

class NewIntArrExpression : public Expression
{
    public:
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    NewIntArrExpression(Expression*);
};

class NewMethodExpression : public Expression
{
    public:
    VarIdExpression* id;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    NewMethodExpression(VarIdExpression*);
};

class NegateExpression : public Expression
{
    public:
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    NegateExpression(Expression*);
};

class ParenExpression : public Expression
{
    public:
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    ParenExpression(Expression*);
};

class BrcktExpression : public Expression
{
    public:
    Expression* exp1;
    Expression* exp2;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    BrcktExpression(Expression*, Expression*);
};

#endif
