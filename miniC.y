%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symboles.h"
#include "genererDot.h"

void ouvrir_graphe();
void fermer_graphe();
int generer_dot_node(Node *node);

 

#define COLOR_RED "\033[31m"
#define COLOR_PURPLE "\033[35m"
#define COLOR_GREEN "\033[32m"
#define RESET_COLOR "\033[0m"
#define DEBUG 1

#define EMIT_WARNING(fmt, ...) do { \
    fprintf(stderr, COLOR_PURPLE "[Warning] " RESET_COLOR fmt "\n", ##__VA_ARGS__); \
    fprintf(stderr, "          at line %d, near '%s'\n", yylineno, yytext); \
} while(0)

extern int yylineno;
extern char *yytext;
extern FILE *yyin;

int yylex(void);       
void yyerror(char *s);
void error(char *s);
void warn(char *s);
void yylex_destroy(void);

FILE *file;


NodeList *liste_fonctions = NULL; // liste globale des fonctions A SUPP
Node *current_function = NULL; // fonction courante
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
%left OP
%left REL


%start programme


/* %type <entier> expression */
%type <chaine> binary_comp
%type <type> type

%type <node> declarateur parm fonction variable affectation instruction selection saut condition expression bloc
%type <node> iteration ouverture_fonction
%type <node_list> liste_declarateurs declaration liste_parms liste_fonctions liste_instructions liste_expressions
%type <node_table> liste_declarations 

%%
programme	:	
		liste_declarations liste_fonctions {
			// └
			printf("Programme complet\n");
			ouvrir_graphe();	// ouverture du fichier dot 

			if (DEBUG) printf("Programme :\n");
			if (DEBUG) printf("├── Déclarations globales\n");
			NodeList *tmp;
			for (int i = 0; i < TAILLE; i++) {
				if ($1[i] != NULL) {
					tmp = $1[i];
					while (tmp != NULL) {
						// printf("│   ├── %s\n", tmp->node->symbole.nom);
						if (DEBUG) afficher_node2("│   ├──", tmp->node);
						// Génération du graphe pour chaque déclaration globale
                    	// generer_dot_node(tmp->node);

						tmp = tmp->suivant;
					}
				}
			}
			if (DEBUG) printf("└── Fonctions :\n");
			tmp = $2;
			

			while (tmp != NULL) {
				generer_dot_node(tmp->node);
				if (DEBUG) printf("    ├── %s\n", tmp->node->fonction.nom);
				if (DEBUG) printf("    │	│\n");
				if (DEBUG) printf("    │	├── Type : %s\n", tmp->node->fonction.type == ENTIER ? "int" : "void");
				NodeList *tmp2 = tmp->node->fonction.liste_parametres;
				if (DEBUG) printf("    │	├── Paramètres : (");
				while (tmp2 != NULL) {
					if (DEBUG) printf("%s, ", tmp2->node->parametre.nom);
					tmp2 = tmp2->suivant;
				}
				if (tmp->node->fonction.externe) {
					if (DEBUG) printf(") EXTERNE\n");
					tmp = tmp->suivant;

					continue;
				} 
				if (DEBUG) printf(")\n");
				if (DEBUG) afficher_node2("    │	", tmp->node->fonction.bloc);
				tmp = tmp->suivant;
				
			}
		
        fermer_graphe(); // fermeture fichier dot
        printf(">> Graphe DOT généré avec succès.\n");
		// free_liste(liste_fonctions); // on libère la liste de fonctions
		// free_list($1); // on libère la liste de déclarations
		// free_list($2); // on libère la liste de fonctions
		// free_all(); // on libère la table de symboles
		
		// free_table($1);
		// free_list($2);
		}
;

liste_declarations	:	
		liste_declarations declaration {
			// printf("Liste de déclarations\n");
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
		}
	| /* epsilon */ {
			// printf("Liste de déclarations vide\n"); 
			$$ = creer_node_table(); 
			// printf("Table de symboles initialisée\n");
		}
;

liste_fonctions	:	
		liste_fonctions fonction {
			if (append_node($1, $2)) {
				char *s = malloc( strlen("Redeclaration de la fonction :") + strlen($2->fonction.nom) + 1);
				sprintf(s, "Redeclaration de la fonction : %s", $2->fonction.nom);
				error(s);
			}
			$$ = $1;
		}
	|	fonction {
			$$ = nouveau_node_list($1);
			// printf(COLOR_GREEN "1 : Node LIST %d\n" RESET_COLOR, $$->id);
				
		}
;

declaration	:	
		type liste_declarateurs ';' {
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
			append_node($1, $3);
		}
	|	declarateur {
		$$ = nouveau_node_list($1);
	}
;

