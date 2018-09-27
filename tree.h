#ifndef TREE_H
#define TREE_H

class Statement { public: virtual ~Statement() {} };

class Assignment : public Statement { };

class Expression { public: virtual ~Expression() {} };

class Type
{
    public:
    const char* id;

    Type(const char*);
};

class VarDecl
{
    public:
    Type* type;
    const char* id;

    VarDecl(Type*, const char*);
};

class VarDecls
{
    public:
    VarDecls* decls;
    VarDecl* decl;

    VarDecls(VarDecls*, VarDecl*);
};

class MainClass
{
    public:
    Statement* stmt;

    MainClass(Statement*);
};

class Statements
{
    public:
    Statements* rest;
    Statement* stmt;

    Statements(Statements*, Statement*);
};

class FormalList
{
    public:
    Type* type;
    const char* id;
    FormalList* rest;

    FormalList(Type*, const char*, FormalList*);
};

class MethodDecl
{
    public:
    Type* type;
    FormalList* formals;
    VarDecls* decls;
    Statements* stmts;

    MethodDecl(Type*, FormalList*, VarDecls*, Statements*);
};

class MethodDecls
{
    public:
    MethodDecls* decls;
    MethodDecl* decl;

    MethodDecls(MethodDecls*, MethodDecl*);
};

class ClassDecl
{
    public:
    VarDecls* vars;
    MethodDecls* decls;

    ClassDecl(VarDecls*, MethodDecls*);
};

class ClassDecls
{
    public:
    ClassDecls* decls; 
    ClassDecl* decl;

    ClassDecls(ClassDecls*, ClassDecl*);
};


class VarAssignment : public Assignment
{
    public:
    const char* id;
    Expression* exp;

    VarAssignment(const char*, Expression*);
};

class ArrayAssignment : public Assignment
{
    public:
    const char* id;
    Expression* exp1;
    Expression* exp2;

    ArrayAssignment(const char*, Expression*, Expression*);
};

class BracedStatement : public Statement
{
    public:
    Statements* stmts;

    BracedStatement(Statements*);
};

class IfElseStatement : public Statement
{
    public:
    Expression* cndexp;
    Statement* ifstmt;
    Statement* elsestmt;

    IfElseStatement(Expression*, Statement*, Statement*);
};

class WhileStatement : public Statement
{
    public:
    Expression* cndexp;
    Statement* whilestmt;

    WhileStatement(Expression*, Statement*);
};

class PrintStatement : public Statement
{
    public:
    Expression* exp;

    PrintStatement(Expression*);
};

class OpExpression : public Expression
{
    public: 
    Expression* exp1;
    Expression* exp2;

    OpExpression(Expression*, Expression*);
};

class LengthExpression : public Expression
{
    public: 
    Expression* exp;

    LengthExpression(Expression*);
};

class ExpList
{
    public:
    Expression* exp;
    ExpList* rest;

    ExpList(Expression*, ExpList*);
};

class MethodExpression : public Expression
{
    public: 
    Expression* exp;
    const char* id;
    ExpList* explist;

    MethodExpression(Expression*, const char*, ExpList*);
};

class NewIntArrExpression : public Expression
{
    public:
    Expression* exp;

    NewIntArrExpression(Expression*);
};

class NewMethodExpression : public Expression
{
    public:
    const char* id;

    NewMethodExpression(const char*);
};

class NegateExpression : public Expression
{
    public:
    Expression* exp;

    NegateExpression(Expression*);
};

class TokenExpression : public Expression
{
    public:
    const char* token;

    TokenExpression(const char* token);
};

class ParenExpression : public Expression
{
    public:
    Expression* exp;

    ParenExpression(Expression*);
};

class BrcktExpression : public Expression
{
    public:
    Expression* exp1;
    Expression* exp2;
};

class Program
{
    public:
    MainClass* mc;
    ClassDecls* cd;

    Program(MainClass*, ClassDecls*);
};

#endif /*TREE_H*/
