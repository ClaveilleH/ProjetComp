%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbre.h"
#include "genererDot.h"

void ouvrir_graphe();
void fermer_graphe();
int generer_dot_node(Node *node);

 

#define COLOR_RED "\033[31m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_GREEN "\033[32m"
#define RESET_COLOR "\033[0m"
#ifndef DEBUG
#define DEBUG 0
#endif

#define EMIT_WARNING(fmt, ...) do { \
    fprintf(stderr, COLOR_PURPLE "[Warning] " RESET_COLOR fmt "\n", ##__VA_ARGS__); \
    fprintf(stderr, "          at line %d, near '%s'\n", yylineno, yytext); \
} while(0) // while(0) pour que les différents printf soient dans le même bloc

#define EMIT_ERROR(fmt, ...) do { \
    fprintf(stderr, COLOR_RED "[Error] " RESET_COLOR fmt "\n", ##__VA_ARGS__); \
    fprintf(stderr, "        at line %d, near '%s'\n", yylineno, yytext); \
    exit(1); \
} while(0)

#define DEBUG_PRINT(fmt, ...) do { \
	if (DEBUG) { \
		fprintf(stderr, fmt , ##__VA_ARGS__); \
	} \
} while(0)

extern int yylineno;
extern char *yytext;
extern FILE *yyin;

int yylex(void);       
void yyerror(char *s);
void error(char *s);
void yylex_destroy(void);

FILE *file;


NodeList *liste_fonctions = NULL; // liste globale des fonctions A SUPP
Node *current_function = NULL; // fonction courante
int in_loop = 0; // dans une boucle
int in_switch = 0; // dans un switch
int in_case = 0; // dans un case
int mode_affectation = 0; // mode déclaration ou pas


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
%left REL


%start programme


/* %type <entier> expression */
%type <chaine> binary_comp
%type <type> type

%type <node> declarateur parm fonction variable affectation instruction selection saut condition expression bloc
%type <node> iteration ouverture_fonction switch_default switch_case
%type <node_list> liste_declarateurs declaration liste_parms liste_fonctions liste_instructions liste_expressions
%type <node_list> dimension_utilisation switch_liste_instructions liste_case
%type <node_table> liste_declarations 

%%
programme	:	
		liste_declarations liste_fonctions {
			DEBUG_PRINT("Programme complet\n");
			ouvrir_graphe();	// ouverture du fichier dot 

			DEBUG_PRINT("Programme :\n");
			DEBUG_PRINT("├── Déclarations globales\n");
			NodeList *tmp;
			for (int i = 0; i < TAILLE; i++) {
				if ($1[i] != NULL) {
					tmp = $1[i];
					while (tmp != NULL) {
						if (DEBUG) afficher_node2("│   ├──", tmp->node);
						tmp = tmp->suivant;
					}
				}
			}
			DEBUG_PRINT("└── Fonctions :\n");
			tmp = $2;
			

			while (tmp != NULL) {
				generer_dot_node(tmp->node);
				DEBUG_PRINT("    ├── %s\n", tmp->node->fonction.nom);
				DEBUG_PRINT("    │	│\n");
				DEBUG_PRINT("    │	├── Type : %s\n", tmp->node->fonction.type == ENTIER ? "int" : "void");
				NodeList *tmp2 = tmp->node->fonction.liste_parametres;
				DEBUG_PRINT("    │	├── Paramètres : (");
				while (tmp2 != NULL) {
					DEBUG_PRINT("%s, ", tmp2->node->parametre.nom);
					tmp2 = tmp2->suivant;
				}
				if (tmp->node->fonction.externe) {
					DEBUG_PRINT(") EXTERNE\n");
					tmp = tmp->suivant;
					continue;
				} 
				DEBUG_PRINT(")\n");
				if (DEBUG) afficher_node2("    │	", tmp->node->fonction.bloc);
				tmp = tmp->suivant;
			}
		
        fermer_graphe(); // fermeture fichier dot
        DEBUG_PRINT(">> Graphe DOT généré avec succès.\n");
		


		// free_liste(liste_fonctions); // on libère la liste de fonctions
		// free_list($1); // on libère la liste de déclarations
		// free_list($2); // on libère la liste de fonctions
		// free_all(); // on libère la table de symboles
		
		// free_table($1);
		// printf(COLOR_PURPLE "------------------------------------------------\n" RESET_COLOR);
		// free_list($2);
		}
