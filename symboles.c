#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "symboles.h"



//table des symboles avec une table de hachage 
symbole *tableGlobale[TAILLE];

// symbole *tableCourante = tableGlobale; //table courante pour la recherche de symboles
//on initilise a global pour les premieres variables


//table de symboles pour un bloc (et permet des blocs imbriqués)
table_t *pile_tables = NULL; // le sommet de la pile

NodePile *pile_variables = NULL; // le sommet de la pile de variables




int hash( char *nom ) {
    int i, r;
    int taille = strlen(nom);
    r = 0;
    for ( i = 0; i < taille; i++ )
        r = ((r << 8) + nom[i]) % TAILLE;
    return r;
}

void table_reset() {
    int i;
    for( i = 0; i < TAILLE; i++ )
        tableGlobale[i] = NULL;
}




//gère l'ajout des symboles dans la table de hachage globale
symbole * inserer( char *nom ) { 
    int h;
    symbole *s;
    symbole *precedent;
    h = hash(nom);  //indice h dans le tableau hash[]
    s = tableGlobale[h];   //début de la liste chaînée de symboles
    precedent = NULL;
    while ( s != NULL ) {   //On parcourt la liste chaînée pour vérifier s'il existe un symbole avec le même nom
        if ( strcmp( s->nom, nom ) == 0 ) 
            return s;   //symbole déjà existant
        precedent = s;
        s = s->suivant;
    }
    //le symbole n'existe pas encore
    if ( precedent == NULL ) {  //premier symbole
        tableGlobale[h] = (symbole *) malloc(sizeof(symbole)); //On crée le symbole
        s = tableGlobale[h]; //On le met en tête de la table
    }
    else {
        precedent->suivant = (symbole *) malloc(sizeof(symbole)); //On crée le symbole
        s = precedent->suivant; //On le met en fin de la liste chaînée 
    }
    s->nom = strdup(nom); //copie du nom
    s->suivant = NULL; //son suivant est forcément NULL car en fin de chaîne

    return s;
}


//liste chaînée pour les paramètres d'une fonction
liste_t *creer_liste( param_t p ) {
    liste_t *liste;
    liste = (liste_t *) malloc(sizeof( liste_t ));
    assert( liste != NULL );
    liste->param = p;
    liste->suivant = NULL;
    return liste;
}

liste_t * concatener_listes( liste_t *l1, liste_t *l2 ) {
    liste_t *l = l1;
    if ( l1 == NULL )
        return l2;
    while ( l->suivant != NULL )
        l = l->suivant;
    l->suivant = l2;
    return l1;
}

void afficher_liste( liste_t *liste ) {
    liste_t *l;
    for ( l = liste; l != NULL; l = l->suivant ) {
        if ( l != liste )
            printf(",");
        printf( " %s (%s)", l->param.nom,
            ( l->param.type == ENTIER )? "int" : "string" );
    }
}

//fonction
fonction_t *fonctions[TAILLE]; //structure pour une fonction

void init_fonction_table() {
    for (int i = 0; i < TAILLE; i++)
        fonctions[i] = NULL;
}

int listes_egales(liste_t *l1, liste_t *l2) {
    while (l1 && l2) {
        if (l1->param.type != l2->param.type)
            return 0;
        l1 = l1->suivant;
        l2 = l2->suivant;
    }
    return l1 == NULL && l2 == NULL;
}

