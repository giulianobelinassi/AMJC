#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <list>

#include "types.h"
#include "interp.h"
#include "compiler.h"

/* Used GraphViz structures*/
struct Agraph_s;
struct Agnode_s;

/*Expressions*/
class Expression;
class VarIdExpression;
class OpExpression;
class LengthExpression;
class MethodExpression;
class NewIntArrExpression;
class NewMethodExpression;
class NegateExpression;
class ParenExpression;
class BrcktExpression;
class BoolExpression;
class NumExpression;

/*Statements*/
class Statement;
class BracedStatement;
class IfElseStatement;
class WhileStatement;
class PrintStatement;

class Assignment;
class VarAssignment;
class ArrayAssignment;

/*Declarations*/
class VarDecl;
class MethodDecl;
class ClassDecl;

/*Miscellaneous*/
class MainClass
{
    public:
    Statement* stmt;

    struct Agnode_s* buildGVNode(struct Agraph_s*);
    struct interp_ret interp(SymbolTable*);
    struct compiler_ret compile(struct compiler_args);

    MainClass(Statement*);
};

class Program
{
    public:
    MainClass* mc;
    std::list<ClassDecl*>* cd;

    void generateGraphViz();

    struct interp_ret interp();
    struct interp_ret interp(SymbolTable*);
    void compile(void);

    Program(MainClass*, std::list<ClassDecl*>*);
};

#endif /*TREE_H*/
