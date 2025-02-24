%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include "y.tab.h"

enum tag {
    TOKEN_EOF,
    TOKEN_VAL,
    TOKEN_VAR,
    TOKEN_PLUS,
    TOKEN_MOINS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_AFFECT,
    TOKEN_FIN,
    TOKEN_AUTO,
    TOKEN_BREAK,
    TOKEN_CASE,
    TOKEN_CHAR,
    TOKEN_CONST,
    TOKEN_CONTINUE,
    TOKEN_DEFAULT,
    TOKEN_DO,
    TOKEN_DOUBLE,
    TOKEN_ELSE,
    TOKEN_ENUM,
    TOKEN_EXTERN,
    TOKEN_FLOAT,
    TOKEN_FOR,
    TOKEN_GOTO,
    TOKEN_IF,
    TOKEN_INT,
    TOKEN_LONG,
    TOKEN_REGISTER,
    TOKEN_RETURN,
    TOKEN_SHORT,
    TOKEN_SIGNED,
    TOKEN_SIZEOF,
    TOKEN_STATIC,
    TOKEN_SWITCH,
    TOKEN_TYPEDEF,
    TOKEN_UNION,
    TOKEN_UNSIGNED,
    TOKEN_VOID,
    TOKEN_VOLATILE,
    TOKEN_WHILE,
    TOKEN_PLUS1,
    TOKEN_MOINS1,
    TOKEN_MOD,
    TOKEN_TESTEGAL,
    TOKEN_TESTINEGAL,
    TOKEN_TESTSUP,
    TOKEN_TESTINF,
    TOKEN_DECALD,
    TOKEN_DECALD1,
    TOKEN_DECALG,
    TOKEN_DECALG1,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_XOR,
    TOKEN_SUSP,
    TOKEN_STRUCT

};


%}

C [0-9]
L [A-Za-z_]
H                       [a-fA-F0-9]     //hexadecimal
E                       [Ee][+-]?[0-9]+   //Partie exponentielle flottant
FS                      (f|F|l|L)       //suffixes flottants
IS                      (u|U|l|L)* 

%%

"auto"                 return TOKEN_AUTO;
"break"                return TOKEN_BREAK;
"case"                 return TOKEN_CASE;
"char"                 return TOKEN_CHAR;
"const"                return TOKEN_CONST;
"continue"             return TOKEN_CONTINUE;
"default"              return TOKEN_DEFAULT;
"do"                   return TOKEN_DO;
"double"               return TOKEN_DOUBLE;
"else"                 return TOKEN_ELSE;
"enum"                 return TOKEN_ENUM;
"extern"               return TOKEN_EXTERN;
"float"                return TOKEN_FLOAT;
"for"                  return TOKEN_FOR;
"goto"                 return TOKEN_GOTO;
"if"                   return TOKEN_IF;
"int"                  return TOKEN_INT;
"long"                 return TOKEN_LONG;
"register"             return TOKEN_REGISTER;
"return"               return TOKEN_RETURN;
"short"                return TOKEN_SHORT;
"signed"               return TOKEN_SIGNED;
"sizeof"               return TOKEN_SIZEOF;
"static"               return TOKEN_STATIC;
"struct"               return TOKEN_STRUCT;
"switch"               return TOKEN_SWITCH;
"typedef"              return TOKEN_TYPEDEF;
"union"                return TOKEN_UNION;
"unsigned"             return TOKEN_UNSIGNED;
"void"                 return TOKEN_VOID;
"volatile"             return TOKEN_VOLATILE;
"while"                return TOKEN_WHILE;

{C}+                    return TOKEN_VAL;
{L}({L}|{C})*           return TOKEN_VAR;

0{C}+{IS}?               {return TOKEN_VAL;}
{C}+{IS}?                {return TOKEN_VAL;}
L?'(\\.|[^\\'])'        {return TOKEN_VAL;}

L?\"(\\.|[^\\"])*\"     {return TOKEN_VAL;}  

";"                     return TOKEN_FIN;
"..."                   return TOKEN_SUSP;
">>="                   return TOKEN_DECALD;
"<<="                   return TOKEN_DECALG;
"+="                    return TOKEN_PLUS;
"-="                    return TOKEN_MOINS;
"*="                    return TOKEN_MUL;
"/="                    return TOKEN_DIV;
"%="                    return TOKEN_MOD;
"&="                    return TOKEN_AND;
"^="                    return TOKEN_XOR;
"|="                    return TOKEN_OR;
">>"                    return TOKEN_DECALD1;              
"<<"                    return TOKEN_DECALG1;
"++"                    return TOKEN_PLUS1;
"--"                    return TOKEN_MOINS1;
"->"                    return TOKEN_AFFECT;
"&&"                    return TOKEN_AND;
"||"                    return TOKEN_OR;
"<="                    return TOKEN_TESTINF;
">="                    return TOKEN_TESTSUP;
"=="                    return TOKEN_TESTEGAL;
"!="                    return TOKEN_TESTINEGAL;

"//".*                  {/* on ignore les commentaires */}  
"/*"(.|\n)*"*/"         {/* on ignore les commentaires */}  


[ \t]+   ;  // Ignore les espaces
\n      { yylineno++; }  // Compte les lignes

. {
    fprintf(stderr, "Erreur lexicale '%s' à la ligne %d.\n", yytext, yylineno);
}

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    }

    yyin = file;  // Remplace l'entrée standard par le fichier

    int token_courant;
    printf("Codes des tokens retournés :\n");
    
    while ((token_courant = yylex()) != TOKEN_EOF) {
        printf("%d ", token_courant);
    }

    fclose(file);
    return 0;
}

