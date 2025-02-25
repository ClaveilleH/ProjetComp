%option noyywrap
%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
// #include "y.tab.h"

void non_defini();
void erreur();

#define TOKEN_EOF       0

#define TOKEN_BREAK     255
#define TOKEN_CASE      254
#define TOKEN_DEFAULT   253
#define TOKEN_ELSE      252
#define TOKEN_EXTERN    251
#define TOKEN_FOR       250    
#define TOKEN_IF        249
#define TOKEN_INT       248
#define TOKEN_SWITCH    247
#define TOKEN_THEN      246   
#define TOKEN_VOID      245
#define TOKEN_WHILE     244

#define TOKEN_SUP_EG    243
#define TOKEN_INF_EG    242
#define TOKEN_EGAL      241
#define TOKEN_DIFF      240
#define TOKEN_AND       239
#define TOKEN_OR        238
#define TOKEN_SHIFT_D   237
#define TOKEN_SHIFT_G   236

#define TOKEN_VAR       235
#define TOKEN_NB        234

#define ERROR           fprintf (stderr, "\033[1;31mErreur lexicale\033[0m: [%s] à la ligne %d.\n", yytext, yylineno);

%}

D                       [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*



%%
">"                     { return '>'; }
"<"                     { return '<'; }
"!"                     { return '!'; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"&"                     { return '&'; }
"|"                     { return '|'; }
"="                     { return '='; }
"."                     { return '.'; }
","                     { return ','; }
";"                     { return ';'; }
":"                     { return ':'; }
"("                     { return '('; }
")"                     { return ')'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
"["                     { return '['; }
"]"                     { return ']'; }


"break"                 { return TOKEN_BREAK    ;}
"case"                  { return TOKEN_CASE     ;}
"default"               { return TOKEN_DEFAULT  ;}
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
">>"                    { return TOKEN_SHIFT_D  ;}
"<<"                    { return TOKEN_SHIFT_G  ;}


{D}+                    { return TOKEN_NB       ;}       // int
({L}+({L}|{D})*)        { return TOKEN_VAR      ;} //lettre suivie de naimporte quel combinsaison de lettre et de chiffre  {L}({L}|{D})*           { non_defini(); return -1; } /* A completer */ 


0[xX]{H}+{IS}?          { non_defini(); return -1; } /* A completer  un nb haxa + u ou l jsp pk */
0{D}+{IS}?              { non_defini(); return -1; } /* A completer  la meme qui commence par 0 */
{D}+{IS}?               { non_defini(); return -1; } /* A completer  un chifre ou nb suivit de u ou l jsp pk */
L?'(\\.|[^\\'])+'       { non_defini(); return -1; } /* A completer  0 ou une fois L suivit de guillemets et de n'importe quoi */

{D}+{E}{FS}?            { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait

L?\"(\\.|[^\\"])*\"     { non_defini(); return -1; } /* A completer */   // JSP ce que ca fait

"/*".*"*/"              { /* RIEN */} //! commentaires, c'est moi qui l'ai fait a verifier
[ \t\v\n\f]             { /* RIEN */}

.                       { erreur(); }
%%
// \n      printf("Ligne %d\n", yylineno);
// " "                     /*rien*/;
// .*                      {ECHO; printf(">%d<", yylineno);};
// .                       { return yytext[0]; }
// [ \t\n] { fprintf (stderr, "erreur lexicale %s à la ligne %d.\n", yytext, yylineno); }
// {D}*"."{D}+({E})?{FS}?  { non_defini(); return -1; } /* A completer */
// ({LETTRE}{LETTRE}|{NUM}) return TOKEN_VAR;
// {D}+"."{D}*({E})?{FS}?  { non_defini(); return -1; } /* A completer */


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