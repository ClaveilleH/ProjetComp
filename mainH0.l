%option noyywrap
%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
// #include "y.tab.h"

void non_defini();
void erreur();

#define TOKEN_EOF       0

#define TOKEN_VAR       235 // IDENTIFICATEUR
// CONSTANTE
#define TOKEN_VOID      245
#define TOKEN_INT       248
#define TOKEN_FOR       250    
#define TOKEN_WHILE     244
#define TOKEN_IF        249
#define TOKEN_ELSE      252
#define TOKEN_SWITCH    247
#define CASE            254
#define DEFAULT         253

#define BREAK           255
#define TOKEN_RETURN    256
// PLUS
// MOINS
// MUL
// DIV
#define TOKEN_LSHIFT    236 // decalage binaire a gauche
#define TOKEN_RSHIFT    237
#define TOKEN_AND       239
#define TOKEN_OR        238
// ! LAND LOR BAND BOR ???
// LT
// GT

#define TOKEN_SUP_EG    243 //GEQ >=
#define TOKEN_INF_EG    242 //LEQ <=
#define TOKEN_DIFF      240 //NEQ !=
// NOT
#define TOKEN_EXTERN    251


#define TOKEN_THEN      246   
#define TOKEN_EGAL      241

#define TOKEN_NB        234

%}

D                       [0-9]
L                       [a-zA-Z_]

%%
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


">"                     { return '>'; } // a redefinir, faut utiliser des tokens
"<"                     { return '<'; }
"!"                     { return '!'; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"&"                     { return '&'; }
"|"                     { return '|'; }
"."                     { return '.'; }


"break"                 { return BREAK          ;}
"return"                { return TOKEN_RETURN   ;}
"case"                  { return CASE           ;}
"default"               { return DEFAULT        ;}
"else"                  { return TOKEN_ELSE     ;}
"extern"                { return TOKEN_EXTERN   ;}
"for"                   { return TOKEN_FOR      ;}
"if"                    { return TOKEN_IF       ;}
"int"                   { return TOKEN_INT      ;}
"switch"                { return TOKEN_SWITCH   ;}
"then"                  { return TOKEN_THEN     ;}
"void"                  { return TOKEN_VOID     ;}
"while"                 { return TOKEN_WHILE    ;}

"<="                    { return TOKEN_SUP_EG   ;}
">="                    { return TOKEN_INF_EG   ;}
"=="                    { return TOKEN_EGAL     ;}
"!="                    { return TOKEN_DIFF     ;}
"&&"                    { return TOKEN_AND      ;}
"||"                    { return TOKEN_OR       ;}
">>"                    { return TOKEN_RSHIFT   ;}
"<<"                    { return TOKEN_LSHIFT   ;}


{D}+                    { return TOKEN_NB       ;}       // int
({L}+({L}|{D})*)        { return TOKEN_VAR      ;} //lettre suivie de nimporte quel combinsaison de lettre et de chiffre  {L}({L}|{D})*           { non_defini(); return -1; } /* A completer */ 


"//".*                  {/* on ignore les commentaires */}  
"/*"(.|\n)*"*/"         {/* on ignore les commentaires */}  
[ \t\v\n\f]             {/* RIEN */}

.                       { erreur(); }
%%

void erreur(){
    fprintf (stderr, "\033[1;31mErreur lexicale\033[0m: [%s] à la ligne %d.\n", yytext, yylineno);
}

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