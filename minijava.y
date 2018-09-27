%{
#include <stdio.h>

// stuff from flex that bison needs to know about:

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

extern int yylex();
extern int yyparse();
extern FILE *yyin;

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#include "tree.h"

void yyerror(const char *s);
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

%%

Program:
      MainClass ClassDecls {printf("MainClass ClassDecls\n");}
    ;

MainClass:
      CLASSSYM IDENT OPENBRACE PUBLICSYM STATICSYM VOIDSYM MAINSYM LPAREN STRINGSYM OPENBRKT CLOSEBRKT 
      IDENT RPAREN OPENBRACE Statement CLOSEBRACE CLOSEBRACE {printf("class id{public...main(){}}\n");}
    ;

ClassDecl:
      CLASSSYM IDENT OPENBRACE VarDecls MethodDecls CLOSEBRACE {printf("class id{VarDecls MethodDecls}\n");}
    | CLASSSYM IDENT EXTENDSYM IDENT OPENBRACE VarDecls MethodDecls CLOSEBRACE {}
    ;

ClassDecls:
      /*Epsilon*/ {}
    | ClassDecls ClassDecl
    ;

PrimitiveTypeDecl:
      PrimitiveType IDENT SEMICOLON {}
    ;

VarDecl:
      Type IDENT SEMICOLON {}
    ;

VarDecls:
      /*Epsilon*/ {}
    | VarDecls VarDecl
    ;

MethodDecl:
      PUBLICSYM Type IDENT LPAREN FormalList RPAREN 
        OPENBRACE MethodCode RETURNSYM Exp SEMICOLON CLOSEBRACE {}
    ;

MethodCode:
      PrimitiveTypeDecl MethodCode {}
    | IDENT IDENT SEMICOLON MethodCode {}
    | IDENT ASSIGNMENT IDENT SEMICOLON MethodCode {}
    | NonAssignStmt Statements {}
    ;

MethodDecls:
      /*Epsilon*/ {}
    | MethodDecls MethodDecl
    ;


FormalList:
    /*Epsislon*/ {}
    | Type IDENT FormalRests {}
    ;

FormalRests:
    /*Epsislon*/ {}
    | FormalRests FormalRest {}
    ;

FormalRest:
      COMMA Type IDENT {}
    ;

Type:
      CustomType {}
    | PrimitiveType {}
    ;

CustomType:
      IDENT {}
    ;

PrimitiveType:
      INTSYM OPENBRKT CLOSEBRKT {}
    | BOOLEANSYM {}
    | INTSYM {}
    ;

Assignment:
      IDENT ASSIGNMENT Exp SEMICOLON {}
    | IDENT OPENBRKT Exp CLOSEBRKT ASSIGNMENT Exp SEMICOLON {}
    ;

NonAssignStmt:
      OPENBRACE Statements CLOSEBRACE {}
    | IFSYM LPAREN Exp RPAREN Statement ELSESYM Statement {}
    | WHILESYM LPAREN Exp RPAREN Statement {}
    | PRINTSYM LPAREN Exp RPAREN SEMICOLON {printf("Print()\n");}
    ;

Statement:
      NonAssignStmt {}
    | Assignment {}
    ;

Statements:
    /*Epsilon*/ {}
    | Statements Statement { printf("Statements\n");}
    ;

Exp:
      Exp[L] Op Exp[R]                             { $$ = new OpExpression($L, $R); }
    | Exp[L] OPENBRKT Exp[R] CLOSEBRKT             { $$ = new BrcktExpression($L, $R); }
    | Exp[L] PERIOD LENGTHSYM                      { $$ = new LengthExpression($L);}
    | Exp[L] PERIOD IDENT LPAREN ExpList[R] RPAREN { $$ = new MethidExpression($L, $R)}
    | NUMBER                                       { $$ = new TokenExpression("Number"); }
    | TRUESYM                                      { $$ = new TokenExpression("True"); }
    | FALSESYM                                     { $$ = new TokenExpression("False"); }
    | IDENT                                        { $$ = new TokenExpression("Ident"); }
    | THISSYM                                      { $$ = new TokenExpression("this"); }
    | NEWSYM INTSYM OPENBRKT Exp[L] CLOSEBRKT      { $$ = new NewIntArrayExpression($L); }
    | NEWSYM IDENT[L] LPAREN RPAREN                { $$ = new MethodExpression($L); }
    | NEG Exp[L]                                   { $$ = new NegExpression($L); }
    | LPAREN Exp[L] RPAREN                         { $$ = new ParenExpression($L);}
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
      Exp ExpRests {}
    ;

ExpRests:
      /*eps*/ {printf("ExpRests -> epsilon");}
    | ExpRests ExpRest {}
    ;

ExpRest:
    COMMA Exp {}
    ;

%%
int main(){
    yyin = stdin;
    yyparse();
    return 0;
}

void yyerror(const char *s) {
	printf ("Error!! %s\n", s);
}
