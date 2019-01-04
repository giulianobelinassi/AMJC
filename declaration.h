#ifndef DECLARATION_H
#define DECLARATION_H

#include <list>

#include "symboltable.h"
#include "tree.h"

class VarDecl
{
    public:
    Type* type;
    VarIdExpression* id;

    struct Agnode_s* buildGVNode(struct Agraph_s*);

    VarDecl(Type*, VarIdExpression*);
};

class MethodDecl
{
    public:
    Type* type;
    VarIdExpression* id;
    std::list<VarDecl*>* formals;
    std::list<VarDecl*>* decls;
    std::list<Statement*>* stmts;
    Expression* exp;

    struct Agnode_s* buildGVNode(struct Agraph_s*);
    int calculateFormalSize();
    int calculateLocalSize();

    MethodDecl(Type*, VarIdExpression*, std::list<VarDecl*>*,
               std::list<VarDecl*>*, std::list<Statement*>*, Expression* exp);
};

class ClassDecl
{
    public:
    VarIdExpression* name;
    std::list<VarDecl*>* vars;
    std::list<MethodDecl*>* decls;
    SymbolTable* compiled_table;

    struct Agnode_s* buildGVNode(struct Agraph_s* g);
    struct compiler_ret compile();

    ClassDecl(VarIdExpression*, std::list<VarDecl*>*, std::list<MethodDecl*>*);
};
#endif
