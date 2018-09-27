%{
#include "minijava.tab.h"

#ifdef __cplusplus
extern "C" int  yylex();
#endif /*__cplusplus*/
%}

digit         [0-9]
letter        [a-zA-Z_]

%option noyywrap
%option noinput
%option nounput

%%

"class"                        { return CLASSSYM;   }
"public"                       { return PUBLICSYM;  }
"static"                       { return STATICSYM;  }
"void"                         { return VOIDSYM;    }
"main"                         { return MAINSYM;    }
"extends"                      { return EXTENDSYM;  }
"int"                          { return INTSYM;     }
"boolean"                      { return BOOLEANSYM; }
"String"                       { return STRINGSYM;  }
"length"                       { return LENGTHSYM;  }
"this"                         { return THISSYM;    }
"true"                         { return TRUESYM;    }
"false"                        { return FALSESYM;   }
"new"                          { return NEWSYM;     }
"System.out.println"           { return PRINTSYM;   }
"return"                       { return RETURNSYM;  }
"["                            { return OPENBRKT;   }
"]"                            { return CLOSEBRKT;  }
"{"                            { return OPENBRACE;  }
"}"                            { return CLOSEBRACE; }
"+"                            { return PLUS;       }
"-"                            { return MINUS;      }
"*"                            { return TIMES;      }
"/"                            { return SLASH;      }
"("                            { return LPAREN;     }
")"                            { return RPAREN;     }
";"                            { return SEMICOLON;  }
","                            { return COMMA;      }
"."                            { return PERIOD;     }
"="                            { return ASSIGNMENT; }
"=="                           { return EQ;         }
"!="                           { return NEQ;        }
"<"                            { return LSS;        }
">"                            { return GTR;        }
"<="                           { return LEQ;        }
">="                           { return GEQ;        }
"!"                            { return NEG;        }
"^"                            { return XOR;        }
"&"                            { return AND;        }
"|"                            { return OR;         }
"for"                          { return FORSYM;     }
"if"                           { return IFSYM;      }
"else"                         { return ELSESYM;    }
"do"                           { return DOSYM;      }
"while"                        { return WHILESYM;   }
{letter}({letter}|{digit})*    { return IDENT;      }
{digit}+                       { return NUMBER;     }
\/\/.*\n                          /*Skip Comment*/


[ \t\n\r]            /* skip whitespace */
.                    { printf("Unknown character [%c]\n",yytext[0]);
                       printf ("UNKNOWN");}
%%
