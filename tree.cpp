#include <stdio.h>
#include <string.h>

#include <graphviz/gvc.h>

#include "tree.h"


#define COPY_STR(dest, src)  do {\
    size_t len = strlen(src); \
    dest = new char[len]; \
    strncpy((char*) dest, src, len); } \
while(0)

#define ZERO_CHILD_VERTEX(v, name) do { \
    v  = agnode(g, (char*) name, 1); \
} while(0)

#define ONE_CHILD_VERTEX(v, name, c1) do { \
    v  = agnode(g, (char*) name, 1); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1); \
    agedge(g, v, _c1, 0, 1); \
} while(0)

#define TWO_CHILD_VERTEX(v, name, c1, c2) do { \
    v  = agnode(g, (char*) name, 1); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c2 = c2? c2->buildGVNode(g): agnode(g, (char*) "NULL", 1);  \
    agedge(g, v, _c1, 0, 1); \
    agedge(g, v, _c2, 0, 1); \
} while(0)

#define THREE_CHILD_VERTEX(v, name, c1, c2, c3) do { \
    v  = agnode(g, (char*) name, 1); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c2 = c2? c2->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c3 = c3? c3->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    agedge(g, v, _c1, 0, 1); \
    agedge(g, v, _c2, 0, 1); \
    agedge(g, v, _c3, 0, 1); \
} while(0)

#define FIVE_CHILD_VERTEX(v, name, c1, c2, c3, c4, c5) do { \
    v  = agnode(g, (char*) name, 1); \
    Agnode_t* _c1 = c1? c1->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c2 = c2? c2->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c3 = c3? c3->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c4 = c4? c4->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    Agnode_t* _c5 = c5? c5->buildGVNode(g): agnode(g, (char*) "NULL", 1);\
    agedge(g, v, _c1, 0, 1); \
    agedge(g, v, _c2, 0, 1); \
    agedge(g, v, _c3, 0, 1); \
    agedge(g, v, _c4, 0, 1); \
    agedge(g, v, _c5, 0, 1); \
} while(0)

Program::Program(MainClass* mc, ClassDecls* cd)
{
    this->mc = mc;
    this->cd = cd;
}

void Program::generateGraphViz()
{
    Agraph_t* g;
    Agnode_t* v1, * v2, * prog;
    Agedge_t* e1, * e2;
    GVC_t* gvc = gvContext();
    const char* dot = "dot";

    gvParseArgs(gvc, 1, (char**) &dot);

    g = agopen((char*) "g", Agdirected, 0);
   
    prog = agnode(g, (char*) "Program", 1);

    v1 = mc->buildGVNode(g);
    v2 = cd->buildGVNode(g);

    e1 = agedge(g, prog, v1, 0, 1);
    e2 = agedge(g, prog, v2, 0, 1);

    gvLayoutJobs(gvc, g);
    gvRenderJobs(gvc, g);
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

ClassDecl::ClassDecl(VarDecls* vars, MethodDecls* decls)
{
    this->vars = vars;
    this->decls = decls;
}

Agnode_t* ClassDecl::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "ClassDecl", vars, decls);

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

Type::Type(const char* id)
{
    COPY_STR(this->id, id);
}

Agnode_t* Type::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, id);
    
    return v;
}

VarDecl::VarDecl(Type* type, const char* id)
{
    this->type = type;
    COPY_STR(this->id, id);
}

Agnode_t* VarDecl::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "VarDecl", type);
    
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
    TWO_CHILD_VERTEX(v, "FormalList", type, rest);
    
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

Statements::Statements(Statements* rest, Statement* stmt)
{
    this->rest = rest;
    this->stmt = stmt;
}

Agnode_t* Statements::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "Statements", rest, stmt);
    
    return v;
}

BracedStatement::BracedStatement(Statements* stmts)
{
    this->stmts = stmts;
}

Agnode_t* BracedStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "BracedStatement", stmts);
    
    return v;
}

IfElseStatement::IfElseStatement(Expression* cndexp, Statement* ifstmt, Statement* elsestmt)
{
    this->cndexp = cndexp;
    this->ifstmt = ifstmt;
    this->elsestmt = elsestmt;
}

Agnode_t* IfElseStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "ArrayAssignment", cndexp, ifstmt, elsestmt);
    
    return v;
}

WhileStatement::WhileStatement(Expression* cndexp, Statement* whilestmt)
{
    this->cndexp = cndexp;
    this->whilestmt = whilestmt;
}

Agnode_t* WhileStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "WhileStatement", cndexp, whilestmt);
    
    return v;
}

PrintStatement::PrintStatement(Expression* exp)
{
    this->exp = exp;    
}

Agnode_t* PrintStatement::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "PrintStatement", exp);
    
    return v;
}

OpExpression::OpExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Agnode_t* OpExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "OpExpression", exp1, exp2);
    
    return v;
}

LengthExpression::LengthExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* LengthExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "LengthExpression", exp);
    
    return v;
}

MethodExpression::MethodExpression(Expression* exp, TokenExpression* id, ExpList* explist)
{
    this->exp = exp;
    this->id = id;
    this->explist = explist;
}

Agnode_t* MethodExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    THREE_CHILD_VERTEX(v, "MethodExpression", exp, id, explist);
    
    return v;
}

NewIntArrExpression::NewIntArrExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* NewIntArrExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NewIntArrExpression", exp);
    
    return v;
}

NewMethodExpression::NewMethodExpression(TokenExpression* id)
{
    this->id = id;
}

Agnode_t* NewMethodExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NewMethodExpression", id);
    
    return v;
}

NegateExpression::NegateExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* NegateExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "NegateExpression", exp);
    
    return v;
}

TokenExpression::TokenExpression(const char* token)
{
    COPY_STR(this->token, token);
}

Agnode_t* TokenExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ZERO_CHILD_VERTEX(v, "TokenExpression");
    
    return v;
}

ParenExpression::ParenExpression(Expression* exp)
{
    this->exp = exp;
}

Agnode_t* ParenExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    ONE_CHILD_VERTEX(v, "ParenExpression", exp);
    
    return v;
}

BrcktExpression::BrcktExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Agnode_t* BrcktExpression::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "BrcktExpression", exp1, exp2);
    
    return v;
}

ExpList::ExpList(Expression* exp, ExpList* rest)
{
    this->exp  = exp;
    this->rest = rest;
}

Agnode_t* ExpList::buildGVNode(Agraph_t* g)
{
    Agnode_t* v;
    TWO_CHILD_VERTEX(v, "ExpList", exp, rest);
    
    return v;
}
