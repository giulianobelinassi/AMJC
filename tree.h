#ifndef TREE_H
#define TREE_H

#include <stdlib.h>

#include "types.h"

struct Agraph_s;
struct Agnode_s;

class Statement
{ 
    public: 

    virtual struct Agnode_s* buildGVNode(struct Agraph_s*) {return NULL;}

    virtual ~Statement() {} 
    
};

class Assignment : public Statement { };

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

class VarDecl
{
    public:
    Type* type;
    TokenExpression* id;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    VarDecl(Type*, TokenExpression*);
};

class VarDecls
{
    public:
    VarDecls* decls;
    VarDecl* decl;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    VarDecls(VarDecls*, VarDecl*);
};

class MainClass
{
    public:
    Statement* stmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    MainClass(Statement*);
};

class Statements
{
    public:
    Statements* rest;
    Statement* stmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    Statements(Statements*, Statement*);
};

class FormalList
{
    public:
    Type* type;
    TokenExpression* id;
    FormalList* rest;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    FormalList(Type*, TokenExpression*, FormalList*);
};

class MethodDecl
{
    public:
    Type* type;
    FormalList* formals;
    VarDecls* decls;
    Statements* stmts;
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    MethodDecl(Type*, FormalList*, VarDecls*, Statements*, Expression* exp);
};

class MethodDecls
{
    public:
    MethodDecls* decls;
    MethodDecl* decl;
    
    struct Agnode_s* buildGVNode(struct Agraph_s*);

    MethodDecls(MethodDecls*, MethodDecl*);
};

class ClassDecl
{
    public:
    TokenExpression* name;
    VarDecls* vars;
    MethodDecls* decls;

    struct Agnode_s* buildGVNode(struct Agraph_s* g);

    ClassDecl(TokenExpression*, VarDecls*, MethodDecls*);
};

class ClassDecls
{
    public:
    ClassDecls* decls; 
    ClassDecl* decl;
    
    struct Agnode_s* buildGVNode(struct Agraph_s* g);

    ClassDecls(ClassDecls*, ClassDecl*);
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

class Program
{
    public:
    MainClass* mc;
    ClassDecls* cd;

    void generateGraphViz();

    Program(MainClass*, ClassDecls*);
};

#endif /*TREE_H*/
