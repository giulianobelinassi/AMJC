#ifndef STATEMENT_H
#define STATEMENT_H

#include "tree.h"
#include "expression.h"

class Statement
{ 
    public: 

    virtual struct Agnode_s* buildGVNode(struct Agraph_s*) {return NULL;}

    virtual ~Statement() {} 
    
};

class Statements
{
    public:
    Statements* rest;
    Statement* stmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    Statements(Statements*, Statement*);
};

class Assignment : public Statement { };

class BracedStatement : public Statement
{
    public:
    Statements* stmts;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    BracedStatement(Statements*);
};

class IfElseStatement : public Statement
{
    public:
    Expression* cndexp;
    Statement* ifstmt;
    Statement* elsestmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    
    IfElseStatement(Expression*, Statement*, Statement*);
};

class WhileStatement : public Statement
{
    public:
    Expression* cndexp;
    Statement* whilestmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    WhileStatement(Expression*, Statement*);
};

class PrintStatement : public Statement
{
    public:
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;

    PrintStatement(Expression*);
};


class VarAssignment : public Assignment
{
    public:
    TokenExpression* id;
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    
    VarAssignment(TokenExpression*, Expression*);
};

class ArrayAssignment : public Assignment
{
    public:
    TokenExpression* id;
    Expression* exp1;
    Expression* exp2;

    struct Agnode_s* buildGVNode(struct Agraph_s*) override;
    
    ArrayAssignment(TokenExpression*, Expression*, Expression*);
};


#endif