declarateur:
    	IDENTIFICATEUR {
			// On crée un nouveau noeud pour la déclaration de variable
			// on verifie pas encore si la variable existe déjà
			// printf("Déclaration de variable : %s, type :%d\n", $1, $<type>-2);
						
			$$ = nouveau_node(SYMBOLE);
			$$->symbole.nom = $1;
			$$->symbole.type = ENTIER;
			$$->symbole.valeur = 0; // valeur initiale à 0 
			$$->symbole.isInitialized = 0; // on met la variable comme non initialisée
			$$->symbole.evaluable = 0; // on met la variable comme non évaluable
			$$->symbole.dimension = 0; // dimension du tableau
			// $$->symbole.taille = 0; // taille du tableau

			// TODO: mettre un param .isInitialized à 0
			
			ajouter_variable($$); // on ajoute la variable à la table de symboles courante
			//? mettre la verification ici ?
		}
	|	declarateur '[' CONSTANTE ']' {
			// on modifie le noeud de la déclaration de variable
			$$ = $1;
			$$->symbole.type = TABLEAU;
			$$->symbole.dimension += 1; // on incremente la dimension du tableau
			// $$->symbole.taille = $3; // taille du tableau
			//la taille ne sert a rien pour l'instant
			// il faut faire une liste de tailles selon la dimension
		}
;

ouverture_fonction:
	{
		// dans le cas d'une fonction reccursive il faut ajouter la fonction à la table de symboles
		// avant de l'initialiser
		// sinon on ne peut pas l'utiliser dans le corps de la fonction
		push_table(); // ouverture de bloc
		$$ = nouveau_node(FONCTION);
		$$->fonction.nom = $<chaine>-2;
		$$->fonction.type = $<type>-4;
		$$->fonction.liste_parametres = $<node_list>-1;
		$$->fonction.bloc = NULL;
		$$->fonction.externe = 0; // on met la fonction comme interne
		ajouter_fonction($$); // on ajoute la fonction à la table de symboles courante
		current_function = $$; // on met la fonction courante
	}


fonction:
    type IDENTIFICATEUR '(' liste_parms ')' ouverture_fonction bloc {
			$$ = $6;
			$$->fonction.type = $1;
			$$->fonction.bloc = $7;

			// on verifie si la fonction a un return
			

			append_node(liste_fonctions, $$);
			//ajouter_fonction($$); //fait a l'ouverture de bloc
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

			append_node(liste_fonctions, $$); //! VERIFIER L'UTILITÉ
			ajouter_fonction($$); // on ajoute la fonction à la table de symboles courante
			//! ATTENION EXTERNE
		}
;


type: VOID { $$ = VOID_TYPE; }
    | INT  { $$ = ENTIER; }
;


liste_parms	:	
		parm	{
			$$ = nouveau_node_list($1);
			// printf(COLOR_GREEN "3 : Node LIST %d\n" RESET_COLOR, $$->id);
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
				// printf(COLOR_GREEN "4 : Node LIST %d\n" RESET_COLOR, $$->id);
				
			} else {
				// on ajoute instruction au début de la liste
				// on verifie pas encore si la variable existe déjà
				$$ = $1;
				NodeList *nouv = nouveau_node_list($2);
				// printf(COLOR_GREEN "5 : Node LIST %d\n" RESET_COLOR, $$->id);
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
			// printf("Liste d'instructions vide\n"); 
			$$ = NULL; 
		}
;

instruction:
    affectation ';'			 { 
		// printf("Instruction : affectation\n"); 
		$$ = $1;
	}
  	| expression ';'           { 
		// printf("Instruction : expression seule\n");
		$$ = $1;
	}
	| iteration				 { 
		// printf("Instruction : boucle\n"); 
		$$ = $1;
	}
	| selection 			 { 
		// printf("Instruction : selection\n"); 
		$$ = $1;
	}
	| saut					 { 
		// printf("Instruction : saut\n"); 
		$$ = $1;
	}
	| bloc	 				 { 
		// printf("Instruction : bloc\n"); 
		$$ = $1;
	}
;


iteration	:	
		FOR '(' affectation ';' condition ';' affectation ')' instruction  {
			// printf("Boucle FOR\n");
			$$ = nouveau_node(FOR_NODE);
			$$->for_node.init = $3;
			$$->for_node.condition = $5;
			$$->for_node.incr = $7;
			$$->for_node.instruction = $9;
		}
	|	WHILE '(' condition ')' instruction	{
			// printf("Boucle WHILE\n");
			$$ = nouveau_node(WHILE_NODE);
			$$->while_node.condition = $3;
			$$->while_node.instruction = $5;
	}
;

