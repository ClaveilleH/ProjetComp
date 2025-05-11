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
void warn(char *s);
void yylex_destroy(void);

FILE *file;

NodeList *liste_fonctions = NULL; // liste globale des fonctions

%}

%union {
    int entier;
    char *chaine;
	type_t type;

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


/* %type <entier> expression */
%type <chaine> binary_comp
%type <type> type

%type <node> declarateur parm fonction variable affectation instruction selection saut condition expression
%type <node_list> liste_declarateurs declaration liste_parms liste_fonctions liste_instructions
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
						// printf("│   ├── %s\n", tmp->node->symbole.nom);
						afficher_node2("│   ├──", tmp->node);
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
				NodeList **tmp3 = tmp->node->fonction.table_declarations;
				for (int i = 0; i < TAILLE; i++) {
					if (tmp3[i] != NULL) {
						tmp2 = tmp3[i];
						while (tmp2 != NULL) {
							afficher_node2("    │	│   ", tmp2->node);
							tmp2 = tmp2->suivant;
						}
					}
				}
				printf("    │	│\n");
				printf("    │	└── Instructions :\n");
				afficher_instructions(tmp->node->fonction.liste_instructions);
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
		// Déclaration avec initialisation
		|type liste_declarateurs '=' expression ';' {$$ = $2; }
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
			$$->symbole.isInitialized = 0; // on met la variable comme non initialisée
			$$->symbole.evaluable = 0; // on met la variable comme non évaluable
			// TODO: mettre un param .isInitialized à 0
			
			ajouter_variable($$); // on ajoute la variable à la table de symboles courante
			//? mettre la verification ici ?
		}
;


fonction:
    type IDENTIFICATEUR '(' liste_parms ')' '{' {push_table();} // ouverture de bloc
		liste_declarations liste_instructions '}' {
			// affiche_node_list($4);
			$$ = nouveau_node(FONCTION);
			$$->fonction.nom = $2;
			$$->fonction.type = $1;
			$$->fonction.liste_parametres = $4;
			$$->fonction.table_declarations = $8;
			$$->fonction.liste_instructions = $9;
			

			append_node(liste_fonctions, $$);
			pop_table(); // fermeture de bloc
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

	ajouter_parametre($$); // on ajoute le paramètre à la table de symboles courante
}
;


liste_instructions :	
		liste_instructions instruction {
			// on ajoute instruction a la fin de la liste
			// on verifie pas encore si la variable existe déjà
			if ($1 == NULL) {
				$$ = nouveau_node_list($2);
				
			} else {
				// on ajoute instruction au début de la liste
				// on verifie pas encore si la variable existe déjà
				$$ = $1;
				NodeList *nouv = nouveau_node_list($2);
				nouv->suivant = NULL; // pour pas faire une boucle
				
				if ($$->suivant == NULL) {
					$$->suivant = nouv;
					$$->precedent = nouv;
					nouv->precedent = $$;
				} else {
					nouv->precedent = $$->precedent;
					$$->precedent->suivant = nouv;
					$$->precedent = nouv;
					$$->precedent = nouv;

				}
			}
			// $$ = $1;
			// NodeList *nouv = nouveau_node_list($2);
			
			// nouv->suivant = $$->precedent;
			// nouv->suivant->precedent = nouv;
			// $$->precedent = nouv;
			// nouv->precedent = $$;
			// printf("Liste d'instructions : \n");
		}
	|   /* epsilon */ { printf("Liste d'instructions vide\n"); $$ = NULL; }
;

instruction:
    affectation ';'			 { printf("Instruction : affectation\n"); 
		$$ = nouveau_node(TEST);
		$$->test.txt = "Instruction : Affectation";
	}
  	| expression ';'           { printf("Instruction : expression seule\n");
		$$ = nouveau_node(TEST);
		$$->test.txt = "Instruction : Expression seule";
	}
	| iteration				 { printf("Instruction : boucle\n"); 
		$$ = nouveau_node(TEST);
		$$->test.txt = "Instruction : boucle";
	}
	| selection 			 { printf("Instruction : selection\n"); 
		// $$ = nouveau_node(TEST);
		// $$->test.txt = "Instruction : selection";
		$$ = $1;
	}
	| saut					 { printf("Instruction : saut\n"); 
		$$ = $1;
	}
	| bloc	 				 { printf("Instruction : bloc\n"); 
		$$ = nouveau_node(TEST);
		$$->test.txt = "Instruction : bloc";
	}
;


iteration	:	
		FOR '(' affectation ';' condition ';' affectation ')' instruction  { printf("Boucle FOR\n"); }
	|	WHILE '(' condition ')' instruction	{ printf("Boucle WHILE\n"); }
