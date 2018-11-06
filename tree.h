#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <list>

#include "types.h"

/* Used GraphViz structures*/
struct Agraph_s;
struct Agnode_s;

/*Expressions*/
class Expression;
class TokenExpression;
class OpExpression;
class LengthExpression;
class MethodExpression;
class NewIntArrExpression;
class NewMethodExpression;
class NegateExpression;
class ParenExpression;
class BrcktExpression; 

/*Statements*/
class Statement;
class BracedStatement;
class IfElseStatement;
class WhileStatement;
class PrintStatement;

class Assignment;
class VarAssignment;
class ArrayAssignment;

/*Lists*/
class Statements;
class ExpList;

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


class Program
{
    public:
    MainClass* mc;
    std::list<ClassDecl*>* cd;

    void generateGraphViz();

    Program(MainClass*, std::list<ClassDecl*>*);
};

#endif /*TREE_H*/