;

liste_declarations	:	
		liste_declarations declaration {
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
							EMIT_ERROR("Variable already declared in this scope : %s", tmp->node->symbole.nom);
						}
						tmp2 = tmp2->suivant;
					}
					tmp->suivant = $$[h]; // on ajoute le noeud au début de la liste
					$$[h] = tmp; // on met à jour la table de hachage
					tmp = tmp->suivant;
				}
			}
		}
	| 	/* epsilon */ {
			$$ = creer_node_table(); 
		}
;

liste_fonctions	:	
		liste_fonctions fonction {
			// printf(COLOR_GREEN "1 : append_node : %s\n" RESET_COLOR, $2->fonction.nom);
			if (append_node($1, $2)) {
				EMIT_ERROR("Redeclaration de la fonction : %s", $2->fonction.nom);
			}
			$$ = $1;
		}
	|	fonction {
			$$ = nouveau_node_list($1);
			// printf(COLOR_GREEN "1 : Node LIST %d" RESET_COLOR "\n", $$->id);
		}
;

declaration	:	
		type liste_declarateurs ';' {
			NodeList *tmp = $2;
			while (tmp != NULL) {
				tmp = tmp->suivant;
			}
			$$ = $2;
		}
		// Déclaration avec initialisation
		|type liste_declarateurs '=' expression ';' {
			$$ = $2;
			NodeList *tmp = $2;
			while (tmp->suivant != NULL) { // on va a la derniere variable
				tmp = tmp->suivant;
			}
			tmp->node->symbole.valeur = $4->expression.valeur; // on affecte la valeur de l'expression à la variable
			tmp->node->symbole.evaluable = $4->expression.evaluable; // on met la variable comme évaluable
			tmp->node->symbole.isInitialized = 1; // on met la variable comme initialisée
		}
;

liste_declarateurs	:	
		liste_declarateurs ',' declarateur {
			// on ajoute declarateur au début de la liste
			// on verifie pas encore si la variable existe déjà
			// printf(COLOR_GREEN "2 : append_node : %s\n" RESET_COLOR, $3->symbole.nom);
			if (append_node($1, $3)) {
				EMIT_ERROR("Variable déjà déclarée dans cette portée : %s", $3->symbole.nom);
			}
		}
	|	declarateur {
		$$ = nouveau_node_list($1);
		// printf(COLOR_GREEN "2 : Node LIST %d" RESET_COLOR "\n", $$->id);
	}
;

declarateur:
    	IDENTIFICATEUR {
			// On crée un nouveau noeud pour la déclaration de variable
			// on verifie pas encore si la variable existe déjà
			$$ = nouveau_node(SYMBOLE);
        	$$->symbole.nom = $1;
			

			ajouter_variable($$); // on ajoute la variable à la table de symboles courante
		}
	|	declarateur '[' CONSTANTE ']' {
			// on modifie le noeud de la déclaration de variable
			$$ = $1;
			$$->symbole.type = TABLEAU;
			$$->symbole.dimension += 1; // on incremente la dimension du tableau
			$$->symbole.isInitialized = 1; // on met initialisée parce que ce n'est pas encore géré
		}
;

ouverture_fonction:
	{
		// dans le cas d'une fonction reccursive il faut ajouter la fonction à la table de symboles
		// avant de l'initialiser, sinon on ne peut pas l'utiliser dans le corps de la fonction
		push_table(); // ouverture de bloc
		$$ = nouveau_node(FONCTION);
		$$->fonction.nom = $<chaine>-2;
		$$->fonction.type = $<type>-4;
		$$->fonction.liste_parametres = $<node_list>-1;
		$$->fonction.bloc = NULL;
		$$->fonction.externe = 0; // on met la fonction comme interne
		ajouter_fonction($$); // on ajoute la fonction à la table de symboles courante
		current_function = $$; // on initialise la fonction courante pour verifier les types de return
	}


