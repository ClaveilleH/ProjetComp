%option noyywrap
%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
// #include "y.tab.h"

#define TOKEN_EOF       0
#define TOKEN_VAL       1
#define TOKEN_VAR       2
#define TOKEN_PLUS      3
#define TOKEN_MOINS     4
#define TOKEN_MUL       5
#define TOKEN_DIV       6
#define TOKEN_AFFECT    7
#define TOKEN_FIN       8
#define TOKEN_LPAR      9
#define TOKEN_RPAR      10

/* Ne gere pas les commentaires. A rajouter */
/* Supprimer les lignes dont vous n'avez pas besoin. */

%}

D                       [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*

NUM                     [0-9]
LETTRE                  [A-Za-z]


%%
"break"                 { /* A completer */ }
"case"                  { /* A completer */ }
"default"               { /* A completer */ }
"else"                  { /* A completer */ }
"extern"                { /* A completer */ }
"for"                   { /* A completer */ }
"if"                    { /* A completer */ }
"int"                   { /* A completer */ }
"switch"                { /* A completer */ }
"then"                  { /* A completer */ }
"void"                  { /* A completer */ }
"while"                 { /* A completer */ }

{NUM}+                  return TOKEN_VAL;       // int
({LETTRE}+({LETTRE}|{NUM})*) return TOKEN_VAR;  //lettre suivie de naimporte quel combinsaison de lettre et de chiffre  {L}({L}|{D})*           { /* A completer */ } 


0[xX]{H}+{IS}?          { /* A completer */ }   // JSP ce que ca fait
0{D}+{IS}?              { /* A completer */ }   // JSP ce que ca fait
{D}+{IS}?               { /* A completer */ }   // JSP ce que ca fait
L?'(\\.|[^\\'])+'       { /* A completer */ }   // JSP ce que ca fait

{D}+{E}{FS}?            { /* A completer */ }   // JSP ce que ca fait

L?\"(\\.|[^\\"])*\"     { /* A completer */ }   // JSP ce que ca fait


">"                     { /* A completer */ }
"<"                     { /* A completer */ }
"<="                    { /* A completer */ }
">="                    { /* A completer */ }
"=="                    { /* A completer */ }
"!="                    { /* A completer */ }


"!"                     { /* A completer */ }
"&&"                    { /* A completer */ }
"||"                    { /* A completer */ }


"+"                     return TOKEN_PLUS;
"-"                     return TOKEN_MOINS;
"*"                     return TOKEN_MUL;
"/"                     return TOKEN_DIV;
">>"                    { /* A completer */ }
"<<"                    { /* A completer */ }
"&"                     { /* A completer */ }
"|"                     { /* A completer */ }

"="                     return TOKEN_AFFECT;


"."                     { /* A completer */ }
","                     { /* A completer */ }
";"                     return TOKEN_FIN;
":"                     { /* A completer */ }
"("                     return TOKEN_LPAR;
")"                     return TOKEN_RPAR;
"{"                     { /* A completer */ }
"}"                     { /* A completer */ }
"["                     { /* A completer */ }
"]"                     { /* A completer */ }


"/*".*"*/"              {/* RIEN */} // commentaires


[ \t\v\n\f]             {/* RIEN */}


.                       { fprintf (stderr, "erreur lexicale [%s] à la ligne %d.\n", yytext, yylineno); }
%%
// \n      printf("Ligne %d\n", yylineno);
// " "                     /*rien*/;
// .*                      {ECHO; printf(">%d<", yylineno);};
// .                       { return yytext[0]; }
// [ \t\n] { fprintf (stderr, "erreur lexicale %s à la ligne %d.\n", yytext, yylineno); }
// {D}*"."{D}+({E})?{FS}?  { /* A completer */ }
// ({LETTRE}{LETTRE}|{NUM}) return TOKEN_VAR;
// {D}+"."{D}*({E})?{FS}?  { /* A completer */ }

int main(){
    int token_courant = yylex();
    printf("Codes des tokens retournés par yylex: ");

    do {
        printf("%d ", token_courant);
        token_courant = yylex();
    } while (token_courant != TOKEN_EOF);

    printf("\n");
    return 0;
}

/*
lex main.l && gcc lex.yy.c -ll && cat fichiersProf/exempleminiC.c | ./a.out
*/