%option noyywrap
%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
/*%
liste des token IDENTIFICATEUR CONSTANTE VOID INT FOR WHILE IF ELSE SWITCH CASE DEFAULT
BREAK RETURN PLUS MOINS MUL DIV LSHIFT RSHIFT BAND BOR LAND LOR LT GT 
GEQ LEQ EQ NEQ NOT EXTERN*/
void non_defini();
void erreur();

#define TOKEN_EOF       0

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





"<"                     { return LT     ; } // le < et le > ne sont pas definis comme ca dans le .y
">"                     { return GT     ; }
"+"                     { return PLUS   ; }
"-"                     { return MOINS  ; }
"*"                     { return MUL    ; }
"/"                     { return DIV    ; }

"&"                     { return LAND   ; } // et logique
"|"                     { return LOR    ; } // ou logique
"!"                     { return NOT    ; }


"break"                 { return BREAK      ;}
"return"                { return RETURN     ;}
"case"                  { return CASE       ;}
"default"               { return DEFAULT    ;}
"else"                  { return ELSE       ;}
"extern"                { return EXTERN     ;}
"for"                   { return FOR        ;}
"if"                    { return IF         ;}
"int"                   { return INT        ;}
"switch"                { return SWITCH     ;}
"then"                  { return THEN       ;}
"void"                  { return VOID       ;}
"while"                 { return WHILE      ;}

"<="                    { return LEQ        ;}
">="                    { return GEQ        ;}
"=="                    { return EQ         ;}
"!="                    { return NEQ        ;}
"&&"                    { return BAND       ;} // et binaire
"||"                    { return BOR        ;} // ou binaire
">>"                    { return RSHIFT     ;}
"<<"                    { return LSHIFT     ;}


{D}+                    { yylval.chaine = strdup(yytext) ; return CONSTANTE; }
{L}+({L}|{D})*          { yylval.chaine = strdup(yytext) ; return IDENTIFICATEUR      ;} //lettre suivie de nimporte quel combinsaison de lettre et de chiffre  {L}({L}|{D})*           { non_defini(); return -1; } /* A completer */ 


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

// int main(){
//     int token_courant = yylex();
//     printf("Codes des tokens retournés par yylex: ");

//     do {
//         printf("%d ", token_courant);
//         token_courant = yylex();
//     } while (token_courant != TOKEN_EOF);

//     printf("\n");
//     return 0;
// }


/*


"<"                     { return LT     ;} // le < et le > ne sont pas definis comme ca dans le .y
">"                     { return GT     ;}



"."                     { return '.'; }

PAS SUR DE SON UTILITE


*/


/*
lex main.l && gcc lex.yy.c -ll && cat fichiersProf/exempleminiC.c | ./a.out
*/