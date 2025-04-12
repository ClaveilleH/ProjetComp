%{

#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(char *s);

extern int yylineno;
extern char *yytext;
extern FILE *yyin;
%}
%token IDENTIFICATEUR CONSTANTE VOID INT FOR WHILE IF ELSE SWITCH CASE DEFAULT
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
		liste_declarations liste_fonctions
	;
liste_declarations	:	
		liste_declarations declaration 
	|	/* epsilon */
	;
liste_fonctions	:	
		liste_fonctions fonction
	|   fonction
	;
declaration	:	
		type liste_declarateurs ';'
	;
liste_declarateurs	:	
		liste_declarateurs ',' declarateur
	|	declarateur
	;
declarateur	:	
		IDENTIFICATEUR
	|	declarateur '[' CONSTANTE ']'
	;
fonction	:	
		type IDENTIFICATEUR '(' liste_parms ')' '{' liste_declarations liste_instructions '}' //{ printf("fonction \n"); }
	|	EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';'
	;
type	:	
		VOID
	|	INT
	;	
liste_parms	:	
		liste_parms ',' parm //{ printf("liste_params \n"); }
	|	parm 				 //{ printf("liste_params \n"); } // ajouté pour le cas d'un seul paramètre
	|	/* epsilon */   	 //{ printf("liste_params epsilon \n"); }
	;
parm:	
		INT IDENTIFICATEUR 
	;
liste_instructions :	
		liste_instructions instruction
	|	/* epsilon */
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
		IDENTIFICATEUR '(' liste_expressions ')' ';'
	;
variable	:	
		IDENTIFICATEUR
	|	variable '[' expression ']'
	;
expression	:	
		'(' expression ')'
	|	expression binary_op expression %prec OP
	|	MOINS expression
	|	CONSTANTE
	|	variable
	|	IDENTIFICATEUR '(' liste_expressions ')'
	;
liste_expressions	:	
		liste_expressions ',' expression
	|	expression /* ajouté pour le cas d'une seule expression */
	|
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
		yyin = f;
	} else {
		yyin = stdin;
	}
	/* printf("Parsing miniC...\n"); */
	yyparse();
	return 0;
}


/*
make && ./comp.out Tests/exempleminiC.c
*/