;

selection	:	
		IF '(' condition ')' instruction %prec THEN 	    { 
			printf("Instruction IF\n");
			$$ = nouveau_node(IF_NODE);
			$$->if_node.instruction = $5;
			$$->if_node.condition = $3;
		}
	|	IF '(' condition ')' instruction ELSE instruction	{ printf("Selection IF ... ELSE\n"); }
	|	SWITCH '(' expression ')' instruction				{ printf("Selection SWITCH\n"); }
	|	CASE CONSTANTE ':' instruction						{ printf("Selection CASE %d\n", $2); }
	|	DEFAULT ':' instruction								{ printf("Selection DEFAULT\n"); }
;

saut	:	
		BREAK ';'	{
			// printf("Instruction BREAK\n"); 
			$$ = nouveau_node(BREAK_NODE);
		}
	|	RETURN ';'	{
			// printf("Instruction RETURN sans valeur\n"); 
			$$ = nouveau_node(RETURN_NODE);
	}
	|	RETURN expression ';'	{
			// printf("Instruction RETURN avec valeur %d\n", $2); 
			$$ = nouveau_node(RETURN_NODE);
	}
;

affectation	:
	expression	{ 
		//? je vois pas trop l'interet de faire ca
		printf("Expression seule : %d\n", $1->expression.valeur); 
	}
	| 	variable '=' expression { 
			printf("*/*/*/*/*/*/*/*/*/*/\n");
			$$ = $1;
			$1->symbole.isInitialized = 1; // on met la variable comme initialisée
			if ($3->expression.evaluable == 1) {
				$1->symbole.valeur = $3->expression.valeur; // on affecte la valeur de l'expression à la variable
				$1->symbole.evaluable = 1; // on met la variable comme évaluable
				printf("Affectation : %s = %d\n", $1->symbole.nom, $1->symbole.valeur);
			} else {
				$1->symbole.evaluable = 0; // on met la variable comme non évaluable
				printf("Affectation : %s (non evaluable) \n", $1->symbole.nom);
				afficher_node2("",$3);
			}
			printf("*//*/*/*/*/*/*/*/*/*/\n");

			//! ATTENTION : IL FAUT EVALUER L'EXPRESSION
		}
;

bloc : '{' {
			afficher_node_table(get_pile()->node);

            push_table(); // ouverture de bloc
			// printf("Ouverture de bloc\n");
        }
        liste_declarations liste_instructions 
        '}' {
			afficher_node_table(get_pile()->node);

            pop_table(); // fermeture de bloc
			// printf("Fermeture de bloc\n");
			printf("-----------\n");
			// affiche_node_list();
			afficher_node_table(get_pile()->node);
			printf("-----------\n");
        }
;






variable	:	// quand on utilise une variable
		IDENTIFICATEUR { /*$$ = inserer($1);*/ //jpense pas qu'il faille utiliser ca ici
			// il faut verif si ca existe déjà
			// sinon on leve une erreur
			Node *result = chercher_symbole($1);
			if (result == NULL) {
				char *s = malloc( strlen("Variable utilisée mais jamais déclarée :") + strlen($1) + 1);
				sprintf(s, "Variable utilisée mais jamais déclarée : %s", $1);
				error(s); // -> kill
			}
			if (result->type == SYMBOLE) {
				// on verifie si la variable est initialisée
				if (result->symbole.isInitialized == 0) {
					char *s = malloc( strlen("Variable utilisée mais jamais initialisée :") + strlen($1) + 1);
					sprintf(s, "Variable utilisée mais jamais initialisée : %s", $1);
					warn(s); // -> warning
					free(s);
				}
			} else if (result->type == FONCTION) {
				yyerror("Fonction utilisée comme variable");
			}

			$$ = result;
		}
	|	variable '[' expression ']'
;

