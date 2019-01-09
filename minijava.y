%{
#include <stdio.h>
#include <string.h>
#include <list>

#include "tree.h"
#include "expression.h"
#include "statement.h"
#include "declaration.h"
#include "types.h"

#define YYERROR_VERBOSE

extern "C" {
    extern int yylex();
    extern int yyparse(Program**);
    extern FILE *yyin;
}

void yyerror(Program** p, const char *s);
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:

%token CLASSSYM
%token PUBLICSYM
%token STATICSYM
%token VOIDSYM
%token MAINSYM
%token EXTENDSYM
%token INTSYM
%token BOOLEANSYM
%token STRINGSYM
%token LENGTHSYM
%token THISSYM
%token TRUESYM
%token FALSESYM
%token NEWSYM
%token OPENBRKT
%token CLOSEBRKT
%token OPENBRACE
%token CLOSEBRACE
%left  PLUS MINUS
%left TIMES SLASH
%token LPAREN
%token RPAREN
%token SEMICOLON
%token COMMA
%token PERIOD
%token ASSIGNMENT
%left  EQ NEQ LSS GTR LEQ GEQ XOR AND OR
%left  NEG
%token FORSYM
%token IFSYM
%token ELSESYM
%token DOSYM
%token WHILESYM
%token IDENT
%token TYPENAME
%token NUMBER
%token PRINTSYM
%token RETURNSYM

/*Makes yyparse accept a parameter*/
%parse-param { Program** program }

%union{
    Expression* exp;
    VarIdExpression* ident;
    Statement* stmt;
    Type* type;
    MethodDecl* methoddecl;
    VarDecl* vardecl;
    ClassDecl* classdecl;
    MainClass* mainclass;

    /*Lists*/
    std::list<ClassDecl* >* classdecls;
    std::list<MethodDecl*>* methoddecls;
    std::list<VarDecl*   >* vardecls;
    std::list<Statement* >* stmts;
    std::list<Expression*>* explist;

    char* str;
}

%type <exp> Exp
%type <explist> ExpList
%type <str> IDENT
%type <str> TYPENAME
%type <str> NUMBER
%type <exp> ExpRest
%type <explist> ExpRests
%type <stmt> Statement
%type <stmts> Statements
%type <stmt> NonAssignStmt
%type <stmt> Assignment
%type <type> Type
/*%type <type> PrimitiveType */
/*%type <type> CustomType */
%type <vardecls> FormalList
%type <vardecls> FormalRest
%type <methoddecls> MethodDecls
%type <methoddecl> MethodDecl
%type <vardecls> VarDecls
%type <vardecl> VarDecl
%type <classdecl> ClassDecl
%type <classdecls> ClassDecls
%type <mainclass> MainClass

/*%define parse.trace*/
%%

Program:
      MainClass[L] ClassDecls[R] { *program = new Program($L, $R);}
    ;

MainClass:
      CLASSSYM IDENT OPENBRACE PUBLICSYM STATICSYM VOIDSYM MAINSYM LPAREN
        STRINGSYM OPENBRKT CLOSEBRKT IDENT RPAREN OPENBRACE Statement[L]
        CLOSEBRACE CLOSEBRACE
          { $$ = new MainClass($L); }
    ;

ClassDecl:
      CLASSSYM IDENT[I] OPENBRACE VarDecls[L] MethodDecls[R] CLOSEBRACE
          { $$ = new ClassDecl(new VarIdExpression(strtok($I, " {")), $L, $R); }
    | CLASSSYM IDENT[I] EXTENDSYM IDENT OPENBRACE VarDecls[L] MethodDecls[R]
        CLOSEBRACE
          { $$ = new ClassDecl(new VarIdExpression(strtok($I, " {")), $L, $R); }
    ;

ClassDecls:
      /*Epsilon*/
          { $$ = new std::list<ClassDecl*>(); }
    | ClassDecls[L] ClassDecl[R]
          { ($L)->push_back($R); $$ = $L; }
    ;

VarDecl:
      Type[L] IDENT[R] SEMICOLON
          { $$ = new VarDecl($L, new VarIdExpression(strtok($R, " ;"))); }
    ;

VarDecls:
      /*Epsilon*/
          { $$ = new std::list<VarDecl*>(); }
    | VarDecls[L] VarDecl[R]
          { ($L)->push_back($R); $$ = $L; }
    ;

MethodDecl:
      PUBLICSYM Type[T] IDENT[I] LPAREN FormalList[FL] RPAREN
        OPENBRACE VarDecls[VD] Statements[STMTS] RETURNSYM Exp[E] SEMICOLON CLOSEBRACE
        { $$ = new MethodDecl($T, new VarIdExpression(strtok($I, " (")), $FL, $VD, $STMTS, $E); }
    ;

MethodDecls:
      /*Epsilon*/ { $$ = new std::list<MethodDecl*>(); }
    | MethodDecls[L] MethodDecl[R] { ($L)->push_back($R); $$ = $L; }
    ;


FormalList:
    /*Epsilon*/ { $$ = new std::list<VarDecl*>(); }
    | FormalRest[L] Type[C] IDENT[R]
        { ($L)->push_back(new VarDecl($C, new VarIdExpression(strtok($R, " ,)")))); $$ = $L; }
    ;

FormalRest:
      /*Epsilon*/ { $$ = new std::list<VarDecl*>(); }
    | FormalRest[L] COMMA Type[C] IDENT[R]
        { ($L)->push_back(new VarDecl($C, new VarIdExpression(strtok($R, " ,)")))); $$ = $L; }
    ;

Type:
      INTSYM OPENBRKT CLOSEBRKT { $$ = Type::getTypeFromStr("int[]");}
    | BOOLEANSYM { $$ = Type::getTypeFromStr("boolean");}
    | INTSYM { $$ = Type::getTypeFromStr("int");}
    | TYPENAME { $$ = Type::getDeclaredType($1);}
    ;
/*
CustomType:
      IDENT { $$ = Type::getDeclaredType($1);}
    ;
*/
/*
PrimitiveType:
      INTSYM OPENBRKT CLOSEBRKT { $$ = Type::getTypeFromStr("int[]");}
    | BOOLEANSYM { $$ = Type::getTypeFromStr("boolean");}
    | INTSYM { $$ = Type::getTypeFromStr("int");}
    ;
*/
Assignment:
      IDENT[L] ASSIGNMENT Exp[R] SEMICOLON 
        { $$ = new VarAssignment(new VarIdExpression(strtok($L, " =")), $R); }
    | IDENT[L] OPENBRKT Exp[C] CLOSEBRKT ASSIGNMENT Exp[R] SEMICOLON 
        { $$ = new ArrayAssignment(new VarIdExpression(strtok($L, " [")), $C, $R);}
    ;

NonAssignStmt:
      OPENBRACE Statements[L] CLOSEBRACE { $$ = new BracedStatement($L);}
    | IFSYM LPAREN Exp[L] RPAREN Statement[C] ELSESYM Statement[R] 
        { $$ = new IfElseStatement($L, $C, $R);}
    | WHILESYM LPAREN Exp[L] RPAREN Statement[R] 
        { $$ = new WhileStatement($L, $R); }
    | PRINTSYM LPAREN Exp[L] RPAREN SEMICOLON 
        { $$ = new PrintStatement($L); }
    ;

Statement:
      NonAssignStmt[L] { $$ = $L; }
    | Assignment[L] { $$ = $L; }
    ;

Statements:
    /*Epsilon*/ { $$ = new std::list<Statement*>(); }
    | Statements[L] Statement[R] { ($L)->push_back($R); $$ = $L; }
    ;

Exp:
      Exp[L] PLUS Exp[R]                              { $$ = new OpExpression($L, OP_PLUS, $R); }
    | Exp[L] MINUS Exp[R]                             { $$ = new OpExpression($L, OP_MINUS, $R); }
    | Exp[L] TIMES Exp[R]                             { $$ = new OpExpression($L, OP_TIMES, $R); }
    | Exp[L] SLASH Exp[R]                             { $$ = new OpExpression($L, OP_DIV, $R); }
    | Exp[L] GTR Exp[R]                               { $$ = new OpExpression($L, OP_GT, $R); }
    | Exp[L] GEQ Exp[R]                               { $$ = new OpExpression($L, OP_GE, $R); }
    | Exp[L] EQ Exp[R]                                { $$ = new OpExpression($L, OP_EQ, $R); }
    | Exp[L] LSS Exp[R]                               { $$ = new OpExpression($L, OP_LT, $R); }
    | Exp[L] LEQ Exp[R]                               { $$ = new OpExpression($L, OP_LE, $R); }
    | Exp[L] NEQ Exp[R]                               { $$ = new OpExpression($L, OP_NE, $R); }
    | Exp[L] OPENBRKT Exp[R] CLOSEBRKT                { $$ = new BrcktExpression($L, $R); }
    | Exp[L] PERIOD LENGTHSYM                         { $$ = new LengthExpression($L);}
    | Exp[L] PERIOD IDENT[C] LPAREN ExpList[R] RPAREN { $$ = new MethodExpression($L, new VarIdExpression(strtok($C, " (")), $R);}
    | NUMBER[C]                                       { $$ = new NumExpression($C); }
    | TRUESYM                                         { $$ = new BoolExpression(false); }
    | FALSESYM                                        { $$ = new BoolExpression(true); }
    | IDENT[C]                                        { $$ = new VarIdExpression($C); }
    | THISSYM                                         { $$ = new VarIdExpression("this"); }
    | NEWSYM INTSYM OPENBRKT Exp[L] CLOSEBRKT         { $$ = new NewIntArrExpression($L); }
    | NEWSYM IDENT[C] LPAREN RPAREN                   { $$ = new NewMethodExpression(new VarIdExpression(strtok($C, " ("))); }
    | NEG Exp[L]                                      { $$ = new NegateExpression($L); }
    | LPAREN Exp[L] RPAREN                            { $$ = new ParenExpression($L);}
    ;

ExpList:
      ExpRests[L] Exp[R] { ($L)->push_back($R); $$ = $L; }
    ;

ExpRests:
      /*eps*/ { $$ = new std::list<Expression*>(); }
    | ExpRests[L] ExpRest[R] { ($L)->push_back($R); $$ = $L; }
    ;

ExpRest:
    COMMA Exp[L] { $$ = $L; }
    ;

%%
int main(){
    //yydebug = 1;
    Program* program;
    yyin = stdin;
    yyparse(&program);
    //program->generateGraphViz();
    //Type::printDeclaredTypes();
    //program->interp();

    program->compile();
    return 0;
}

void yyerror(Program** p, const char *s) {
	printf ("Error: %s\n", s);
}