fonction:
    type IDENTIFICATEUR '(' liste_parms ')' ouverture_fonction bloc {
			$$ = $6;
			$$->fonction.type = $1;
			$$->fonction.bloc = $7;
			// printf(COLOR_GREEN "3 : append_node : %s\n" RESET_COLOR, $$->fonction.nom);
			append_node(liste_fonctions, $$);
			pop_table(); // fermeture de bloc
			current_function = NULL; // on remet la fonction courante à NULL
		}
  	| EXTERN type IDENTIFICATEUR '(' liste_parms ')' ';' {
			$$ = nouveau_node(FONCTION);
			$$->fonction.nom = $3;
			$$->fonction.type = $2;
			$$->fonction.liste_parametres = $5;
			$$->fonction.bloc = NULL;
			$$->fonction.externe = 1; // on met la fonction comme externe

			// printf(COLOR_GREEN "4 : append_node : %s\n" RESET_COLOR, $$->fonction.nom);
			append_node(liste_fonctions, $$); //! VERIFIER L'UTILITÉ
			ajouter_fonction($$); // on ajoute la fonction à la table de symboles courante
		}
;


type: 
		VOID { $$ = VOID_TYPE; }
    | 	INT  { $$ = ENTIER; }
;


liste_parms	:	
		parm	{
			$$ = nouveau_node_list($1);
			// printf(COLOR_GREEN "3 : Node LIST %d" RESET_COLOR "\n", $$->id);
		}
	|	liste_parms ',' parm	{
			$$ = $1;
			// printf(COLOR_GREEN "4 : append_node : %s\n" RESET_COLOR, $3->parametre.nom);
			if (append_node($$, $3)) {
				EMIT_ERROR("Paramètre déjà déclaré dans cette fonction : %s", $3->parametre.nom);
			}
		}
	|   /* vide */ { $$ = NULL; }	
;

parm	: 
	INT IDENTIFICATEUR {
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
				// printf(COLOR_GREEN "4 : Node LIST %d" RESET_COLOR "\n", $$->id);			
			} else {
				// on ajoute instruction au début de la liste
				// on verifie pas encore si la variable existe déjà
				$$ = $1;
				NodeList *nouv = nouveau_node_list($2);
				// printf(COLOR_GREEN "5 : Node LIST %d" RESET_COLOR "\n", nouv->id);
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
		}
	|   /* epsilon */ {
			$$ = NULL; 
		}
;

instruction:
    affectation ';'			 { 
		$$ = $1;
	}
  	| expression ';'           {
		$$ = $1;
	}
	| iteration				 {
		$$ = $1;
	}
	| selection 			 {
		$$ = $1;
	}
	| saut					 {
		$$ = $1;
	}
	| bloc	 				 {
		$$ = $1;
	}
;

debut_boucle:
	{
		in_loop = 1;
	}

iteration	:	
		FOR '(' affectation ';' condition ';' affectation ')' debut_boucle instruction  {
			$$ = nouveau_node(FOR_NODE);
			$$->for_node.init = $3;
			$$->for_node.condition = $5;
			$$->for_node.incr = $7;
			$$->for_node.instruction = $10;
			in_loop = 0;
		}
	|	WHILE '(' condition ')' debut_boucle instruction	{
			$$ = nouveau_node(WHILE_NODE);
			$$->while_node.condition = $3;
			$$->while_node.instruction = $6;
			in_loop = 0;
	}
;

selection	:	
		IF '(' condition ')' instruction %prec THEN 	    {
			$$ = nouveau_node(IF_NODE);
			$$->if_node.instruction = $5;
			$$->if_node.condition = $3;
		}
	|	IF '(' condition ')' instruction ELSE instruction	{
			$$ = nouveau_node(IF_ELSE_NODE);
			$$->if_else_node.instruction = $5;
			$$->if_else_node.condition = $3;
			$$->if_else_node.instruction_else = $7;
		}
	|	SWITCH '(' expression ')' { in_switch = 1; } '{' switch_liste_instructions '}'	{
			$$ = nouveau_node(SWITCH_NODE);
			$$->switch_node.expression = $3;
			$$->switch_node.liste_instructions = $7;
			in_switch = 0;
		}
