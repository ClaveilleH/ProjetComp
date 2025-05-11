#ifndef SYMBOLES_H
#define SYMBOLES_H
#include <stdlib.h>

#define TAILLE 103  //nombre premier 


typedef enum { ENTIER, VOID_TYPE } type_t;


typedef enum { 
    SYMBOLE, FONCTION, PARAMETRE,
    IF_NODE, BREAK_NODE, RETURN_NODE, 
    CONDITION_BINAIRE, CONDITION_UNAIRE, CONDITION_NOT,
    EXPRESSION,
    TEST, 
} NodeType;
typedef struct NodeList NodeList; // declaration anticipée
typedef struct Node Node; // declaration anticipée

typedef enum { EXPRESSION_BINAIRE, EXPRESSION_MOINS_UNAIRE, EXPRESSION_PARENTHESE, EXPRESSION_CONSTANTE, } ExpressionType;
typedef struct Node { //! est-ce qu'il faut pas faire des nodes pour les expressions separément ?
    NodeType type;
    union {
        struct { char *nom; type_t type; int taille; int position; int isInitialized; int valeur; int evaluable; } symbole; // changer le nom
        struct { char *nom; type_t type; NodeList *liste_parametres; NodeList **table_declarations; NodeList *liste_instructions; } fonction;
        struct { char *nom; type_t type; } parametre;

        struct { Node *condition; Node *instruction; } if_node;
        struct {} break_node;
        struct {} return_node;

        struct {Node *gauche; Node *droite; char *operateur; } condition_binaire;
        struct {} condition_unaire;
        struct {} condition_not;

        struct { 
            ExpressionType type;
            int valeur; 
            int evaluable;
            char *operateur; // pour les expressions binaires
            Node *gauche; // pour les expressions binaires
            Node *droite; // pour les expressions binaires
            Node *expression; 
         } expression;

        struct {} appel_fonction;

        struct { char *txt; } test;
    };
} Node;

typedef struct NodeList {
    Node *node;
    struct NodeList *suivant;
    struct NodeList *precedent; // pour les instructions, l'ordre est important
} NodeList;

typedef struct NodePile {
    NodeList **node; // tableau de listes chaînées TODO: changer le nom
    struct NodePile *suivant;
} NodePile;

Node *nouveau_node(NodeType type);
NodeList *nouveau_node_list(Node *node);
int append_node(NodeList *list, Node *node);
NodeList **creer_node_table();

int ajouter_variable(Node *node); // ajoute une variable à la table courante
int ajouter_parametre(Node *node); // ajoute un parametre à la table courante
// Node *chercher_variable(char *nom); // cherche une variable dans la table courante
// Node *chercher_variable_profondeur(char *nom); // cherche une variable dans la table courante et les tables parentes
// Node *chercher_parametre_profondeur(char *nom); // cherche un parametre dans la table courante et les tables parentes 
Node *chercher_symbole(char *nom); // cherche un symbole dans la table courante

void affiche_node(Node *node);
void afficher_node2(char *header, Node *node);
void affiche_node_list(NodeList *nodeList);
void afficher_node_table(NodeList **nodeList);
void afficher_instructions(NodeList *list);

int evaluer(int operateur, Node *gauche, Node *droite, int *resultat, int *evaluable);

void push_table();
void pop_table();


// A ENLEVER
int hash(char *nom);
NodePile *get_pile();

#endif 
