
//#include "interp.h"
#include "tree.h"
#include "expression.h"
#include "statement.h"
#include "declaration.h"
#include "macros.h"
#include "symboltable.h"
#include "compiler.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <graphviz/gvc.h>

Program::Program(MainClass* mc, std::list<ClassDecl*>* cd)
{
    this->mc = mc;
    this->cd = cd;
}

struct interp_ret Program::interp()
{
    SymbolTable* st = new SymbolTable();

    std::list<ClassDecl*>::iterator it;

    for (it = cd->begin(); it != cd->end(); it++)
        Type::declareType((*it)->name->token, *it);

    return mc->interp(st);
}

struct interp_ret MainClass::interp(SymbolTable* st)
{
    return stmt->interp(st);
}

void Program::compile()
{
    struct compiler_args args;

    SymbolTable* st = new SymbolTable();

    std::list<ClassDecl*>::iterator it;

    for (it = cd->begin(); it != cd->end(); it++)
        Type::declareType((*it)->name->token, *it);

    for (it = cd->begin(); it != cd->end(); it++)
        (*it)->compile();

    mc->compile(args);

}

struct compiler_ret MainClass::compile(struct compiler_args args)
{
    std::cout << "segment .text" << std::endl;
    std::cout << "global _start" << std::endl;
    std::cout << "_start:" << std::endl;
}

void Program::generateGraphViz()
{
    Agraph_t* g;
    Agnode_t* v1, * prog;
    GVC_t* gvc = gvContext();
    FILE* pdf_file = fopen("graph.pdf", "w");
    std::list<ClassDecl*>::iterator it;

    g = agopen((char*) "g", Agdirected, 0);

    prog = agnode(g, NULL, 1);
    agset(prog, (char*) "label", (char*) "Program");

    v1 = mc->buildGVNode(g);
    agedge(g, prog, v1, 0, 1);

    for (it = cd->begin(); it != cd->end(); ++it)
    {
        Agnode_t* _c1 = (*it)->buildGVNode(g);
        agedge(g, prog, _c1, 0, 1);
    }

    gvLayout(gvc, g, "dot");
    gvRender(gvc, g, "pdf", pdf_file);
    gvFreeLayout(gvc, g);

    agclose(g);

    gvFreeContext(gvc);
}

MainClass::MainClass(Statement* stmt)
{
    this->stmt = stmt;
}

Agnode_t* MainClass::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "MainClass", stmt);

    return v;
}