//Ajoute une fonction (nom, type, valeur) et on véifie qu'elle n'est pas redéclarée
fonction_t *ajouter_fonction(type_t type, char *nom, liste_t *args) {
    int h = hash(nom);
    fonction_t *f = fonctions[h], *prev = NULL;
    //On cherche dans la table de hachage si la fonction existe déjà
    while (f) { 
        if (strcmp(f->nom, nom) == 0) {
            if (f->type == type && listes_egales(f->arguments, args)) {
                printf("Redéclaration cohérente de la fonction %s\n", nom);
            } else {
                printf("Redéclaration incohérente de la fonction %s\n", nom);
            }
            return NULL;
        }
        prev = f;
        f = f->suivant;
    }

    fonction_t *nouvelle = malloc(sizeof(fonction_t));
    assert(nouvelle);
    nouvelle->type = type;
    nouvelle->nom = strdup(nom);
    nouvelle->arguments = args;
    nouvelle->suivant = NULL;

    if (prev)
        prev->suivant = nouvelle;
    else
        fonctions[h] = nouvelle;

    return nouvelle;
}

void afficher_fonction(fonction_t *fonction) {
    printf("Fonction: %s\n", fonction->nom);
    printf("Type: %s\n", (fonction->type == ENTIER) ? "int" : "void");
    printf("Arguments:");
    if (fonction->arguments)
        afficher_liste(fonction->arguments);
    else
        printf(" aucun");
    printf("\n\n");
}


//ouvrir un bloc en empilant les symboles
void push_table() {
    table_t *nouvelle = malloc(sizeof(table_t));
    nouvelle->symboles = NULL;
    nouvelle->suivant = pile_tables;
    pile_tables = nouvelle;

    NodePile *nouvelle_pile = malloc(sizeof(NodePile));
    nouvelle_pile->node = NULL;
    nouvelle_pile->suivant = pile_variables;
    pile_variables = nouvelle_pile;
}
//fermer un bloc en dépilant les symboles
void pop_table() {
    if (pile_tables) {
        table_t *temp = pile_tables;
        pile_tables = pile_tables->suivant;
        free(temp);
    }

    if (pile_variables) {
        NodePile *temp = pile_variables;
        pile_variables = pile_variables->suivant;
        free(temp);
    }
}


symbole *ajouter_symbole(char *nom, type_t type, int init) {
    //Vérifie si la variable existe déjà dans le bloc courant
    if (pile_tables) {
        symbole *courant = pile_tables->symboles;
        while (courant) {
            if (strcmp(courant->nom, nom) == 0) {
                printf("Erreur : variable '%s' déjà déclarée dans ce bloc.\n", nom);
                return NULL;
            }
            courant = courant->suivant;
        }
    }

    //On ajoute à la table globale uniquement après vérification
    symbole *s = inserer(nom); //table de hachage globale 
    s->type = type;
    s->valeur = 0;
    s->taille = 1;
    s->position = 0;

    // On l’ajoute dans le bloc courant
    s->suivant = pile_tables->symboles;
    pile_tables->symboles = s;

    return s;
}


symbole *chercher_symbole(char *nom) {
    int h = hash(nom);
    symbole *s = tableGlobale[h];
    while (s != NULL) {
        if (strcmp(s->nom, nom) == 0)
            return s;
        s = s->suivant;
    }
    return NULL;
}



Node *nouveau_node(NodeType type) {
    Node *node = malloc(sizeof(Node));
    assert(node != NULL);
    node->type = type;
    if (type == SYMBOLE) {
        node->symbole.nom = NULL;
        node->symbole.type = ENTIER;
        node->symbole.valeur = 0;
        node->symbole.taille = 1;
        node->symbole.position = 0;
        // node->symbole.suivant = NULL;
    } else if (type == FONCTION) {
        node->fonction.nom = NULL;
        node->fonction.type = ENTIER;
        // node->fonction.arguments = NULL;
        // node->fonction.suivant = NULL;
    }
    return node;
}

NodeList *nouveau_node_list(Node *node) {
    NodeList *nodeList = malloc(sizeof(NodeList));
    assert(nodeList != NULL);
    nodeList->node = node;
    nodeList->suivant = NULL;
    return nodeList;
}

NodeList **creer_node_table() {
    NodeList **nodeList = malloc(TAILLE * sizeof(NodeList *));
    assert(nodeList != NULL);
    for (int i = 0; i < TAILLE; i++) {
        nodeList[i] = NULL;
    }
    return nodeList;
}