;

switch_liste_instructions :
		liste_case switch_default {
			// on ajoute switch_instructions a la fin de la liste
			append_node($1, $2);
			$$ = $1;
		}
	|	liste_case {
			$$ = $1;
		}
	|	switch_default {
			$$ = nouveau_node_list($1);
		}
;

liste_case :
		liste_case switch_case {
			// on ajoute switch_case a la fin de la liste
			// append_node($1, $2);
			if (append_node($1, $2)) {
				EMIT_ERROR("Case avec la même constante déjà déclarée : %d", $2->case_node.constante->expression.valeur);
			}
			$$ = $1;
		}
	|	switch_case {
			$$ = nouveau_node_list($1);
		}
;

switch_case :
		CASE CONSTANTE ':' {in_case = 1;} liste_instructions {
			if (in_switch == 0) {
				EMIT_ERROR("CASE hors d'un switch");
			}
			$$ = nouveau_node(CASE_NODE);
			$$->case_node.liste_instructions = $5;
			Node *cst = nouveau_node(EXPRESSION);
			cst->expression.type = EXPRESSION_CONSTANTE;
			cst->expression.valeur = $2;
			cst->expression.evaluable = 1;
			$$->case_node.constante = cst;
			in_case = 0;
		}
;

switch_default :
		DEFAULT ':' liste_instructions {
			if (in_switch == 0) {
				EMIT_ERROR("DEFAULT hors d'un switch");
			}
			$$ = nouveau_node(DEFAULT_NODE);
			$$->default_node.liste_instructions = $3;
		}
;

/*
|	SWITCH '(' expression ')' { in_switch = 1; } instruction				{
			$$ = nouveau_node(SWITCH_NODE);
			$$->switch_node.expression = $3;
			$$->switch_node.instruction = $6;
			in_switch = 0;
		}
*/

/*
	|	CASE CONSTANTE ':' {in_case = 1;} instruction {
			$$ = nouveau_node(CASE_NODE);
			$$->case_node.instruction = $5;
			Node *cst = nouveau_node(EXPRESSION);
			cst->expression.type = EXPRESSION_CONSTANTE;
			cst->expression.valeur = $2;
			cst->expression.evaluable = 1;
			$$->case_node.constante = cst;
			in_case = 0;
			if (in_switch == 0) {
				EMIT_ERROR("CASE hors d'un switch");
			}
		}
	|	DEFAULT ':' instruction		{
			if (in_switch == 0) {
				EMIT_ERROR("DEFAULT hors d'un case");
			}
			$$ = nouveau_node(DEFAULT_NODE);
			$$->default_node.instruction = $3;
		}
		*/

saut	:	
		BREAK ';'	{
			$$ = nouveau_node(BREAK_NODE);
			if (in_loop == 0 && in_switch == 0) {
				EMIT_ERROR("BREAK hors d'une boucle");
			}
		}
	|	RETURN ';'	{
			$$ = nouveau_node(RETURN_NODE);
			$$->return_node.expression = NULL;
			if (current_function == NULL) {
				EMIT_ERROR("RETURN sans valeur dans une fonction void");
			}
			if (current_function->fonction.type == ENTIER) {
				EMIT_ERROR("RETURN sans valeur dans une fonction int");
			}
	}
	|	RETURN expression ';'	{
			$$ = nouveau_node(RETURN_NODE);
			$$->return_node.expression = $2;
			if (current_function == NULL) {
				EMIT_ERROR("RETURN avec valeur dans une fonction void");
			}
			if (current_function->fonction.type == VOID_TYPE) {
				EMIT_ERROR("RETURN avec valeur dans une fonction void");
			}
	}
;

