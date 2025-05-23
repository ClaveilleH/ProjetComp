#ifndef ARBRE_H
#define ARBRE_H
#include <stdlib.h>

#define TAILLE 103  //nombre premier 


typedef enum { ENTIER, VOID_TYPE, TABLEAU, } type_t;


typedef enum { 
    SYMBOLE, FONCTION, PARAMETRE,
    IF_NODE, IF_ELSE_NODE,
    SWITCH_NODE, CASE_NODE, DEFAULT_NODE, 
    FOR_NODE, WHILE_NODE,
    CONDITION_BINAIRE, CONDITION_UNAIRE, CONDITION_NOT,
    BREAK_NODE, RETURN_NODE, 
    EXPRESSION,
    AFFECTATION,
    APPEL_FONCTION,
    ACCES_TABLEAU,
    BLOC,
    TEST, 
} NodeType;
typedef struct NodeList NodeList; // declaration anticipée
typedef struct Node Node; // declaration anticipée

typedef enum { EXPRESSION_BINAIRE, EXPRESSION_MOINS_UNAIRE, EXPRESSION_PARENTHESE, EXPRESSION_CONSTANTE, } ExpressionType;
typedef struct Node { //! est-ce qu'il faut pas faire des nodes pour les expressions separément ?
    NodeType type;
    int id; // id du noeud pour le debug
    union {
        struct { char *nom; type_t type; int dimension; int isInitialized; int valeur; int evaluable; } symbole; // changer le nom
        struct { char *nom; type_t type; NodeList *liste_parametres; Node *bloc; int externe; } fonction;
        struct { char *nom; type_t type; } parametre;

        struct { Node *condition; Node *instruction; } if_node;
        struct { Node *condition; Node *instruction; Node *instruction_else; } if_else_node;

        // struct { Node *expression; Node *instruction; } switch_node;
        struct { Node *expression; NodeList *liste_instructions; } switch_node;
        // struct { Node *constante; Node *instruction; } case_node;
        struct { Node *constante; NodeList *liste_instructions ; } case_node;
        // struct { Node *instruction; } default_node;
        struct { NodeList *liste_instructions ; } default_node;

        struct { Node *init; Node *condition; Node *incr; Node *instruction; } for_node;
        struct { Node *condition; Node *instruction; } while_node;

        struct {Node *gauche; Node *droite; char *operateur; } condition_binaire;
        struct {} condition_unaire;
        struct { Node *condition; } condition_not;
        struct { 
            ExpressionType type;
            int valeur; 
            int evaluable;
            char *operateur; // pour les expressions binaires
            Node *gauche; // pour les expressions binaires
            Node *droite; // pour les expressions binaires
            Node *expression; // pour les expressions unaire
        } expression;

        
        struct {} break_node;
        struct { Node *expression; } return_node;

        struct { Node *variable; Node *expression; } affectation;
        
        struct { char *nom; NodeList *liste_expressions; } appel_fonction;

        struct { Node *variable; NodeList *liste_expressions; } acces_tableau;

        struct { NodeList **table_declarations; NodeList *liste_instructions; } bloc;

        struct { char *txt; } test;
    };
} Node;

typedef struct NodeList {
    int id; // id du noeud pour le debug
    Node *node;
    struct NodeList *suivant;
    struct NodeList *precedent; // pour les instructions, l'ordre est important
} NodeList;

typedef struct NodePile {
    int id; // id du noeud pour le debug
    NodeList **node; // tableau de listes chaînées TODO: changer le nom
    struct NodePile *suivant;
} NodePile;

Node *nouveau_node(NodeType type);
NodeList *nouveau_node_list(Node *node);
int append_node(NodeList *list, Node *node);
int append_node_table(NodeList **table, Node *node);
NodeList **creer_node_table();
Node *construire_expr_binaire(Node *gauche, Node *droite, char *op);
// Node *reduire_expression(Node *node);


int ajouter_variable(Node *node); // ajoute une variable à la table courante
int ajouter_parametre(Node *node); // ajoute un parametre à la table courante
int ajouter_fonction(Node *node); // ajoute une fonction à la table courante

Node *chercher_fonction(char *nom); // cherche une fonction dans la table courante
Node *chercher_symbole(char *nom); // cherche un symbole dans la table courante

void afficher_node2(char *header, Node *node);

// int evaluer(int operateur, Node *gauche, Node *droite, int *resultat, int *evaluable);
int evaluer_expression(Node *node, int *resultat, int *evaluable);

void push_table();
void pop_table();
void free_node(Node* node);
void free_list(NodeList *list);
void free_table(NodeList **table);
void free_all();

//try
int verifier_initialisation_expression(Node *expr, char **nom);


// A ENLEVER
int hash(char *nom);
NodePile *get_pile();

#endif  // ARBRE_H