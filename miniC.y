%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symboles.h"


#define COLOR_RED "\033[31m"
#define COLOR_PURPLE "\033[35m"
#define RESET_COLOR "\033[0m"

extern int yylineno;
extern char *yytext;
extern FILE *yyin;

int yylex(void);       
void yyerror(char *s);
void error(char *s);
void yylex_destroy(void);

FILE *file;

void table_reset(); 
symbole *inserer(char *nom);
symbole *chercher(char *nom);

liste_t * creer_liste( param_t p ); 
liste_t * concatener_listes( liste_t *l1, liste_t *l2 );
void afficher_liste( liste_t *liste ); 

NodeList *liste_fonctions = NULL; // liste globale des fonctions

%}

%union {
    int entier;
    char *chaine;
    struct symbole *sym;
	type_t type;
	param_t param;
	liste_t *liste;

	struct Node *node;
	struct NodeList *node_list;
	struct NodeList **node_table;
}


%token <entier> CONSTANTE
%token <chaine> IDENTIFICATEUR
%token  VOID INT FOR WHILE IF ELSE SWITCH CASE DEFAULT
%token BREAK RETURN PLUS MOINS MUL DIV LSHIFT RSHIFT BAND BOR LAND LOR LT GT 
%token GEQ LEQ EQ NEQ NOT EXTERN

%right '=' NOT
%left PLUS MOINS
%left MUL DIV
%left LSHIFT RSHIFT
%left BAND BOR      
%left LAND LOR      
%nonassoc THEN
%nonassoc ELSE
%nonassoc LT GT LEQ GEQ EQ NEQ // comparateurs non associatifs
%nonassoc MOINSUNAIRE		   // moins unaire non associatif
%left OP
%left REL


%start programme


%type <entier> expression
%type <sym> variable

%type <type> type
/* %type <param> parm */
/* %type <liste> liste_parms */

%type <node> declarateur parm fonction
%type <node_list> liste_declarateurs declaration liste_parms liste_fonctions
%type <node_table> liste_declarations

%%
programme	:	
		liste_declarations liste_fonctions {
			// └
			printf("Programme complet\n");
			printf("Programme :\n");
			printf("├── Déclarations globales\n");
			NodeList *tmp;
			for (int i = 0; i < TAILLE; i++) {
				if ($1[i] != NULL) {
					tmp = $1[i];
					while (tmp != NULL) {
						printf("│   ├── %s\n", tmp->node->symbole.nom);
						tmp = tmp->suivant;
					}
				}
			}
			printf("└── Fonctions\n");
			tmp = $2;
			while (tmp != NULL) {
				printf("    ├── %s\n", tmp->node->fonction.nom);
				printf("    │	│\n");
				printf("    │	├── Type : %s\n", tmp->node->fonction.type == ENTIER ? "int" : "void");
				NodeList *tmp2 = tmp->node->fonction.liste_parametres;
				printf("    │	├── Paramètres : (");
				while (tmp2 != NULL) {
					printf("%s, ", tmp2->node->parametre.nom);
					tmp2 = tmp2->suivant;
				}
				printf(")\n");
				printf("    │	├── Déclarations :\n");
				printf("    │	└── Instructions :\n");
				tmp = tmp->suivant;
			}
		}
;

liste_declarations	:	
		liste_declarations declaration {
			printf("Liste de déclarations\n");
			$$ = $1;
			NodeList *tmp = $2;
			while (tmp != NULL) {
				int h = hash(tmp->node->symbole.nom);
				if ($$[h] == NULL) { // si la ligne de la table est vide
					// on ajoute le noeud à la table de hachage
					$$[h] = tmp;
					tmp = tmp->suivant;
					$$[h]->suivant = NULL; // on met le suivant à NULL
					// pour pas ajouter toute la liste chaînée
				} else {
					NodeList *tmp2 = $$[h];
					while (tmp2 != NULL) { // on verifie si la variable existe déjà
						if (strcmp(tmp2->node->symbole.nom, tmp->node->symbole.nom) == 0) {
							yyerror("Variable already declared in this scope");
						}
						tmp2 = tmp2->suivant;
					}
					tmp->suivant = $$[h]; // on ajoute le noeud au début de la liste
					$$[h] = tmp; // on met à jour la table de hachage
					tmp = tmp->suivant;
				}
			}
			// afficher_node_table($$);
		}
	| /* epsilon */ {printf("Liste de déclarations vide\n"); $$ = creer_node_table(); }
