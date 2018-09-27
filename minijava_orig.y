%{
#include <stdio.h>

// stuff from flex that bison needs to know about:
extern int yylex();
extern int yyparse();
extern FILE *yyin;
 
void yyerror(const char *s);
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:

%union {
    int num;
    Type* type;
    VacDecl* vardecl;
    VarDecls* vardecls;
    MainClass* mainclass;
    Statement* statement;
    Statements* statements;
    FormalList* formallist;
    MethodDecl methoddecl;
    MethodDecls* methoddecls;
    ClassDecl* classdecl;
    ClassDecls* classdecls;
    Assignment* assignment;
    ExpList* explist;
    Program* program;
}

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

VarDecl:
      Type IDENT SEMICOLON {}
    ;

VarDecls:
      /*Epsilon*/ {}
    | VarDecls VarDecl
    ;

MethodDecl:
      PUBLICSYM Type IDENT LPAREN FormalList RPAREN 
        OPENBRACE VarDecls Statements RETURNSYM Exp SEMICOLON CLOSEBRACE {}
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
      INTSYM OPENBRKT CLOSEBRKT {}
    | BOOLEANSYM {}
    | INTSYM {}
    | IDENT {} 
    ;

Statement:
      OPENBRACE Statements CLOSEBRACE {}
    | IFSYM LPAREN Exp RPAREN Statement ELSESYM Statement {}
    | WHILESYM LPAREN Exp RPAREN Statement {}
    | PRINTSYM LPAREN Exp RPAREN SEMICOLON {printf("Print()\n");}
    | IDENT ASSIGNMENT Exp SEMICOLON {}
    | IDENT OPENBRKT Exp CLOSEBRKT ASSIGNMENT Exp SEMICOLON {}
    ;

Statements:
    /*Epsilon*/ {}
    | Statements Statement { printf("Statements\n");}
    ;

Exp:
      Exp Op Exp                             { printf("Exp Op Exp\n");    }
    | Exp OPENBRKT Exp CLOSEBRKT             { printf("Exp [Exp]\n");     }
    | Exp PERIOD LENGTHSYM                   { printf("Exp.length\n");}
    | Exp PERIOD IDENT LPAREN ExpList RPAREN { printf("Exp.id(ExpList)\n");}
    | NUMBER                                 { printf("NUMBER\n");}
    | TRUESYM                                { printf("TRUE\n");}
    | FALSESYM                               { printf("FALSE\n");}
    | IDENT                                  { printf("Identificador\n");}
    | THISSYM                                { printf("this\n");}
    | NEWSYM INTSYM OPENBRKT Exp CLOSEBRKT   { printf("new int[Exp]\n");}
    | NEWSYM IDENT LPAREN RPAREN             { printf("new id()\n");}
    | NEG Exp                                { printf("!Exp\n");}
    | LPAREN Exp RPAREN                      { printf("( Exp )\n");} 
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