selection	:	
		IF '(' condition ')' instruction %prec THEN 	    { 
			// printf("Instruction IF\n");
			$$ = nouveau_node(IF_NODE);
			$$->if_node.instruction = $5;
			$$->if_node.condition = $3;
		}
	|	IF '(' condition ')' instruction ELSE instruction	{
			// printf("Selection IF ... ELSE\n"); 
			$$ = nouveau_node(IF_ELSE_NODE);
			$$->if_else_node.instruction = $5;
			$$->if_else_node.condition = $3;
			$$->if_else_node.instruction_else = $7;
		}
	|	SWITCH '(' expression ')' instruction				{
			// printf("Selection SWITCH\n"); 
			$$ = nouveau_node(SWITCH_NODE);
			$$->switch_node.expression = $3;
			$$->switch_node.instruction = $5;
		}
	|	CASE CONSTANTE ':' instruction						{
			// printf("Selection CASE %d\n", $2); 
			$$ = nouveau_node(CASE_NODE);
			$$->case_node.instruction = $4;
			Node *cst = nouveau_node(EXPRESSION);
			cst->expression.type = EXPRESSION_CONSTANTE;
			cst->expression.valeur = $2;
			cst->expression.evaluable = 1;
			$$->case_node.constante = cst;
		}
	|	DEFAULT ':' instruction								{
			// printf("Selection DEFAULT\n");
			$$ = nouveau_node(DEFAULT_NODE);
			$$->default_node.instruction = $3;
		}
;

saut	:	
		BREAK ';'	{
			// printf("Instruction BREAK\n"); 
			$$ = nouveau_node(BREAK_NODE);
		}
	|	RETURN ';'	{
			// printf("Instruction RETURN sans valeur\n"); 
			$$ = nouveau_node(RETURN_NODE);
			$$->return_node.expression = NULL;
			if (current_function == NULL) {
				yyerror("RETURN sans valeur dans une fonction void");
			}
			if (current_function->fonction.type == ENTIER) {
				yyerror("RETURN sans valeur dans une fonction int");
			}
	}
	|	RETURN expression ';'	{
			// printf("Instruction RETURN avec valeur %d\n", $2); 
			$$ = nouveau_node(RETURN_NODE);
			$$->return_node.expression = $2;
			if (current_function == NULL) {
				yyerror("RETURN avec valeur dans une fonction void");
			}
			if (current_function->fonction.type == VOID_TYPE) {
				yyerror("RETURN avec valeur dans une fonction void");
			}
	}
;

affectation	: // try
		
		variable '=' expression { // on véifie si expression contient une variable non initialisée
			if (verifier_initialisation_expression($3)){EMIT_WARNING("Variable utilisée sans être initialisée");}
			$1->symbole.isInitialized = 1; // on met la variable comme initialisée
			if ($3->expression.evaluable == 1) {
				$1->symbole.valeur = $3->expression.valeur; // on affecte la valeur de l'expression à la variable
				$1->symbole.evaluable = 1; // on met la variable comme évaluable
			} else {
				$1->symbole.evaluable = 0; // on met la variable comme non évaluable
			}
			$$ = nouveau_node(AFFECTATION);
			$$->affectation.variable = $1;
			$$->affectation.expression = $3;
			//! ATTENTION : IL FAUT EVALUER L'EXPRESSION
			// PENSER A REDUIRE L'EXPRESSION
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
			// printf(COLOR_GREEN);
			// printf("Bloc :\n");
			// printf(RESET_COLOR);
        }
;


variable	:	// quand on utilise une variable
		IDENTIFICATEUR { /*$$ = inserer($1);*/ //jpense pas qu'il faille utiliser ca ici
			// il faut verif si ca existe déjà
			// sinon on leve une erreur
			Node *result = chercher_symbole($1);
			if (result == NULL) {
				size_t alloc_len = strlen("Variable utilisée mais jamais déclarée :") + strlen($1) + 10; // Ajoutez une marge de sécurité // try
				char *s = malloc(alloc_len);
				if (s == NULL) {
					fprintf(stderr, "Erreur : allocation mémoire échouée\n");
					exit(EXIT_FAILURE);
				}
				snprintf(s, alloc_len, "Variable utilisée mais jamais déclarée : %s", $1);
				error(s); // Appel de la fonction error
			}
			// je crois que ce n'est pas ici qu'il faut tester l'initialisation d'une variable car quand elle est déclarée elle n'est pas forcément initialisée
			// if (result->type == SYMBOLE && result->symbole.isInitialized == 0) {
			//	EMIT_WARNING("Variable '%s' utilisée sans être initialisée", result->symbole.nom);
			// } 
			 if (result->type == FONCTION) {
				yyerror("Fonction utilisée comme variable");
			}

			$$ = result;
		}
	|	variable '[' expression ']' {
		$$ = $1;
		// on verifie si la variable est un acces tableau ou un symbole
		if ($1->type == SYMBOLE) {
			// on verifie si la variable est un tableau
			if ($1->symbole.type != TABLEAU) {
				yyerror("Variable non déclarée comme tableau");
			}
			$$ = nouveau_node(ACCES_TABLEAU);
			$$->acces_tableau.variable = $1;
			$$->acces_tableau.liste_expressions = nouveau_node_list($3);
		} else if ($1->type == ACCES_TABLEAU) {
			$$ = $1;
			// on ajoute l'expression à la liste d'expressions
			append_node($1->acces_tableau.liste_expressions, $3);
		} else {
			yyerror("Variable non déclarée comme tableau");
		}


	}
