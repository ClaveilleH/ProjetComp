#ifndef SYMBOLES_H
#define SYMBOLES_H
#include <stdlib.h>

#define TAILLE 103  //nombre premier 


typedef enum { ENTIER, VOID_TYPE } type_t;


typedef struct symbole {
    char *nom;
    int valeur;
    type_t type;
    int taille;
    int position;
    struct symbole *suivant;
} symbole;

symbole *inserer(char *nom);
symbole *chercher(char *nom);
void table_reset();

typedef enum { SYMBOLE, FONCTION, PARAMETRE, } NodeType;
typedef struct NodeList NodeList; // declaration anticipée
typedef struct Node {
    NodeType type;
    union {
        struct { char *nom; int valeur; type_t type; int taille; int position; } symbole;
        struct { char *nom; type_t type; NodeList *liste_parametres; } fonction;
        struct { char *nom; type_t type; } parametre;
        
    };
} Node;

typedef struct NodeList {
    Node *node;
    struct NodeList *suivant;
} NodeList;


Node *nouveau_node(NodeType type);
NodeList *nouveau_node_list(Node *node);
int append_node(NodeList *list, Node *node);
NodeList **creer_node_table();

void affiche_node(Node *node);
void affiche_node_list(NodeList *nodeList);
void afficher_node_table(NodeList **nodeList);

typedef struct _param_t {
    type_t type;
    char *nom;
} param_t;

typedef struct _liste_t {
    param_t param;
    struct _liste_t *suivant;
} liste_t;


liste_t * creer_liste( param_t p );
liste_t * concatener_listes( liste_t *l1, liste_t *l2 );
void afficher_liste( liste_t *liste );

//On gère à présent les fonctions
// Ajouter après liste_t
typedef struct _fonction_t {
    type_t type;
    char *nom;
    liste_t *arguments;
    struct _fonction_t *suivant;
} fonction_t;

fonction_t *ajouter_fonction(type_t type, char *nom, liste_t *args);
void afficher_fonction(fonction_t *fonction);
int listes_egales(liste_t *l1, liste_t *l2);
void init_fonction_table();



typedef struct table {
    symbole *symboles;
    struct table *suivant; // permet de chaîner les tables (pile)
} table_t;

extern table_t *pile_tables; // le sommet de la pile

void push_table();
void pop_table();
symbole *ajouter_symbole(char *nom, type_t type, int init);
symbole *chercher_symbole(char *nom);



// A ENLEVER
int hash(char *nom);

#endif 