affectation	: // try
		
		variable '=' expression { // on véifie si expression contient une variable non initialisée
			char *nom;
			if (verifier_initialisation_expression($3, &nom)) {
				EMIT_WARNING("Variable utilisée sans être initialisée : %s", nom);
			}
			$1->symbole.isInitialized = 1; // on met la variable comme initialisée
			int eval;
			int res;
			evaluer_expression($3, &res, &eval);
			if ($3->expression.evaluable == 1) {
				$1->symbole.valeur = $3->expression.valeur; // on affecte la valeur de l'expression à la variable
				$1->symbole.evaluable = 1; // on met la variable comme évaluable
			} else {
				$1->symbole.evaluable = 0; // on met la variable comme non évaluable
			}
			$$ = nouveau_node(AFFECTATION);
			$$->affectation.variable = $1;
			$$->affectation.expression = $3;
		}
		
;

opn_bloc :
		'{' {
			push_table(); // ouverture de bloc
		}
;

close_bloc :
		'}' {
			pop_table(); // fermeture de bloc
		}
;

bloc : 
	opn_bloc liste_declarations liste_instructions close_bloc {
			$$ = nouveau_node(BLOC);
			$$->bloc.table_declarations = $2;
			$$->bloc.liste_instructions = $3;
        }
;


variable	:	// quand on utilise une variable
		IDENTIFICATEUR {
			/* On vérifie si la variable existe dans la table de symboles */
			Node *result = chercher_symbole($1);
			if (result == NULL) {
				size_t alloc_len = strlen("Variable utilisée mais jamais déclarée :") + strlen($1) + 10; // Ajoutez une marge de sécurité // try
				char *s = malloc(alloc_len);
				if (s == NULL) {
					fprintf(stderr, "Erreur : allocation mémoire échouée\n");
					exit(EXIT_FAILURE);
				}
				EMIT_ERROR("Variable utilisée mais jamais déclarée : %s", $1);
			}
			if (result->type == FONCTION) {
				EMIT_ERROR("Fonction utilisée comme variable");
			}
			if (result->symbole.type == TABLEAU) {
				EMIT_ERROR("Variable tableau utilisée comme int");
			}
			$$ = result;
		}
	|	IDENTIFICATEUR dimension_utilisation {
		// on verifie si la variable existe
		int cpt = 0;
		Node *result = chercher_symbole($1);
		if (result == NULL) {
			EMIT_ERROR("Variable utilisée mais jamais déclarée : %s", $1);
		}
		if (result->type == FONCTION) {
			EMIT_ERROR("Fonction utilisée comme variable : %s", $1);
		}
		if (result->symbole.type != TABLEAU) {
			EMIT_ERROR("Variable int utilisée comme tableau : %s", $1);
		}
		NodeList *tmp = $2;
		while (tmp != NULL) {
			cpt++;
			tmp = tmp->suivant;
		}
		if (cpt != result->symbole.dimension) {
			EMIT_ERROR("Nombre de dimensions incorrect : %s", $1);
		}
		$$ = nouveau_node(ACCES_TABLEAU);
		$$->acces_tableau.variable = result;
		$$->acces_tableau.liste_expressions = $2;
		


	}
;

dimension_utilisation:
		dimension_utilisation '[' expression ']' {
			$$ = $1;
			// on ajoute l'expression à la liste d'expressions
			// printf(COLOR_GREEN "5 : append_node : %d\n" RESET_COLOR, $3->id);
			append_node($$, $3);
		}
	|	'[' expression ']' {
			$$ = nouveau_node_list($2);
			// printf(COLOR_GREEN "7 : Node LIST %d" RESET_COLOR "\n", $$->id);
		}