expression	:		
		'(' expression ')' 			  			{ 
			// $$ = $2; printf("Expression entre parenthèses : %d\n", $2); 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_PARENTHESE;
			$$->expression.expression = $2;
			$$->expression.evaluable = $2->expression.evaluable;
			$$->expression.valeur = $2->expression.valeur;

			$$ = nouveau_node(TEST);
			$$->test.txt = "Expression entre parenthèses";

		}
	| MOINS expression %prec MOINSUNAIRE 		{ 
			// $$ = -$2; printf("Moins unaire : %d\n", -$2); 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_MOINS_UNAIRE;
			$$->expression.expression = $2;
			if ($2->expression.evaluable) {
				$$->expression.evaluable = 1;
				$$->expression.valeur = -$2->expression.valeur;
			} else {
				$$->expression.evaluable = 0;
			}
		}
	| expression PLUS expression      			{ 
			// $$ = $1 + $3; printf("Somme : %d + %d = %d\n", $1, $3, $1 + $3); 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_BINAIRE;
			$$->expression.gauche = $1;
			$$->expression.droite = $3;
			$$->expression.operateur = strdup("+");
			evaluer('+', $1, $3, &$$->expression.valeur, &$$->expression.evaluable);
		}
    | expression MOINS expression     			{ 
			// $$ = $1 - $3; printf("Soustraction : %d - %d = %d\n", $1, $3, $1 - $3); 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_BINAIRE;
			$$->expression.gauche = $1;
			$$->expression.droite = $3;
			$$->expression.operateur = strdup("-");
			evaluer('-', $1, $3, &$$->expression.valeur, &$$->expression.evaluable);
		}
    | expression MUL expression       			{ 
			// $$ = $1 * $3; printf("Multiplication : %d * %d = %d\n", $1, $3, $1 * $3); 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_BINAIRE;
			$$->expression.gauche = $1;
			$$->expression.droite = $3;
			$$->expression.operateur = strdup("*");
			evaluer('*', $1, $3, &$$->expression.valeur, &$$->expression.evaluable);

		}
    | expression DIV expression       			{ 
			// $$ = $1 / $3;  printf("Division : %d / %d = %d\n", $1, $3, $1 / $3); 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_BINAIRE;
			$$->expression.gauche = $1;
			$$->expression.droite = $3;
			$$->expression.operateur = strdup("/");

			if ($3->expression.valeur == 0) {
				yyerror("Division par zéro");
			}
			evaluer('/', $1, $3, &$$->expression.valeur, &$$->expression.evaluable);
		}
	| CONSTANTE              		  			{ 
			// $$ = $1;  printf("Constante : %d\n", $1); 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_CONSTANTE;
			$$->expression.valeur = $1;
			$$->expression.evaluable = 1;
		}
	| variable 						  			{ 
			// $$ = $1->symbole.valeur; printf("Variable : %s = %d\n", $1->symbole.nom, $1->symbole.valeur); 
			$$ = $1;
			printf("Variable : %s \n", $1->symbole.nom);

		}
	| IDENTIFICATEUR '(' liste_expressions ')' 	{ 
			printf("EXPRESSION -- Appel fonction %s\n", $1); $$ = 0; 
			$$ = nouveau_node(TEST);
			$$->test.txt = "Appel fonction";

		}
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
	|	expression binary_comp expression  { 
			printf("Condition binaire\n");
			$$ = nouveau_node(CONDITION_BINAIRE);
			$$->condition_binaire.gauche = $1;
			$$->condition_binaire.droite = $3;
			$$->condition_binaire.operateur = $2;
		
			// $$->condition_binaire.droite = nouveau_node(TEST);
			// $$->condition_binaire.droite->test.txt = "Condition binaire";
			// printf("--------%d\n", $$->condition_binaire.droite->type);
	}
;



binary_rel	:	
		LAND
	|	LOR
;

binary_comp	:	
		LT	{ $$ = strdup(yytext); }
	|	GT	{ $$ = strdup(yytext); }
	|	GEQ	{ $$ = strdup(yytext); }
	|	LEQ	{ $$ = strdup(yytext); }
	|	EQ	{ $$ = strdup(yytext); }
	|	NEQ	{ $$ = strdup(yytext); }
;
%%

void warn(char *s) {
	fprintf(stderr, COLOR_PURPLE);
	fprintf(stderr, "Warning: ");
	fprintf(stderr, RESET_COLOR);
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Warning at line %d\n", yylineno);
	fprintf(stderr, "Warning near: %s\n", yytext);
}

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
    yyparse();
    pop_table(); // nettoyage
	yylex_destroy();
	if (file != NULL) {
		fclose(file);
	}
    return 0;
}



/*
Node *result = chercher_variable($1);
if (result == NULL) {
	char *s = malloc( strlen("Variable utilisée mais jamais déclarée :") + strlen($1) + 1);
	sprintf(s, "Variable utilisée mais jamais déclarée : %s", $1);
	error(s); // -> kill
}
if (result->symbole.isInitialized == 0) {
	char *s = malloc( strlen("Variable utilisée mais jamais initialisée :") + strlen($1) + 1);
	sprintf(s, "Variable utilisée mais jamais initialisée : %s", $1);
	printf("------WARNING : Variable utilisée mais jamais initialisée : %s\n", $1);
	// WARNING -------------- A FAIRE
	// warning(s); // -> warning
}


*/