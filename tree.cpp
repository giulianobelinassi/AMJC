#include <stdio.h>
#include <string.h>

#include "tree.h"


#define COPY_STR(dest, src)  do {\
    size_t len = strlen(src); \
    dest = new char[len]; \
    strncpy((char*) dest, src, len); } \
while(0)

Program::Program(MainClass* mc, ClassDecls* cd)
{
    this->mc = mc;
    this->cd = cd;
}

MainClass::MainClass(Statement* stmt)
{
    this->stmt = stmt;
}

ClassDecl::ClassDecl(VarDecls* vars, MethodDecls* decls)
{
    this->vars = vars;
    this->decls = decls;
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

VarDecl::VarDecl(Type* type, const char* id)
{
    this->type = type;
    COPY_STR(this->id, id);
}

VarDecls::VarDecls(VarDecls* decls, VarDecl* decl)
{
    this->decls = decls;
    this->decl = decl;
}

MethodDecl::MethodDecl(Type* type, FormalList* formals, VarDecls* decls, Statements* stmts)
{
    this->type = type;
    this->formals = formals;
    this->decls = decls;
    this->stmts = stmts;
}

MethodDecls::MethodDecls(MethodDecls* decls, MethodDecl* decl)
{
    this->decls = decls;
    this->decl = decl;
}

FormalList::FormalList(Type* type, const char* id, FormalList* rest)
{
    this->type = type;
    COPY_STR(this->id, id);
    this->rest = rest;
}

VarAssignment::VarAssignment(const char* id, Expression* exp)
{
    COPY_STR(this->id, id);
    this->exp = exp;
}

ArrayAssignment::ArrayAssignment(const char* id, Expression* exp1, Expression* exp2)
{
    COPY_STR(this->id, id);
    this->exp1 = exp1;
    this->exp2 = exp2;
}

Statements::Statements(Statements* rest, Statement* stmt)
{
    this->rest = rest;
    this->stmt = stmt;
}

BracedStatement::BracedStatement(Statements* stmts)
{
    this->stmts = stmts;
}

IfElseStatement::IfElseStatement(Expression* cndexp, Statement* ifstmt, Statement* elsestmt)
{
    this->cndexp = cndexp;
    this->ifstmt = ifstmt;
    this->elsestmt = elsestmt;
}

WhileStatement::WhileStatement(Expression* cndexp, Statement* whilestmt)
{
    this->cndexp = cndexp;
    this->whilestmt = whilestmt;
}

PrintStatement::PrintStatement(Expression* exp)
{
    this->exp = exp;    
}

OpExpression::OpExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
}

LengthExpression::LengthExpression(Expression* exp)
{
    this->exp = exp;
}

MethodExpression::MethodExpression(Expression* exp, const char* id, ExpList* explist)
{
    this->exp = exp;
    COPY_STR(this->id, id);
    explist = explist;
}

NewIntArrExpression::NewIntArrExpression(Expression* exp)
{
    this->exp = exp;
}

NewMethodExpression::NewMethodExpression(const char* id)
{
    COPY_STR(this->id, id);
}

NegateExpression::NegateExpression(Expression* exp)
{
    this->exp = exp;
}

TokenExpression::TokenExpression(const char* token)
{
    COPY_STR(this->token, token);
}

ParenExpression::ParenExpression(Expression* exp)
{
    this->exp = exp;
}

BrcktExpression::BrcktExpression(Expression* exp1, Expression* exp2)
{
    this->exp1 = exp1;
    this->exp2 = exp2;
}

ExpList::ExpList(Expression* exp, ExpList* rest)
{
    this->exp  = exp;
    this->rest = rest;
}
