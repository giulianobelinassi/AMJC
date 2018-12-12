#ifndef STATEMENT_H
#define STATEMENT_H

#include "tree.h"
#include "expression.h"
#include "interp.h"

class SymbolTable;

class Statement
{
    public:

    virtual struct Agnode_s* buildGVNode(struct Agraph_s*) {return NULL;}
    virtual struct interp_ret interp(SymbolTable*) = 0;

    virtual ~Statement() {}

};

class Assignment : public Statement { };

class BracedStatement : public Statement
{
    public:
    std::list<Statement*>* stmts;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    BracedStatement(std::list<Statement*>*);
};

class IfElseStatement : public Statement
{
    public:
    Expression* cndexp;
    Statement* ifstmt;
    Statement* elsestmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    IfElseStatement(Expression*, Statement*, Statement*);
};

class WhileStatement : public Statement
{
    public:
    Expression* cndexp;
    Statement* whilestmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    WhileStatement(Expression*, Statement*);
};

class PrintStatement : public Statement
{
    public:
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    PrintStatement(Expression*);
};


class VarAssignment : public Assignment
{
    public:
    VarIdExpression* id;
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    VarAssignment(VarIdExpression*, Expression*);
};

class ArrayAssignment : public Assignment
{
    public:
    VarIdExpression* id;
    Expression* exp1;
    Expression* exp2;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    struct interp_ret interp(SymbolTable*) override;

    ArrayAssignment(VarIdExpression*, Expression*, Expression*);
};


#endif
