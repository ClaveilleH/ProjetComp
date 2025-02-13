%option noyywrap
%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
// #include "y.tab.h"

void non_defini();

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

%}

D                       [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*



%%
"break"                 { non_defini(); return -1; } /* A completer */
"case"                  { non_defini(); return -1; } /* A completer */
"default"               { non_defini(); return -1; } /* A completer */
"else"                  { non_defini(); return -1; } /* A completer */
"extern"                { non_defini(); return -1; } /* A completer */
"for"                   { non_defini(); return -1; } /* A completer */
"if"                    { non_defini(); return -1; } /* A completer */
"int"                   { non_defini(); return -1; } /* A completer */
"switch"                { non_defini(); return -1; } /* A completer */
"then"                  { non_defini(); return -1; } /* A completer */
"void"                  { non_defini(); return -1; } /* A completer */
"while"                 { non_defini(); return -1; } /* A completer */

{D}+                    {return TOKEN_VAL;}       // int
({L}+({L}|{D})*) return TOKEN_VAR;  //lettre suivie de naimporte quel combinsaison de lettre et de chiffre  {L}({L}|{D})*           { non_defini(); return -1; } /* A completer */ 


0[xX]{H}+{IS}?          { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait
0{D}+{IS}?              { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait
{D}+{IS}?               { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait
L?'(\\.|[^\\'])+'       { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait

{D}+{E}{FS}?            { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait

L?\"(\\.|[^\\"])*\"     { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait


">"                     { non_defini(); return -1; } /* A completer */
"<"                     { non_defini(); return -1; } /* A completer */
"<="                    { non_defini(); return -1; } /* A completer */
">="                    { non_defini(); return -1; } /* A completer */
"=="                    { non_defini(); return -1; } /* A completer */
"!="                    { non_defini(); return -1; } /* A completer */


"!"                     { non_defini(); return -1; } /* A completer */
"&&"                    { non_defini(); return -1; } /* A completer */
"||"                    { non_defini(); return -1; } /* A completer */


"+"                     return TOKEN_PLUS;
"-"                     return TOKEN_MOINS;
"*"                     return TOKEN_MUL;
"/"                     return TOKEN_DIV;
">>"                    { non_defini(); return -1; } /* A completer */
"<<"                    { non_defini(); return -1; } /* A completer */
"&"                     { non_defini(); return -1; } /* A completer */
"|"                     { non_defini(); return -1; } /* A completer */

"="                     return TOKEN_AFFECT;


"."                     { non_defini(); return -1; } /* A completer */
","                     { non_defini(); return -1; } /* A completer */
";"                     return TOKEN_FIN;
":"                     { non_defini(); return -1; } /* A completer */
"("                     return TOKEN_LPAR;
")"                     return TOKEN_RPAR;
"{"                     { non_defini(); return -1; } /* A completer */
"}"                     { non_defini(); return -1; } /* A completer */
"["                     { non_defini(); return -1; } /* A completer */
"]"                     { non_defini(); return -1; } /* A completer */


"/*".*"*/"              {/* RIEN */} //! commentaires, c'est moi qui l'ai fait a verifier


[ \t\v\n\f]             {/* RIEN */}


.                       { fprintf (stderr, "\033[1;31mErreur lexicale\033[0m: [%s] à la ligne %d.\n", yytext, yylineno); }
%%
// \n      printf("Ligne %d\n", yylineno);
// " "                     /*rien*/;
// .*                      {ECHO; printf(">%d<", yylineno);};
// .                       { return yytext[0]; }
// [ \t\n] { fprintf (stderr, "erreur lexicale %s à la ligne %d.\n", yytext, yylineno); }
// {D}*"."{D}+({E})?{FS}?  { non_defini(); return -1; } /* A completer */
// ({LETTRE}{LETTRE}|{NUM}) return TOKEN_VAR;
// {D}+"."{D}*({E})?{FS}?  { non_defini(); return -1; } /* A completer */


void non_defini(){
    // \033[1;31m 
    // \033[0m
    // seulement pour nous pour etre sur qu'on a tout setup
    fprintf(stderr, "\033[1;31mErreur\033[0m: fonction non définie\n ligne %d : >> %s <<\n", yylineno, yytext);
}

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