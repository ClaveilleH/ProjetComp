%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

#define COLOR_RED "\033[31m"
#define COLOR_PURPLE "\033[35m"
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
	char *chaine;
	struct Node *node;
	struct _nodeList *nodeList;

	struct _variable *variable;
	struct _variable **varTable;

	// struct _functionList **functionTable;
	struct _functionList *functionList_type;
}
/* %define YYSTYPE code */ // pas compatible avec yacc (que bison)
/* %type <code> declarateur liste_declarations declaration type liste_declarateurs */

/* %type <code> declaration type liste_declarations
%type <code> fonction liste_fonctions
%type <code> variable */
%type <variable> declarateur liste_declarateurs declaration 
%type <variable> parm liste_parms
%type <varTable> liste_declarations

/* %type <functionList_type> liste_fonctions */
%type <node> fonction appel liste_expressions variable instruction
%type <nodeList> liste_fonctions liste_instructions

%type <chaine> type



/* %token <entier>  */
%token <chaine> IDENTIFICATEUR CONSTANTE


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
			// final_print($1, $2);
			printf("Programme :\n");
			printf("├── Declarations globales\n");
			for (int i = 0; i < TAILLE; i++) {
				if ($1[i] != NULL) {
					variable *temp = $1[i];
					while (temp != NULL) {
						printf("│   ├── %s\n", temp->varName);
						temp = temp->nextVar;
					}
				}
			}
			printf("│\n├── Fonctions\n");
			nodeList *temp2 = $2;
			while (temp2 != NULL) {
				if (temp2->node->type == FUNCTION) {
					printf("│   ├── Fonction : %s\n", temp2->node->function.name);
					printf("│   │   │\n");
					printf("│   │   ├── Type : %s\n", temp2->node->function.type);
					printf("│   │   ├── Paramètres : (");
					variable *temp = temp2->node->function.params;
					while (temp != NULL) {
						printf("%s, ", temp->varName);
						temp = temp->nextVar;
					}
					printf(")\n");
					printf("│   │   ├── Declarations\n");
					
					printf("│   │   ├── Instructions\n");
					// nodeList *temp3 = temp2->node->function.instructions;
					// while (temp3 != NULL) {
					// 	if (temp3->node->type == FUNCTION_CALL) {
					// 		printf("│   │   ├── Appel de fonction : %s\n", temp3->node->fctCall.name);
					// 	} else if (temp3->node->type == VARIABLE) {
					// 		printf("│   │   ├── Variable : %s\n", temp3->node->variable.name);
					// 	} else if (temp3->node->type == TEST) {
					// 		printf("│   │   ├── Test\n");
					// 	}
					// 	temp3 = temp3->next;
					// }
					printf("│   │\n");
				}
				temp2 = temp2->next;
			}
		}
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
			
			

		}
	|	/* epsilon */ { $$ = new_varTable(); }
	;
liste_fonctions	:	
		liste_fonctions fonction {
			/*on ajoute la fonction a la liste de fonctions*/
			$$ = $1;
			nodeList *temp = new_nodeList();
			temp->node = $2;
			temp->next = NULL;
			append_nodeList($$, temp);
		}
	|   fonction { 
			$$ = new_nodeList();
			$$->next = NULL;
			$$->node = $1;
		}
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

			// printf("[");
			// while (temp->nextVar != NULL) {
			// 	printf("%s, ", temp->varName);
			// 	temp = temp->nextVar;
			// }
			// printf("%s]\n", temp->varName);
		}
	|	declarateur {
		$$ = new_variable();
		$$->varName = strdup($1->varName);
		// printf("[$$ = %s]\n", $1->varName);
		
		}
	;
declarateur	:	
		IDENTIFICATEUR {
			$$ = new_variable();
			$$->varName = strdup($1);
		}
	|	declarateur '[' CONSTANTE ']'
	;