;

liste_fonctions	:	
		liste_fonctions fonction {
			if (append_node($1, $2)) {
				char *s = malloc( strlen("Redeclaration de la fonction :") + strlen($2->fonction.nom) + 1);
				sprintf(s, "Redeclaration de la fonction : %s", $2->fonction.nom);
				error(s);
			}
		}
	|	fonction {
			$$ = nouveau_node_list($1);
		}
;

declaration	:	
		type liste_declarateurs ';' {
			$$ = $2;
		}
;

liste_declarateurs	:	
		liste_declarateurs ',' declarateur {
			// on ajoute declarateur au début de la liste
			// on verifie pas encore si la variable existe déjà
			$$ = nouveau_node_list($3);
			$$->suivant = $1;
		}
	|	declarateur {
		$$ = nouveau_node_list($1);
	}
;

declarateur:
    IDENTIFICATEUR {
			// On crée un nouveau noeud pour la déclaration de variable
			// on verifie pas encore si la variable existe déjà
			printf("Déclaration de variable : %s, type :%d\n", $1, $<type>-2);
						
			$$ = nouveau_node(SYMBOLE);
			$$->symbole.nom = $1;
			$$->symbole.type = ENTIER; //$<type>-2; // NE MARCHE PAS type transmis via $-2 (depuis 'type') (dans 'declaration')
			$$->symbole.valeur = 0; // valeur initiale à 0 
			// TODO: mettre un param .isInitialized à 0
			
		}
;


fonction:
    type IDENTIFICATEUR '(' liste_parms ')' '{' liste_declarations liste_instructions '}' {
			// affiche_node_list($4);
			$$ = nouveau_node(FONCTION);
			$$->fonction.nom = $2;
			$$->fonction.type = $1;
			$$->fonction.liste_parametres = $4;
			

			append_node(liste_fonctions, $$);
		}
  	| EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';' {
			$$ = nouveau_node(FONCTION);
			$$->fonction.nom = $3;
			$$->fonction.type = $2;
			$$->fonction.liste_parametres = $5;
			//! ATTENION EXTERNE
		}
;


type: VOID { $$ = VOID_TYPE; }
    | INT  { $$ = ENTIER; }
;


liste_parms	:	
		parm	{
			$$ = nouveau_node_list($1);
			// printf("-----Paramètre : int %s\n", $1->parametre.nom);
		}
	|	liste_parms ',' parm	{
			$$ = $1;
			if (append_node($$, $3)) {
				yyerror("Paramètre déjà déclaré dans cette fonction");
			}
		} //concatener_listes( $1, creer_liste($3)); }
	|   /* vide */ { $$ = NULL; }	
;

parm: INT IDENTIFICATEUR {
	$$ = nouveau_node(PARAMETRE);
	$$->parametre.type = ENTIER;
	$$->parametre.nom = $2;
}
;


liste_instructions :	
		liste_instructions instruction
	|
;

instruction:
    affectation ';'
  | appel 
  | iteration
  | selection
  | saut
  | bloc
;


iteration	:	
		FOR '(' affectation ';' condition ';' affectation ')' instruction  { printf("Boucle FOR\n"); }
	|	WHILE '(' condition ')' instruction	{ printf("Boucle WHILE\n"); }
;