;

expression	:		
		'(' expression ')' 			  			{ 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_PARENTHESE;
			$$->expression.expression = $2;
			$$->expression.evaluable = $2->expression.evaluable;
			$$->expression.valeur = $2->expression.valeur;
		}
	| MOINS expression %prec MOINSUNAIRE 		{ 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_MOINS_UNAIRE;
			$$->expression.expression = $2;
			$$->expression.evaluable = $2->expression.evaluable;
			// if ($2->expression.evaluable) {
			// 	$$->expression.evaluable = 1;
			// 	$$->expression.valeur = -$2->expression.valeur;
			// } else {
			// 	$$->expression.evaluable = 0;
			// }
		}
	| expression PLUS expression      			{ 
			$$ = construire_expr_binaire($1, $3, "+");
			// $$ = reduire_expression($$);
		}
    | expression MOINS expression     			{ 
			$$ = construire_expr_binaire($1, $3, "-");
			// $$ = reduire_expression($$);
		}
    | expression MUL expression       			{ 
			$$ = construire_expr_binaire($1, $3, "*");
			// $$ = reduire_expression($$);

		}
    | expression DIV expression       			{ 
			int res;
			int eval;
			evaluer_expression($3, &res, &eval);
			if (eval && res == 0) {
				// warn("Division par zéro");
				EMIT_WARNING("Division par zéro");
			}
			$$ = construire_expr_binaire($1, $3, "/");
			// $$ = reduire_expression($$);
		}
	|	expression LSHIFT expression 			{ 
			$$ = construire_expr_binaire($1, $3, "<<");
			// $$ = reduire_expression($$);
			// j'utilise '<' parce que je peux pas utiliser '<<'
			
		}
	| expression RSHIFT expression 			{ 
			$$ = construire_expr_binaire($1, $3, ">>");
			// j'utilise '>' parce que je peux pas utiliser '>>'
			// $$ = reduire_expression($$);
		}
	| expression BAND expression       			{ 
			$$ = construire_expr_binaire($1, $3, "&");
			// $$ = reduire_expression($$);
		}
	| expression BOR expression       			{ 
			$$ = construire_expr_binaire($1, $3, "|");
			// $$ = reduire_expression($$);
		}
	| CONSTANTE              		  			{ 
			$$ = nouveau_node(EXPRESSION);
			$$->expression.type = EXPRESSION_CONSTANTE;
			$$->expression.valeur = $1;
			$$->expression.evaluable = 1;
		}
	| variable 						  			{ 
			// $$ = $1->symbole.valeur; printf("Variable : %s = %d\n", $1->symbole.nom, $1->symbole.valeur); 
			$$ = $1;
			// printf("Variable : %s \n", $1->symbole.nom);

		}
	| IDENTIFICATEUR '(' liste_expressions ')' 	{ //appel de fonction 
			// on verifie si la fonction existe
			// on verifie si le nombre d'arguments est correct
			// si la fonction est recursive 
			Node *result = chercher_fonction($1);
			if (result == NULL) {
				char *s = malloc( strlen("Fonction non déclarée :") + strlen($1) + 1);
				sprintf(s, "Fonction non déclarée : %s", $1);
				error(s); // -> kill
			}
			if (result->type != FONCTION) {
				yyerror("Identificateur utilisé comme fonction");
			}
			if (result->type == FONCTION) {
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
					yyerror("Nombre d'arguments incorrect");
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
			// printf("Expression unique \n");
			$$ = nouveau_node_list($1);
			// printf(COLOR_GREEN "6 : Node LIST %d\n" RESET_COLOR, $$->id);
	}
	|	/* epsilon */ { 
			// printf("Liste d'expressions vide\n");
			$$ = NULL; 
	}
;

condition	:	
		NOT '(' condition ')' { 
			printf("Condition NOT\n"); 
		}
	|	condition binary_rel condition %prec REL {
		printf("Condition relationnelle\n");
		//JSP CE QUE C'EST
	}
	|	'(' condition ')'  {
		printf("Condition\n"); 
	}
	|	expression binary_comp expression  { 
			// printf("Condition binaire\n");
			$$ = nouveau_node(CONDITION_BINAIRE);
			$$->condition_binaire.gauche = $1;
			$$->condition_binaire.droite = $3;
			$$->condition_binaire.operateur = $2;
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
	/* free_all(); // nettoyage */
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