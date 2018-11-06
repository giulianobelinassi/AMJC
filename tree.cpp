#include <stdio.h>
#include <string.h>

#include <graphviz/gvc.h>

#include "tree.h"
#include "macros.h"


Program::Program(MainClass* mc, ClassDecls* cd)
{
    this->mc = mc;
    this->cd = cd;
}

void Program::generateGraphViz()
{
    Agraph_t* g;
    Agnode_t* v1, * v2, * prog;
    GVC_t* gvc = gvContext();
    FILE* pdf_file = fopen("graph.pdf", "w");

    g = agopen((char*) "g", Agdirected, 0);
   
    prog = agnode(g, NULL, 1);
    agset(prog, (char*) "label", (char*) "Program");

    v1 = mc->buildGVNode(g);
    v2 = cd->buildGVNode(g);

    agedge(g, prog, v1, 0, 1);
    agedge(g, prog, v2, 0, 1);

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

ClassDecl::ClassDecl(TokenExpression* name, VarDecls* vars, MethodDecls* decls)
{
    this->name = name;
    this->vars = vars;
    this->decls = decls;
}

Agnode_t* ClassDecl::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "ClassDecl", name, vars, decls);

    return v;
}

Agnode_t* ClassDecls::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "ClassDecls", decls, decl);
    
    return v;
}

ClassDecls::ClassDecls(ClassDecls* decls, ClassDecl* decl)
{
    this->decls = decls;
    this->decl  = decl;
}

VarDecl::VarDecl(Type* type, TokenExpression* id)
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

VarDecls::VarDecls(VarDecls* decls, VarDecl* decl)
{
    this->decls = decls;
    this->decl = decl;
}

Agnode_t* VarDecls::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "VarDecls", decls, decl);
    
    return v;
}

MethodDecl::MethodDecl(Type* type, FormalList* formals, VarDecls* decls, Statements* stmts, Expression* exp)
{
    this->type = type;
    this->formals = formals;
    this->decls = decls;
    this->stmts = stmts;
    this->exp = exp;
}

Agnode_t* MethodDecl::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    FIVE_CHILD_VERTEX(v, "MethodDecl", type, formals, decls, stmts, exp);
    
    return v;
}

MethodDecls::MethodDecls(MethodDecls* decls, MethodDecl* decl)
{
    this->decls = decls;
    this->decl = decl;
}

Agnode_t* MethodDecls::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "MethodDecls", decls, decl);
    
    return v;
}

FormalList::FormalList(Type* type, TokenExpression* id, FormalList* rest)
{
    this->type = type;
    this->id = id;
    this->rest = rest;
}

Agnode_t* FormalList::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "FormalList", type, id, rest);
    
    return v;
}

VarAssignment::VarAssignment(TokenExpression* id, Expression* exp)
{
    this->id = id;
    this->exp = exp;
}

Agnode_t* VarAssignment::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "VarAssignment", id, exp);
    
    return v;
}

ArrayAssignment::ArrayAssignment(TokenExpression* id, Expression* exp1, Expression* exp2)
{
    this->id = id;
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Agnode_t* ArrayAssignment::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "ArrayAssignment", id, exp1, exp2);
    
    return v;
}