selection	:	
		IF '(' condition ')' instruction %prec THEN 	    { printf("Instruction IF\n"); }
	|	IF '(' condition ')' instruction ELSE instruction	{ printf("Instruction IF ... ELSE\n"); }
	|	SWITCH '(' expression ')' instruction				{ printf("Instruction SWITCH\n"); }
	|	CASE CONSTANTE ':' instruction						{ printf("Instruction CASE %d\n", $2); }
	|	DEFAULT ':' instruction								{ printf("Instruction DEFAULT\n"); }
;

saut	:	
		BREAK ';'	{ printf("Instruction BREAK\n"); }
	|	RETURN ';'	{ printf("Instruction RETURN sans valeur\n"); }
	|	RETURN expression ';'	{ printf("Instruction RETURN avec valeur %d\n", $2); }
;

affectation	:
	expression	{ printf("Expression seule : %d\n", $1); }
	| variable '=' expression { $1->valeur = $3;
								printf("Affectation : %s = %d\n", $1->nom, $3); 
							  }
;

bloc : '{' {
            push_table(); // ouverture de bloc
        }
        liste_declarations liste_instructions 
        '}' {
            pop_table(); // fermeture de bloc
        }
;


appel	:	
		IDENTIFICATEUR '(' liste_expressions ')' ';' { printf("Appel de fonction (instruction) : %s\n", $1); }
;



variable	:	// quand on utilise une variable
		IDENTIFICATEUR { $$ = inserer($1); }
	|	variable '[' expression ']'
;

expression	:	
		'(' expression ')' 			  { $$ = $2; printf("Expression entre parenthèses : %d\n", $2); }
	| MOINS expression %prec MOINSUNAIRE { $$ = -$2; printf("Moins unaire : %d\n", -$2); }
	| expression PLUS expression      { $$ = $1 + $3; printf("Somme : %d + %d = %d\n", $1, $3, $1 + $3); }
    | expression MOINS expression     { $$ = $1 - $3; printf("Soustraction : %d - %d = %d\n", $1, $3, $1 - $3); }
    | expression MUL expression       { $$ = $1 * $3; printf("Multiplication : %d * %d = %d\n", $1, $3, $1 * $3); }
    | expression DIV expression       { $$ = $1 / $3;  printf("Division : %d / %d = %d\n", $1, $3, $1 / $3); }
	| expression LSHIFT expression   { $$ = $1 << $3; printf("LSHIFT: %d << %d = %d\n", $1, $3, $$); }
    | expression RSHIFT expression   { $$ = $1 >> $3; printf("RSHIFT: %d >> %d = %d\n", $1, $3, $$); }
	| CONSTANTE              		  { $$ = $1;  printf("Constante : %d\n", $1); }
	| variable 						  { $$ = $1->valeur; printf("Variable : %s = %d\n", $1->nom, $1->valeur); }
	| IDENTIFICATEUR '(' liste_expressions ')' { printf("Appel fonction %s\n", $1); $$ = 0; }
;

liste_expressions	:	
		liste_expressions ',' expression { printf("Liste d'expressions \n");  }
	|	expression { printf("Expression unique \n"); }
	|
;

condition	:	
		NOT '(' condition ')' { printf("Condition NOT\n"); }
	|	condition binary_rel condition %prec REL
	|	'(' condition ')'  { printf("Condition\n"); }
	|	expression binary_comp expression  { printf("Condition binaire\n"); }
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
	if (argc > 1) {
		file = fopen(argv[1], "r");
		if (file == NULL) {
			fprintf(stderr, "Erreur d'ouverture du fichier %s\n", argv[1]);
			exit(1);
		}
		yyin = file;
	} else {
		yyin = stdin;
	}
    push_table(); // init table globale
    init_fonction_table();
    yyparse();
    pop_table(); // nettoyage
	yylex_destroy();
	if (file != NULL) {
		fclose(file);
	}
    return 0;
}