fonction	:	
		type IDENTIFICATEUR '(' liste_parms ')' '{' liste_declarations liste_instructions '}' {
			$$ = new_node(FUNCTION);
			$$->function.name = strdup($2);
			$$->function.type = strdup($1);
			$$->function.params = $4;
			Node *temp = new_node(BLOCK);
			temp->block.nodeList = $8;
			$$->function.body = temp;

		}
	|	EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';'	{
			$$ = new_function_node();
			$$->function.name = strdup($3);
			$$->function.type = strdup($2);
			printf("Fonction externe : %s | %s (", $2, $3);
		}
	
	;
type	:	
		VOID 	{ $$ = strdup("void"); }
	|	INT		{ $$ = strdup("int"); }
	;	
liste_parms	:	
		liste_parms ',' parm { 
			if (append_variable($1, $3)) {
				yyerror("Param name already used");
			}
			$$ = $1; }
	|	parm { $$ = $1; }
	|	/* epsilon */ { $$ = NULL; }
	;
parm:	
		INT IDENTIFICATEUR {$$ = new_variable(); $$->varName = strdup($2); }
	;
liste_instructions :	
		liste_instructions instruction {
			/*on ajoute instruction a la liste d'instructions*/
			if ($1 == NULL) {
				$$ = new_nodeList();
				$$->node = $2;
				$$->next = NULL;
			} else {
				$$ = $1;
				nodeList *temp = new_nodeList();
				temp->node = $2;
				temp->next = NULL;
				append_nodeList($$, temp);
				
			}
		}
	|	/* epsilon */ { $$ = NULL; printf("liste_instructions : epsilon\n"); }
	;
instruction	:	
		iteration { printf("iteration\n"); $$ = new_node(TEST); }
	|	selection { printf("selection\n"); $$ = new_node(TEST); }
	|	saut { printf("saut\n"); $$ = new_node(TEST); }
	|	affectation ';' { printf("affectation\n"); $$ = new_node(TEST); } 
	|	bloc { printf("bloc\n"); $$ = new_node(TEST); }
	|	appel { printf("appel\n"); $$ = new_node(TEST); }
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
		IDENTIFICATEUR '(' liste_expressions ')' ';' {
			printf("appel de fonction : %s\n", $1);
			$$ = new_node(FUNCTION_CALL);
			$$->fctCall.name = strdup($1);
			// $$->function_call.params = $3;
		} 
	;
variable	:	
		IDENTIFICATEUR	{
			$$ = new_node(VARIABLE);
			$$->variable.name = strdup($1);
			printf("5->%s\n", $1);
		}
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
		liste_expressions ',' expression	{printf("1\n");}
	|	expression /* ajouté pour le cas d'une seule expression */ {printf("2\n");}
	|	/* epsilon */			{printf("3\n");}
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



			// printf("Fonction : %s\n", $$->function.name);
			// printf("│\n├── Type : %s\n", $$->function.type);
			// printf("├── Paramètres : (");
			// variable *temp = $4;
			// while (temp != NULL) {
			// 	printf("%s, ", temp->varName);
			// 	temp = temp->nextVar;
			// }
			// printf(")\n├── Declarations\n");
			// for (int i = 0; i < TAILLE; i++) {
			// 	if ($7[i] != NULL) {
			// 		variable *temp = $7[i];
			// 		while (temp != NULL) {
			// 			printf("│   ├── %s\n", temp->varName);
			// 			temp = temp->nextVar;
			// 		}
			// 	}
			// }
			// printf("│\n├── Instructions\n");
			// nodeList *temp2 = $8;
			// while (temp2 != NULL) {
			// 	if (temp2->node->type == FUNCTION_CALL) {
			// 		printf("│   ├── Appel de fonction : %s\n", temp2->node->fctCall.name);
			// 	} else if (temp2->node->type == VARIABLE) {
			// 		printf("│   ├── Variable : %s\n", temp2->node->variable.name);
			// 	} else if (temp2->node->type == TEST) {
			// 		printf("│   ├── Test\n");
			// 	}
			// 	temp2 = temp2->next;
			// }
			// printf("\n");


*/



/*
make && ./comp.out Tests/exempleminiC.c
*/