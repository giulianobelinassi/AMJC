%{
#include <stdio.h>
#include <string.h>
#include <list>

#include "tree.h"
#include "expression.h"
#include "statement.h"
#include "declaration.h"
#include "types.h"

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
%token PLUS       
%token MINUS      
%token TIMES      
%token SLASH      
%token LPAREN     
%token RPAREN     
%token SEMICOLON  
%token COMMA      
%token PERIOD     
%token ASSIGNMENT 
%token EQ         
%token NEQ        
%token LSS        
%token GTR        
%token LEQ        
%token GEQ        
%token NEG        
%token XOR        
%token AND        
%token OR         
%token FORSYM     
%token IFSYM      
%token ELSESYM    
%token DOSYM      
%token WHILESYM   
%token IDENT
%token NUMBER
%token PRINTSYM
%token RETURNSYM

%precedence IDENT

/*Makes yyparse accept a parameter*/
%parse-param { Program** program }

%union{
    Expression* exp;
    TokenExpression* ident;
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
%type <str> NUMBER
%type <exp> ExpRest
%type <explist> ExpRests
%type <stmt> Statement
%type <stmts> Statements
%type <stmt> NonAssignStmt
%type <stmt> Assignment
%type <type> Type
%type <type> PrimitiveType
%type <type> CustomType
%type <vardecls> FormalList
%type <vardecls> FormalRest
%type <methoddecls> MethodDecls
%type <methoddecl> MethodDecl
%type <vardecls> VarDecls
%type <vardecl> VarDecl
%type <classdecl> ClassDecl
%type <classdecls> ClassDecls
%type <mainclass> MainClass
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
          /*{ $$ = new ClassDecl(new TokenExpression(strtok($I, "{")), $L, $R); }*/
          { $$ = new ClassDecl(new TokenExpression($I), $L, $R); }
    | CLASSSYM IDENT[I] EXTENDSYM IDENT OPENBRACE VarDecls[L] MethodDecls[R] 
        CLOSEBRACE 
          { $$ = new ClassDecl(new TokenExpression(strtok($I, "{")), $L, $R); }
    ;

ClassDecls:
      /*Epsilon*/ 
          { $$ = new std::list<ClassDecl*>(); }
    | ClassDecls[L] ClassDecl[R] 
          { ($L)->push_back($R); $$ = $L; }
    ;

VarDecl:
      Type[L] IDENT[R] SEMICOLON 
          { $$ = new VarDecl($L, new TokenExpression(strtok($R, ";"))); }
    ;

VarDecls:
      /*Epsilon*/ 
          { $$ = new std::list<VarDecl*>(); }
    | VarDecls[L] VarDecl[R] 
          { ($L)->push_back($R); $$ = $L; }
    ;

MethodDecl:
      PUBLICSYM Type[T] IDENT LPAREN FormalList[FL] RPAREN 
        OPENBRACE VarDecls[VD] Statements[STMTS] RETURNSYM Exp[E] SEMICOLON CLOSEBRACE 
        { $$ = new MethodDecl($T, $FL, $VD, $STMTS, $E); }
    ;

MethodDecls:
      /*Epsilon*/ { $$ = new std::list<MethodDecl*>(); }
    | MethodDecls[L] MethodDecl[R] { ($L)->push_back($R); $$ = $L; }
    ;


FormalList:
    /*Epsilon*/ { $$ = new std::list<VarDecl*>(); }
    | FormalRest[L] Type[C] IDENT[R] 
        { ($L)->push_back(new VarDecl($C, new TokenExpression($R))); $$ = $L; }
    ;

FormalRest:
      /*Epsilon*/ { $$ = new std::list<VarDecl*>(); }
    | FormalRest[L] COMMA Type[C] IDENT[R] 
        { ($L)->push_back(new VarDecl($C, new TokenExpression($R))); $$ = $L; }
    ;

Type:
      CustomType[L] { $$ = $L; }
    | PrimitiveType[L] { $$ = $L; }
    ;

CustomType:
      IDENT { $$ = Type::getDeclaredType($1);}
    ;

PrimitiveType:
      INTSYM OPENBRKT CLOSEBRKT { $$ = Type::getTypeFromStr("int[]");}
    | BOOLEANSYM { $$ = Type::getTypeFromStr("boolean");}
    | INTSYM { $$ = Type::getTypeFromStr("int");}
    ;

Assignment:
      IDENT[L] ASSIGNMENT Exp[R] SEMICOLON 
        { $$ = new VarAssignment(new TokenExpression(strtok($L, "=")), $R); }
    | IDENT[L] OPENBRKT Exp[C] CLOSEBRKT ASSIGNMENT Exp[R] SEMICOLON 
        { $$ = new ArrayAssignment(new TokenExpression(strtok($L, "{")), $C, $R);}
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
      Exp[L] Op Exp[R]                                { $$ = new OpExpression($L, $R); }
    | Exp[L] OPENBRKT Exp[R] CLOSEBRKT                { $$ = new BrcktExpression($L, $R); }
    | Exp[L] PERIOD LENGTHSYM                         { $$ = new LengthExpression($L);}
    | Exp[L] PERIOD IDENT[C] LPAREN ExpList[R] RPAREN { $$ = new MethodExpression($L, new TokenExpression(strtok($C, "(")), $R);}
    | NUMBER[C]                                       { $$ = new TokenExpression($C); }
    | TRUESYM                                         { $$ = new TokenExpression("True"); }
    | FALSESYM                                        { $$ = new TokenExpression("False"); }
    | IDENT[C]                                        { $$ = new TokenExpression($C); }
    | THISSYM                                         { $$ = new TokenExpression("this"); }
    | NEWSYM INTSYM OPENBRKT Exp[L] CLOSEBRKT         { $$ = new NewIntArrExpression($L); }
    | NEWSYM IDENT[C] LPAREN RPAREN                   { $$ = new NewMethodExpression(new TokenExpression(strtok($C, "("))); }
    | NEG Exp[L]                                      { $$ = new NegateExpression($L); }
    | LPAREN Exp[L] RPAREN                            { $$ = new ParenExpression($L);}
    ;

Op:
     PLUS  {}
   | MINUS {}
   | TIMES {}
   | SLASH {}
   | GTR {}
   | GEQ {}
   | EQ {}
   | LSS {}
   | LEQ {}
   | NEQ {}
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
    Program* program;
    yyin = stdin;
    yyparse(&program);
    program->generateGraphViz();
    Type::printDeclaredTypes();
    return 0;
}

void yyerror(Program** p, const char *s) {
	printf ("Error!! %s\n", s);
}
