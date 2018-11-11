#include <graphviz/cgraph.h>
#include <iostream>

#include "tree.h"
#include "declaration.h"
#include "expression.h"
#include "statement.h"
#include "macros.h"
#include "symboltable.h"

ClassDecl::ClassDecl(VarIdExpression* name, std::list<VarDecl*>* vars, 
                     std::list<MethodDecl*>* decls)
{
    this->name = name;
    this->vars = vars;
    this->decls = decls;
    Type::declareType(name->token, this);    
}

Agnode_t* ClassDecl::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    std::list<VarDecl*>::iterator var_it;
    std::list<MethodDecl*>::iterator method_it;

    ONE_CHILD_VERTEX(v, "ClassDecls", name);
    EXPAND_LIST_VERTEX(v, var_it, "VarDecls",  vars);
    EXPAND_LIST_VERTEX(v, method_it, "MethodDecls",  decls);

    return v;
}


VarDecl::VarDecl(Type* type, VarIdExpression* id)
{
    this->type = type;
    this->id = id;
}

Agnode_t* VarDecl::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "VarDecl", type, id);
    
    return v;
}

MethodDecl::MethodDecl(Type* type, VarIdExpression* id, 
                       std::list<VarDecl*>* formals, 
                       std::list<VarDecl*>* decls, 
                       std::list<Statement*>* stmts, Expression* exp)
{
    this->type = type;
    this->id   = id;
    this->formals = formals;
    this->decls = decls;
    this->stmts = stmts;
    this->exp = exp;
}

Agnode_t* MethodDecl::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    std::list<VarDecl*>::iterator var_it;
    std::list<Statement*>::iterator stmt_it;

    TWO_CHILD_VERTEX(v, "MethodDecl", type, exp);
    EXPAND_LIST_VERTEX(v, var_it, "FormalDecls",  formals);
    EXPAND_LIST_VERTEX(v, var_it, "VarDecls",  decls);
    EXPAND_LIST_VERTEX(v, stmt_it, "Statements", stmts);
    
    return v;
}

