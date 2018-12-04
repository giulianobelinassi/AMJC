
//#include "interp.h"
#include "tree.h"
#include "expression.h"
#include "statement.h"
#include "declaration.h"
#include "macros.h"
#include "symboltable.h"


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
    struct interp_ret ret;
    return ret;
//    return mc->interp();
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
    
    EXPAND_LIST_VERTEX(prog, it, "ClassDecls", cd);

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

