%option noyywrap
%option yylineno

%{
#include "symboles.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"


extern int yylineno;

int yyerror(char* s);

void non_defini();
void erreur();

#define TOKEN_EOF 0
%}

D       [0-9]
L       [a-zA-Z_]

%%


"<="                    { return LEQ; }
">="                    { return GEQ; }
"=="                    { return EQ; }
"!="                    { return NEQ; }
">>"                    { return RSHIFT; }
"<<"                    { return LSHIFT; }
"&&"                    { return LAND; }
"||"                    { return LOR; }

"["                     { return '['; }
"]"                     { return ']'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
","                     { return ','; }
";"                     { return ';'; }
":"                     { return ':'; }
"="                     { return '='; }



"<"                     { return LT; }
">"                     { return GT; }
"+"                     { return PLUS; }
"-"                     { return MOINS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"&"                     { return BAND; }
"|"                     { return BOR; }
"!"                     { return NOT; }

"break"                 { return BREAK; }
"return"                { return RETURN; }
"case"                  { return CASE; }
"default"               { return DEFAULT; }
"else"                  { return ELSE; }
"extern"                { return EXTERN; }
"for"                   { return FOR; }
"if"                    { return IF; }
"int"                   { return INT; }
"switch"                { return SWITCH; }
"then"                  { return THEN; }
"void"                  { return VOID; }
"while"                 { return WHILE; }

{D}+                    { yylval.entier = atoi(yytext); return CONSTANTE; }

{L}+({L}|{D})*          { yylval.chaine = strdup(yytext); return IDENTIFICATEUR; }

"//".*                  { /* commentaire ligne */ }
"/*"([^*]|\*+[^*/])*\*+"/"   { /* Ignorer le commentaire multi-ligne */ }

[ \t\v\n\f]             { /* ignorer espaces */ }

.                       { erreur(); }

%%

void erreur() {
    fprintf(stderr, "\033[1;31mErreur lexicale\033[0m: [%s] à la ligne %d.\n", yytext, yylineno);
}

void non_defini() {
    fprintf(stderr, "\033[1;31mErreur\033[0m: fonction non définie\n ligne %d : >> %s <<\n", yylineno, yytext);
}
