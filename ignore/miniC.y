%{
#include "functions.h"

extern int yylex();
void yyerror(char *s);
void error(char *s);
void yylex_destroy();
FILE *file ;
%}

%union {
    var *variable;
}

%%
programme: /* vide */ ;
%%

/*
yacc -d miniC.y
gcc -o test y.tab.c
*/