expression	:		
		'(' expression ')' 			  			{ 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_PARENTHESE;
			$$->expression.expression = $2;
			$$->expression.evaluable = $2->expression.evaluable;
			$$->expression.valeur = $2->expression.valeur;
		}
	| 	MOINS expression %prec MOINSUNAIRE 		{ 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_MOINS_UNAIRE;
			$$->expression.expression = $2;
			$$->expression.evaluable = $2->expression.evaluable;
		}
	| 	expression PLUS expression      			{ 
			$$ = construire_expr_binaire($1, $3, "+");
		}
    | 	expression MOINS expression     			{ 
			$$ = construire_expr_binaire($1, $3, "-");
		}
    | 	expression MUL expression       			{ 
			$$ = construire_expr_binaire($1, $3, "*");

		}
    | 	expression DIV expression       			{ 
			int res = 1;
			int eval;
			evaluer_expression($3, &res, &eval);
			if (eval && res == 0) {
				EMIT_WARNING("Division par zéro");
			}
			$$ = construire_expr_binaire($1, $3, "/");
		}
	|	expression LSHIFT expression 			{ 
			$$ = construire_expr_binaire($1, $3, "<<");
		}
	| 	expression RSHIFT expression 			{ 
			$$ = construire_expr_binaire($1, $3, ">>");
		}
	| 	expression BAND expression       			{ 
			$$ = construire_expr_binaire($1, $3, "&");
		}
	| 	expression BOR expression       			{ 
			$$ = construire_expr_binaire($1, $3, "|");
		}
	| 	CONSTANTE              		  			{ 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_CONSTANTE;
			$$->expression.valeur = $1;
			$$->expression.evaluable = 1;
		}
	| 	variable 						  			{ 
			$$ = $1;

		}
	|	IDENTIFICATEUR '(' liste_expressions ')' 	{ //appel de fonction 
			// on verifie si la fonction existe
			// on verifie si le nombre d'arguments est correct
			// si la fonction est recursive 
			Node *result = chercher_fonction($1);
			if (result == NULL) {
				EMIT_ERROR("Fonction utilisée mais jamais déclarée : %s", $1);
			}
			if (result->type != FONCTION) {
				EMIT_ERROR("Variable utilisée comme fonction : %s", $1);
			}
			if (result->type == FONCTION) {
				if (result->fonction.type == VOID_TYPE) {
					EMIT_ERROR("Fonction void utilisée comme int : %s", $1);
				}

				// on verifie si le nombre d'arguments est correct
				NodeList *tmp = $3;
				NodeList *tmp2 = result->fonction.liste_parametres;
				int i = 0;
				while (tmp != NULL && tmp2 != NULL) {
					tmp = tmp->suivant;
					tmp2 = tmp2->suivant;
					i++;
				}
				if (tmp != NULL || tmp2 != NULL) {
					// EMIT_ERROR("Nombre d'arguments incorrect : %s, attendu %d, trouvé %d", $1, result->fonction.nombre_parametres, i);
					EMIT_ERROR("Nombre d'arguments incorrect : %s", $1);
				}
				NodeList *indices = $3; // try
				char *nom;
            	while (indices) {
                	if (verifier_initialisation_expression(indices->node, &nom)) {
						EMIT_WARNING("Variable utilisée sans être initialisée : %s", nom);
					}
                	indices = indices->suivant;
            	}
			}
			$$ = nouveau_node(APPEL_FONCTION);
			$$->appel_fonction.nom = $1;
			$$->appel_fonction.liste_expressions = $3;
		}
;

liste_expressions	:	
		liste_expressions ',' expression { 
			// printf("Liste d'expressions \n");
			$$ = $1;
			append_node($$, $3);
		}
	|	expression { 
			$$ = nouveau_node_list($1);
	}
	|	/* epsilon */ { 
			$$ = NULL; 
	}
;

condition	:	
		NOT '(' condition ')' { 
			$$ = nouveau_node(CONDITION_NOT);
			$$->condition_not.condition = $3;
		}
	|	'(' condition ')'  {
			$$ = nouveau_node(CONDITION_PARENTHESE);
			$$->condition_parenthese.condition = $2;
	}
	|	expression binary_comp expression  { 
			// printf("Condition binaire\n");
			$$ = nouveau_node(CONDITION_BINAIRE);
			$$->condition_binaire.gauche = $1;
			$$->condition_binaire.droite = $3;
			$$->condition_binaire.operateur = $2;
	}
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

void yyerror(char *s) {
	fprintf(stderr, COLOR_RED "[Error] "RESET_COLOR);
	fprintf(stderr, "%s\n", s);
    fprintf(stderr, "        at line %d, near '%s'\n", yylineno, yytext); \
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
	/* free_all(); // nettoyage */
	yylex_destroy();
	if (file != NULL) {
		fclose(file);
	}
	
    return 0;
}