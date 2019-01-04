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
    Type* t;
    this->name = name;
    this->vars = vars;
    this->decls = decls;
    t = Type::declareType(name->token, this);
    compiled_table = new SymbolTable(t, this);
    std::cout << "COMPILED_TABLE::" << std::endl;
    compiled_table->printTable();

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

int MethodDecl::calculateFormalSize()
{
    std::list<VarDecl*>::iterator it;
    int size = 0;

    for (it = formals->begin(); it != formals->end(); ++it)
       size += 4; //Assume all types are pointers or integers.

    return size + 4; //pointer to this
}

int MethodDecl::calculateLocalSize()
{
    std::list<VarDecl*>::iterator it;
    int size = 0;

    for (it = decls->begin(); it != decls->end(); ++it)
       size += 4; //Assume all types are pointers or integers.

    return size;
}

struct compiler_ret ClassDecl::compile()
{
    std::list<MethodDecl*>::iterator it;
    std::list<Statement*>::iterator stmt_it;
    for (it = decls->begin(); it != decls->end(); it++)
    {
        SymbolTable* st = new SymbolTable(compiled_table);
        st->parseVars((*it)->formals, false);
        st->parseVars((*it)->decls, true);

        std::cout << "FRAME_TBL" << std::endl;
        st->printTable();

        std::cout << name->token << "__" << (*it)->id->token << ":" << std::endl;
        std::cout << "push ebp" << std::endl;
        std::cout << "mov ebp, esp" << std::endl;
        std::cout << "sub esp, " << (*it)->calculateLocalSize() << std::endl;

        for (stmt_it = (*it)->stmts->begin(); stmt_it != (*it)->stmts->end(); stmt_it++)
        {
            (*stmt_it)->compile(st);
        }

        std::cout << "mov esp, ebp" << std::endl;
        std::cout << "pop ebp" << std::endl;
        std::cout << "ret " << (*it)->calculateFormalSize() << std::endl;
    }
}