int append_node(NodeList *list, Node *node) {
    // Ajoute l2 à la fin de l1
    if (list == NULL) {
        list = nouveau_node_list(node);
        return 0; // Ajout réussi
    }
    NodeList *temp = list;
    while (temp->suivant != NULL) {
        if (temp->node->type == SYMBOLE && node->type == SYMBOLE) {// si le type est SYMBOLE
            if (strcmp(temp->node->symbole.nom, node->symbole.nom) == 0) { // Vérifie si le nom est déjà présent
                return 1; // Erreur d'ajout
            }
        }
        temp = temp->suivant;
    }
    if (temp->node->type == SYMBOLE && node->type == SYMBOLE) { // si le type est SYMBOLE
        if (strcmp(temp->node->symbole.nom, node->symbole.nom) == 0) { // Vérifie si le nom est déjà présent
            return 1; // Erreur d'ajout
        }
    }
    if (temp->node->type == PARAMETRE && node->type == PARAMETRE) { // si le type est PARAMETRE
        if (strcmp(temp->node->parametre.nom, node->parametre.nom) == 0) { // Vérifie si le nom est déjà présent
            return 1; // Erreur d'ajout
        }
    }
    temp->suivant = nouveau_node_list(node);
    return 0; // Ajout réussi
}



void affiche_node(Node *node) {
    if (node->type == SYMBOLE) {
        printf("Nom: %s, Type: %s, Valeur: %d, Taille: %d, Position: %d\n",
               node->symbole.nom,
               (node->symbole.type == ENTIER) ? "int" : "void",
               node->symbole.valeur,
               node->symbole.taille,
               node->symbole.position);
    }
}

void affiche_node_list(NodeList *nodeList) {
    NodeList *temp = nodeList;
    while (temp != NULL) {
        affiche_node(temp->node);
        temp = temp->suivant;
    }
}

void afficher_node_table(NodeList **nodeList) {
    if (nodeList == NULL) {
        printf("Table vide\n");
        return;
    }
    for (int i = 0; i < TAILLE; i++) {
        if (nodeList[i] != NULL) {
            printf("[%d]", i);
            NodeList *temp = nodeList[i];
            while (temp != NULL) {
                if (temp->node->type == SYMBOLE) {
                    printf(" -> %s", temp->node->symbole.nom);
                }
                temp = temp->suivant;
            }
            printf("\n");
        }
    }
}

int ajouter_variable(Node *node) {
    // On ajoute le symbole à la pile de variables
    NodeList **courant = pile_variables->node;
    if (courant == NULL) {
        pile_variables->node = creer_node_table();
        courant = pile_variables->node;
    }
    int h = hash(node->symbole.nom);
    if (courant[h] == NULL) {
        courant[h] = nouveau_node_list(node);
        return 0; // Ajout réussi
    }
    
    NodeList *temp = courant[h];
    while (temp->suivant != NULL) {
        if (strcmp(temp->node->symbole.nom, node->symbole.nom) == 0) {
            return 1; // Erreur d'ajout
        }
        temp = temp->suivant;
    }
    if (strcmp(temp->node->symbole.nom, node->symbole.nom) == 0) {
        return 1; // Erreur d'ajout
    }
    temp->suivant = nouveau_node_list(node);
    
    return 0; // Ajout réussi
}

Node *chercher_variable(char *nom) {
    int h = hash(nom);
    NodeList **courant = pile_variables->node;
    if (courant == NULL) {
        return NULL; // Pas de variables dans la pile
    }
    NodeList *temp = courant[h];
    while (temp != NULL) {
        if (strcmp(temp->node->symbole.nom, nom) == 0) {
            return temp->node; // Variable trouvée
        }
        temp = temp->suivant;
    }
    return NULL; // Variable non trouvée
}

NodePile *get_pile() {
    return pile_variables;
}