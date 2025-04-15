%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

#define COLOR_RED "\033[31m"
#define RESET_COLOR "\033[0m"

int yylex();
void yyerror(char *s);
void error(char *s);
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
	struct _variable *variable;
	struct _variable **varTable;
}
/* %define YYSTYPE code */ // pas compatible avec yacc (que bison)
/* %type <code> declarateur liste_declarations declaration type liste_declarateurs */

/* %type <code> declaration type liste_declarations
%type <code> fonction liste_fonctions
%type <code> variable */
%type <variable> declarateur liste_declarateurs declaration 
%type <varTable> liste_declarations


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
		liste_declarations liste_fonctions 
	;
liste_declarations	:	
		liste_declarations declaration 	{
			/*on fusione la liste chainée avec la hashtable*/
			$$ = $1;
			variable *temp = $2;
			variable *prev = NULL;
			while (temp != NULL) {
				int h = hash(temp->varName);
				if ($$[h] == NULL) {
					$$[h] = temp;
				} else {
					variable *temp2 = $$[h];
					if (strcmp(temp2->varName, temp->varName) == 0) { // si la variable est déjà déclarée
						yyerror("Variable already declared in the same scope");
					}
					append_variable(temp2, temp);
					temp2->nextVar = temp;
				}
				prev = temp;
				temp = temp->nextVar;
				prev->nextVar = NULL; // on coupe la liste pour ne pas la relier à la fin
			}
			// print_var_table($$);

		}
	|	/* epsilon */ { $$ = new_varTable(); }
	;
liste_fonctions	:	
		liste_fonctions fonction
	|   fonction
	;
declaration	:	
		type liste_declarateurs ';' {
			$$ = $2;
		}
	;
liste_declarateurs	:	
		liste_declarateurs ',' declarateur 	{
			/*on ajoute declarateur a la fin de la liste de declarateurs*/
			$$ = $1;
			variable *temp = $1;
			while (temp->nextVar != NULL) {
				if (strcmp(temp->varName, $3->varName) == 0) { // si la variable est déjà déclarée
					yyerror("Variable already declared in the same scope");
				}
				temp = temp->nextVar;
			}
			temp->nextVar = $3;
			temp = $$;

			printf("[");
			while (temp->nextVar != NULL) {
				printf("%s, ", temp->varName);
				temp = temp->nextVar;
			}
			printf("%s]\n", temp->varName);
		}
	|	declarateur {
		$$ = new_variable();
		$$->varName = strdup($1->varName);
		printf("[$$ = %s]\n", $1->varName);
		
		}
	;
declarateur	:	
		IDENTIFICATEUR {
			$$ = malloc(sizeof(variable));
			if ($$ == NULL) {
				fprintf(stderr, "Erreur d'allocation mémoire\n");
				exit(1);
			}
			$$->varName = strdup($1);
			$$->nextVar = NULL;
		}
	|	declarateur '[' CONSTANTE ']'
	;
fonction	:	
		type IDENTIFICATEUR '(' liste_parms ')' '{' liste_declarations liste_instructions '}' 
	|	EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';'	
	;
type	:	
		VOID
	|	INT
	;	
liste_parms	:	
		liste_parms ',' parm
	|	parm
	|	/* epsilon */
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
	|	IDENTIFICATEUR '(' liste_expressions ')' // ?? c'est un appel de fonction ?
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
	fprintf(stderr, COLOR_RED);
	fprintf(stderr, "Error: ");
	fprintf(stderr, RESET_COLOR);
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Error at line %d\n", yylineno);
	fprintf(stderr, "Error near: %s\n", yytext);
	exit(1);
}

void error(char *s) {
	/*comme yyerror mais avec un free*/
	fprintf(stderr, COLOR_RED);
	fprintf(stderr, "Error: ");
	fprintf(stderr, RESET_COLOR);
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Error at line %d\n", yylineno);
	fprintf(stderr, "Error near: %s\n", yytext);
	free(s);
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

		/* reset_tables(); */


		yyparse();
		/* print_var_table(); */
		/* print_function_table(); */

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