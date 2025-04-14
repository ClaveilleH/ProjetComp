%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

int yylex();
void yyerror(char *s);
void yylex_destroy();
FILE *file ;

extern int yylineno;
extern char *yytext;
extern FILE *yyin;
%}

%union {
	int entier;
	char *chaine;
	char *code;
}
/* %define YYSTYPE code */ // pas compatible avec yacc (que bison)
%type <code> declarateur liste_declarations declaration type liste_declarateurs
%type <code> fonction liste_fonctions



%token <entier> CONSTANTE
%token <chaine> IDENTIFICATEUR


%token VOID INT FOR WHILE IF ELSE SWITCH CASE DEFAULT
%token BREAK RETURN PLUS MOINS MUL DIV LSHIFT RSHIFT BAND BOR LAND LOR LT GT 
%token GEQ LEQ EQ NEQ NOT EXTERN



%left PLUS MOINS
%left MUL DIV
%left LSHIFT RSHIFT
%left BOR BAND
%left LAND LOR
%nonassoc THEN
%nonassoc ELSE
%left OP
%left REL
%start programme
%%
programme	:	
		liste_declarations liste_fonctions { 
			fprintf(file, "digraph mon_programme {\n");
			// fprintf(file, "%s", $1);
			fprintf(file, "\n");
			fprintf(file, "%s", $2);
			// fprintf(file, liste_declarations.CODE);
			// printf("liste_declarations : %s\n", $1);
			// printf("liste_fonctions : %s\n", $2);
			fprintf(file, "\n}"); } 
	;
liste_declarations	:	
		liste_declarations declaration 	{printf("lst:%s|%s\n", $1, $2); $$ = concat($1, $2);}
	|	/* epsilon */					{ $$ = ""; }
	;
liste_fonctions	:	
		liste_fonctions fonction 		{/*printf("lst:%s|%s\n", $1, $2);*/ $$ = concat($1, $2);}
	|   fonction						{/*printf("lst:%s|%s\n", $1, $2);*/ $$ = $1;}
	;
declaration	:	
		type liste_declarateurs ';' {
			$$ = concat3($1, " ", $2);
		}
	;
liste_declarateurs	:	
		liste_declarateurs ',' declarateur 	{ 
			$$ = concat("decl_", get_random_name());
			$$ = concat3($$, ", ", $3);
			printf("L->|%s|\n", $$);
		}
	|	declarateur { 
			$$ = $1;
		}
	;
declarateur	:	
		IDENTIFICATEUR					{$$ = $1;}
	|	declarateur '[' CONSTANTE ']' 	{printf("C->|%d|\n", $3);}
	;
fonction	:	
		type IDENTIFICATEUR '(' liste_parms ')' '{' liste_declarations liste_instructions '}' { 
			char *str = concat("func_", get_random_name());
			str = concat(str, " [label=\""); 
			str = concat(str, $2);
			str = concat(str, ", ");
			str = concat(str, $1);
			str = concat(str, "\" shape=invtrapezium color=blue];\n");

			$$ = str; 
			}
	|	EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';'	{ printf("fonction externe \n"); $$ = "fonction externe"; }
	;
type	:	
		VOID { $$ = "void"; }
	|	INT  { $$ = "int"; }
	;	
liste_parms	:	
		liste_parms ',' parm //{ printf("liste_params \n"); }
	|	parm 				 //{ printf("liste_params \n"); } // ajouté pour le cas d'un seul paramètre
	|	/* epsilon */   	 //{ $$ = ""; }
	;
parm:	
		INT IDENTIFICATEUR 		{ printf("paramètre : %s\n", $2); }
	;
liste_instructions :	
		liste_instructions instruction
	|	/* epsilon */		//{ $$ = ""; }
	;
instruction	:	
		iteration
	|	selection
	|	saut
	|	affectation ';'
	|	bloc
	|	appel
	;
iteration	:	
		FOR '(' affectation ';' condition ';' affectation ')' instruction
	|	WHILE '(' condition ')' instruction
	;
selection	:	
		IF '(' condition ')' instruction %prec THEN
	|	IF '(' condition ')' instruction ELSE instruction
	|	SWITCH '(' expression ')' instruction
	|	CASE CONSTANTE ':' instruction
	|	DEFAULT ':' instruction
	;
saut	:	
		BREAK ';'
	|	RETURN ';'
	|	RETURN expression ';'
	;
affectation	:	
		variable '=' expression
	;
bloc	:	
		'{' liste_declarations liste_instructions '}'
	;
appel	:	
		IDENTIFICATEUR '(' liste_expressions ')' ';' { printf("appel de fonction : %s\n", $1); }
	;
variable	:	
		IDENTIFICATEUR		{ printf("variable : %s\n", $1); }	
	|	variable '[' expression ']'
	;
expression	:	
		'(' expression ')'
	|	expression binary_op expression %prec OP
	|	MOINS expression
	|	CONSTANTE
	|	variable
	|	IDENTIFICATEUR '(' liste_expressions ')'	{ printf("appel de fonction : %s\n", $1); }
	;
liste_expressions	:	
		liste_expressions ',' expression
	|	expression /* ajouté pour le cas d'une seule expression */
	|	/* epsilon */		//{ $$ = ""; }
	;
condition	:	
		NOT '(' condition ')'
	|	condition binary_rel condition %prec REL
	|	'(' condition ')'
	|	expression binary_comp expression
	;
binary_op	:	
		PLUS
	|   MOINS
	|	MUL
	|	DIV
	|   LSHIFT
	|	RSHIFT
	|	BAND
	|	BOR
	;
binary_rel	:	
		LAND
	|	LOR
	;
binary_comp	:	
		LT
	|	GT
	|	GEQ
	|	LEQ
	|	EQ
	|	NEQ
	;
%%


void yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Error at line %d\n", yylineno);
	fprintf(stderr, "Error near: %s\n", yytext);
	exit(1);
}

int main(int argc, char **argv) {
	if (argc > 1) { //! J'ai fais ca comme ca mais a verifier si ca marche bien, c'est pour executer sur le fichier passé en parametre
		FILE *f = fopen(argv[1], "r");
		if (!f) {
			perror("Error opening file");
			return 1;
		}
		file = fopen("output.dot", "w");
		if (!file) {
			perror("Error opening output file");
			fclose(f);
			return 1;
		}
		fflush(file);
		yyin = f;
		yyparse();
		fclose(f);
		fclose(file);
	} else {
		yyin = stdin;
		yyparse();
	}
	/* printf("Parsing miniC...\n"); */
	/* yyparse(); */
	yylex_destroy(); // liberer la mémoire allouée par lex
	return 0;
}



/*

{
        $$ = malloc(strlen($1) + strlen($2) + 2); // +2 pour le '\0' et le séparateur éventuel
        if (!$$) {
            yyerror("Erreur d'allocation mémoire");
            exit(1);
        }
        strcpy($$, $1);  // Copie la première chaîne
        strcat($$, $2);  // Concatène la deuxième chaîne
    }

*/



/*
make && ./comp.out Tests/exempleminiC.c
*/