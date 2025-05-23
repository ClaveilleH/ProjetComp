#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "arbre.h"


#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define RESET_COLOR "\033[0m"

//table de symboles pour un bloc (et permet des blocs imbriqués)

NodePile *pile_variables = NULL;    // le sommet de la pile de variables
NodePile *pile_parametres = NULL;   // le sommet de la pile de paramètres 
NodePile *pile_fonctions = NULL;    // le sommet de la pile de fonctions
NodeList **table_fonctions = NULL; // table de fonctions

NodeList *malloc_list = NULL; // liste de mallocs pour le free
int malloc_count = 0; // compteur de mallocs

void free_all() {
    NodeList *temp = malloc_list;
    // while (temp != NULL) {
    //     NodeList *next = temp->suivant;
    //     free(temp->node);
    //     temp->node == NULL;
    //     free(temp);
    //     temp = next;
    // }
    // free(pile_variables);
    // free(pile_parametres);
    // free(pile_fonctions);
    free(table_fonctions);
}

void free_node(Node* node) {
    switch (node->type) {
        case SYMBOLE:
            printf("free symbole\n");
            free(node->symbole.nom);
            
            break;
        case FONCTION:
            printf("free fonction ");
            printf("id: %d ", node->id);
            printf("nom: %s\n", node->fonction.nom);
            free(node->fonction.nom);
            free_list(node->fonction.liste_parametres);
            free_node(node->fonction.bloc);
            break;
        case PARAMETRE:
            printf("free parametre\n");
            free(node->parametre.nom);
            break;



        case BLOC:
            // printf(COLOR_RED "free bloc\n" RESET_COLOR);
            printf("free bloc\n");
            free_table(node->bloc.table_declarations);
            free_list(node->bloc.liste_instructions);
            break;

        default:
            fprintf(stderr, "Erreur: type de noeud non géré dans free_node\n");
            break;
            
    }
    // printf("free node id: %d\n", node->id);
    free(node);
}

char *get_nom(Node *node) {
    switch (node->type) {
        case SYMBOLE:
            return node->symbole.nom;
        case FONCTION:
            return node->fonction.nom;
        case PARAMETRE:
            return node->parametre.nom;
        default:
            return NULL;
    }
}
char *get_type(Node *node) {
    switch (node->type) {
        case SYMBOLE:
            return "SYMBOLE";
        case FONCTION:
            return "FONCTION";
        case PARAMETRE:
            return "PARAMETRE";
        default:
            return NULL;
    }
}

void free_list(NodeList *list) {
    NodeList *temp = list;
    while (temp != NULL) {
        printf(COLOR_RED"free list id: %d\n" RESET_COLOR, temp->id);
        printf("    node: %s\n", get_type(temp->node));
        printf("    node: %s\n", get_nom(temp->node));
        NodeList *next = temp->suivant;
        free_node(temp->node);
        free(temp);
        temp = next;
    }
}


void free_table(NodeList **table) {
    printf(COLOR_RED "free table\n" RESET_COLOR);
    NodeList *list;
    for (int i = 0; i < TAILLE; i++) {
        list = table[i];
        if (list != NULL) {
            free_list(list);
        }
    }
    // printf("free table\n");
    free(table);
}

int hash( char *nom ) {
    int i, r;
    int taille = strlen(nom);
    r = 0;
    for ( i = 0; i < taille; i++ )
        r = ((r << 8) + nom[i]) % TAILLE;
    return r;
}

//ouvrir un bloc en empilant les symboles
void push_table() {
    NodePile *nouvelle_pile = malloc(sizeof(NodePile));
    nouvelle_pile->id = malloc_count;
    // printf("malloc pile id: %d\n", malloc_count++);

    nouvelle_pile->node = NULL;
    nouvelle_pile->suivant = pile_variables;
    pile_variables = nouvelle_pile;

    nouvelle_pile = malloc(sizeof(NodePile));
    nouvelle_pile->id = malloc_count;
    // printf("malloc pile id: %d\n", malloc_count++);
    nouvelle_pile->node = NULL;
    nouvelle_pile->suivant = pile_parametres;
    pile_parametres = nouvelle_pile;
}
//fermer un bloc en dépilant les symboles
void pop_table() {
    if (pile_variables) {
        NodePile *temp = pile_variables;
        pile_variables = pile_variables->suivant;
        NodeList **table = temp->node;
        if (table != NULL) {
            free_table(table);
        }
        free(temp);
    }
    if (pile_parametres) {
        NodePile *temp = pile_parametres;
        pile_parametres = pile_parametres->suivant;
        NodeList **table = temp->node;
        if (table != NULL) {
            free_table(table);
        }
        free(temp);
    }
}


Node *nouveau_node(NodeType type) {
    Node *node = malloc(sizeof(Node));
    node->id = malloc_count;
    // printf("malloc node id: %d\n", malloc_count++);
    assert(node != NULL);
    node->type = type;
    if (type == SYMBOLE) {
        node->symbole.type = ENTIER;
        node->symbole.valeur = 0; // valeur initiale à 0 
        node->symbole.isInitialized = 0; // on met la variable comme non initialisée
        node->symbole.evaluable = 0; // on met la variable comme non évaluable
        node->symbole.dimension = 0; // dimension du tableau
    } else if (type == FONCTION) {
        node->fonction.nom = NULL;
        node->fonction.type = ENTIER;
        // node->fonction.arguments = NULL;
        // node->fonction.suivant = NULL;
    }
    // printf(COLOR_GREEN "9 : append_node %d\n" RESET_COLOR, node->id);
    // append_node(malloc_list, node);
    return node;
}

NodeList *nouveau_node_list(Node *node) {
    NodeList *nodeList = malloc(sizeof(NodeList));
    nodeList->id = malloc_count;
    printf("malloc nodeList id: %d\n", malloc_count++);
    assert(nodeList != NULL);
    nodeList->node = node;
    nodeList->suivant = NULL;
    nodeList->precedent = NULL;
    return nodeList;
}

NodeList **creer_node_table() {
    NodeList **nodeList = malloc(TAILLE * sizeof(NodeList *));
    // printf("malloc nodeList table id: %d\n", malloc_count++);
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
        printf(COLOR_GREEN "8 : Node LIST %d\n" RESET_COLOR, list->id);
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
    if (temp->node->type == FONCTION && node->type == FONCTION) { // si le type est FONCTION
        if (strcmp(temp->node->fonction.nom, node->fonction.nom) == 0) { // Vérifie si le nom est déjà présent
            return 1; // Erreur d'ajout
        }
    }
    temp->suivant = nouveau_node_list(node);
        printf(COLOR_GREEN "9 : Node LIST %d\n" RESET_COLOR, temp->suivant->id);
        return 0; // Ajout réussi
}


Node *construire_expr_binaire(Node *gauche, Node *droite, char *op) {
    Node *node = nouveau_node(EXPRESSION);
    node->expression.type = EXPRESSION_BINAIRE;
    node->expression.gauche = gauche;
    node->expression.droite = droite;
    node->expression.operateur = strdup(op);
    // evaluer(op2, gauche, droite, &node->expression.valeur, &node->expression.evaluable);

    return node;
}

int ajouter_variable(Node *node) {
    // On ajoute le symbole à la pile de variables
    NodeList **courant = pile_variables->node;
    if (courant == NULL) {
        pile_variables->node = creer_node_table();
        // printf("Initialisation de la table de variables %d\n", malloc_count);
        courant = pile_variables->node;
    }
    int h = hash(node->symbole.nom);
    if (courant[h] == NULL) {
        courant[h] = nouveau_node_list(node);
        printf(COLOR_GREEN "10 : Node LIST %d\n" RESET_COLOR, courant[h]->id);
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
    printf(COLOR_GREEN "11 : Node LIST %d\n" RESET_COLOR, temp->suivant->id);
    
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

Node *chercher_variable_profondeur_rec(char *nom, NodePile *pile) {
    if (pile == NULL) {
        return NULL; // Pas de variables dans la pile
    }
    NodeList **courant = pile->node;
    if (courant == NULL) {
        return chercher_variable_profondeur_rec(nom, pile->suivant); // Pas de variables dans la pile courante
    }
    int h = hash(nom);
    NodeList *temp = courant[h];
    while (temp != NULL) {
        if (strcmp(temp->node->symbole.nom, nom) == 0) {
            return temp->node; // Variable trouvée
        }
        temp = temp->suivant;
    }
    return chercher_variable_profondeur_rec(nom, pile->suivant); // Cherche dans la pile suivante
}

Node *chercher_variable_profondeur(char *nom) {
    return chercher_variable_profondeur_rec(nom, pile_variables);
}



int ajouter_parametre(Node *node) {
    // On ajoute le symbole à la pile de paramètres
    NodeList **courant = pile_parametres->node;
    if (courant == NULL) {
        pile_parametres->node = creer_node_table();
        // printf("Initialisation de la table de paramètres %d\n", malloc_count);
        courant = pile_parametres->node;
    }
    int h = hash(node->parametre.nom);
    if (courant[h] == NULL) {
        courant[h] = nouveau_node_list(node);
        printf(COLOR_GREEN "13 : Node LIST %d\n" RESET_COLOR, courant[h]->id);
        return 0; // Ajout réussi
    }
    printf(COLOR_RED "7 : append_node %d\n" RESET_COLOR, courant[h]->id);
    append_node(courant[h], node);
    return 0; // Ajout réussi
}

Node *chercher_parametre_profondeur_rec(char *nom, NodePile *pile) {
    if (pile == NULL) {
        return NULL; // Pas de paramètres dans la pile
    }
    NodeList **courant = pile->node;
    if (courant == NULL) {
        return chercher_parametre_profondeur_rec(nom, pile->suivant); // Pas de paramètres dans la pile courante
    }
    int h = hash(nom);
    NodeList *temp = courant[h];
    while (temp != NULL) {
        if (strcmp(temp->node->parametre.nom, nom) == 0) {
            return temp->node; // Paramètre trouvé
        }
        temp = temp->suivant;
    }
    return chercher_parametre_profondeur_rec(nom, pile->suivant); // Cherche dans la pile suivante
}
Node *chercher_parametre_profondeur(char *nom) {
    return chercher_parametre_profondeur_rec(nom, pile_parametres);
}

int ajouter_fonction(Node *node) {
    // On ajoute le symbole à la table de fonctions
    NodeList **courant = table_fonctions;
    if (courant == NULL) {
        table_fonctions = creer_node_table();
        // printf("Initialisation de la table de fonctions %d\n", malloc_count);
        courant = table_fonctions;
    }
    int h = hash(node->fonction.nom);
    if (courant[h] == NULL) {
        courant[h] = nouveau_node_list(node);
        printf(COLOR_GREEN "13 : Node LIST %d\n" RESET_COLOR, courant[h]->id);
        return 0; // Ajout réussi
    }
    NodeList *temp = courant[h];
    printf(COLOR_RED "8 : append_node %d\n" RESET_COLOR, temp->id);
    return append_node(temp, node); // retourne 0 si l'ajout a réussi, 1 sinon
}


Node *chercher_fonction(char *nom) {
    int h = hash(nom);
    NodeList **courant = table_fonctions;
    if (courant == NULL) {
        return NULL; // Pas de fonctions dans la table
    }
    NodeList *temp = courant[h];
    while (temp != NULL) {
        if (strcmp(temp->node->fonction.nom, nom) == 0) {
            return temp->node; // Fonction trouvée
        }
        temp = temp->suivant;
    }
    return NULL; // Fonction non trouvée
}


Node *chercher_symbole(char *nom) {
    Node *var = chercher_variable_profondeur(nom);
    if (var != NULL) {
        return var;
    }
    Node *param = chercher_parametre_profondeur(nom);
    if (param != NULL) {
        return param;
    }
    return NULL; // Symbole non trouvé
}

// try
//On teste si une variable est initialisée avant d'être utilisée
int verifier_initialisation_expression(Node *expr) {
    if (expr == NULL) return 0;

    switch (expr->type) {
        case SYMBOLE:
            return (expr->symbole.isInitialized == 0);

        case EXPRESSION:
            switch (expr->expression.type) {
                case EXPRESSION_CONSTANTE:
                    return 0;

                case EXPRESSION_PARENTHESE:
                case EXPRESSION_MOINS_UNAIRE:
                    return verifier_initialisation_expression(expr->expression.expression);

                case EXPRESSION_BINAIRE:
                    return 
                        verifier_initialisation_expression(expr->expression.gauche) ||
                        verifier_initialisation_expression(expr->expression.droite);

                default:
                    return 0;
            }
        
        case APPEL_FONCTION: { // On vérifie les paramètres de l'appel de fonction
            NodeList *args = expr->appel_fonction.liste_expressions;
            while (args) {
                if (verifier_initialisation_expression(args->node)) return 1;
                args = args->suivant;
            }
            return 0;
        }
        
        case ACCES_TABLEAU: { // Il faudra aussi dans un second temps vérifier lors de l'appel d'un tableau si sa dimension et ses indices sont valides
            if (expr->acces_tableau.variable && expr->acces_tableau.variable->symbole.isInitialized == 0) {
                return 1;
            }

            NodeList *indices = expr->acces_tableau.liste_expressions;
            while (indices) {
                if (verifier_initialisation_expression(indices->node)) return 1;
                indices = indices->suivant;
            }
            return 0;
        }

        case CONDITION_BINAIRE:
            return verifier_initialisation_expression(expr->condition_binaire.gauche) ||
                   verifier_initialisation_expression(expr->condition_binaire.droite);

        default:
            return 0;
    }
}




NodePile *get_pile() {
    return pile_variables;
}

void afficher_instructions2(char *header, NodeList *list) {
    if (list == NULL) {
        printf("Aucune instruction\n");
        return;
    }
    while (list != NULL) {
        afficher_node2(header, list->node);
        list = list->suivant;
    }
}

int evaluer_expression(Node *node, int *resultat, int *evaluable) {
    if (node->type != EXPRESSION) {
        if (node->type == SYMBOLE) {
            if (node->symbole.evaluable) {
                *evaluable = 1;
                *resultat = node->symbole.valeur;
                return 0; // Évaluation réussie
            } else {
                *evaluable = 0;
                return 1; // Erreur d'évaluation
            }
        }
        fprintf(stderr, "Erreur : le noeud n'est pas une expression\n");
        exit(1); //! TODO: gérer l'erreur
        /* Si il n'y a pas de probleme de grammaire normalement elle arrive jamais*/
    }
    if (node->expression.type == EXPRESSION_CONSTANTE) {
        node->expression.evaluable = 1;
        *evaluable = 1;
        *resultat = node->expression.valeur;
        return 0;
    }
    if (node->expression.type == EXPRESSION_MOINS_UNAIRE) {
        int _valeur;
        int _evaluable;
        evaluer_expression(node->expression.expression, &_valeur, &_evaluable);
        if (_evaluable) {
            *resultat = -_valeur;
            node->expression.evaluable = 1;
            *evaluable = 1;
            return 0; // Évaluation réussie
        }
        return 1; // Erreur d'évaluation
    }
    if (node->expression.type == EXPRESSION_PARENTHESE) {
        int _valeur;
        int _evaluable;
        evaluer_expression(node->expression.expression, &_valeur, &_evaluable);
        if (_evaluable) {
            *resultat = _valeur;
            node->expression.evaluable = 1;
            node->expression.valeur = _valeur;
            *evaluable = 1;
            return 0; // Évaluation réussie
        }
        node->expression.evaluable = 0;
        return 1; // Erreur d'évaluation
    }
    if (node->expression.type == EXPRESSION_BINAIRE) {
        int _valeur_gauche;
        int _valeur_droite;
        int _evaluable_gauche;
        int _evaluable_droite;
        evaluer_expression(node->expression.gauche, &_valeur_gauche, &_evaluable_gauche);
        evaluer_expression(node->expression.droite, &_valeur_droite, &_evaluable_droite);
        if (_evaluable_gauche && _evaluable_droite) {
            switch (node->expression.operateur[0]) {
                case '+':
                    *resultat = _valeur_gauche + _valeur_droite;
                    break;
                case '-':
                    *resultat = _valeur_gauche - _valeur_droite;
                    break;
                case '*':
                    *resultat = _valeur_gauche * _valeur_droite;
                    break;
                case '/':
                    *resultat = _valeur_gauche / _valeur_droite;
                    break;
                case '&':
                    *resultat = _valeur_gauche & _valeur_droite;
                    break;
                case '|':
                    *resultat = _valeur_gauche | _valeur_droite;
                    break;
                case '<':
                    *resultat = _valeur_gauche << _valeur_droite;
                    break;
                case '>':
                    *resultat = _valeur_gauche >> _valeur_droite;
                    break;
                default:
                    *evaluable = 0;
                    return 1; // Erreur d'évaluation
            }
            printf("resultat: %d\n", *resultat);
            node->expression.valeur = *resultat;
            node->expression.evaluable = 1;
            *evaluable = 1;
            return 0; // Évaluation réussie
        }

        node->expression.evaluable = 0;
        *evaluable = 0;
        return 1; // Erreur d'évaluation
    }
    return 1; // Erreur d'évaluation
}

void afficher_node2(char *header, Node *node) {
    if (node == NULL) {
        printf("%sAucun noeud\n", header ? header : "");
        return;
    }
    if (header == NULL) {
        header = "";
    }

    char *header2;
    size_t len = strlen(header);
    size_t suffixe_len = strlen("│   "); // Compte les caractères, pas les octets réels
    size_t alloc_len = len + suffixe_len + 1;

    header2 = calloc(alloc_len, sizeof(char)); // Utilisez calloc pour initialiser la mémoire
    if (header2 == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée\n");
        return;
    }

    snprintf(header2, alloc_len, "%s│   ", header);
    switch (node->type){
        case SYMBOLE :
            if (node->symbole.type == TABLEAU) {
                printf("%s├── Symbole : %s (tableau) dim:%d\n", header, node->symbole.nom, node->symbole.dimension);
            } else if (node->symbole.evaluable) {
                printf("%s├── Symbole : %s = %d\n", header, node->symbole.nom, node->symbole.valeur);
            } else {
                printf("%s├── Symbole : %s (non évaluable)\n", header, node->symbole.nom);
            }
            // printf("%s├── Symbole : %s\n", header, node->symbole.nom);
            break;
        case FONCTION :
            printf("%s├── Fonction t'es pas censé voir ça : %s\n", header, node->fonction.nom);
            break;
        case PARAMETRE :
            printf("%s├── Parametre : %s\n", header, node->parametre.nom);
            break;
        case IF_NODE :
            printf("%s├── If :\n", header);
            afficher_node2(header2, node->if_node.condition);
            afficher_node2(header2, node->if_node.instruction);
            break;
        case IF_ELSE_NODE :
            printf("%s├── If :\n", header);
            afficher_node2(header2, node->if_else_node.condition);
            afficher_node2(header2, node->if_else_node.instruction);
            printf("%s├── Else :\n", header);
            afficher_node2(header2, node->if_else_node.instruction_else);
            break;
        
        case SWITCH_NODE :
            printf("%s├── Switch :\n", header);
            afficher_node2(header2, node->switch_node.expression);
            afficher_node2(header2, node->switch_node.instruction);
            break;
        case CASE_NODE :
            printf("%s├── Case :\n", header);
            afficher_node2(header2, node->case_node.constante);
            afficher_node2(header2, node->case_node.instruction);
            break;
        case DEFAULT_NODE :
            printf("%s├── Default :\n", header);
            afficher_node2(header2, node->default_node.instruction);
            break;
        case FOR_NODE :
            printf("%s├── For :\n", header);
            afficher_node2(header2, node->for_node.init);
            afficher_node2(header2, node->for_node.condition);
            afficher_node2(header2, node->for_node.incr);
            afficher_node2(header2, node->for_node.instruction);
            break;
        case WHILE_NODE :
            printf("%s├── While :\n", header);
            afficher_node2(header2, node->while_node.condition);
            afficher_node2(header2, node->while_node.instruction);
            break;
        case CONDITION_BINAIRE :
        // printf("aaaaaaa");
            printf("%s├── Condition binaire : %s\n", header, node->condition_binaire.operateur);
            afficher_node2(header2, node->condition_binaire.gauche);
            afficher_node2(header2, node->condition_binaire.droite);
            break;
        case EXPRESSION :
            switch (node->expression.type) {
                case EXPRESSION_BINAIRE :
                    printf("%s├── Expression binaire : %s", header, node->expression.operateur);
                    if (node->expression.evaluable) {
                        printf("  (%d)", node->expression.valeur);
                    } else {
                        printf("  (_)");
                    }
                    printf("\n");
                    afficher_node2(header2, node->expression.gauche);
                    afficher_node2(header2, node->expression.droite);
                    break;
                case EXPRESSION_MOINS_UNAIRE :
                    printf("%s├── Expression moins unaire :\n", header);
                    afficher_node2(header2, node->expression.expression);
                    break;
                case EXPRESSION_PARENTHESE :
                    printf("%s├── Expression entre parenthèses", header);
                    if (node->expression.evaluable) {
                        printf("  (%d)", node->expression.valeur);
                    } else {
                        printf("  (_)");
                    }
                    printf("\n");
                    afficher_node2(header2, node->expression.expression);
                    break;
                case EXPRESSION_CONSTANTE :
                    printf("%s├── Constante : %d\n", header, node->expression.valeur);
                    break;
                default:
                    printf("%sEXPRESSION NON DEFINI", header);
            }
            break;
        case BREAK_NODE :
            printf("%s├── Break\n", header);
            break;
        case RETURN_NODE :
            if (node->return_node.expression != NULL) {
                printf("%s├── Return :\n", header);
                afficher_node2(header2, node->return_node.expression);
            } else {
                printf("%s├── Return\n", header);
            }
            break;
        case AFFECTATION :
            printf("%s├── Affectation : ", header);
            if (node->affectation.variable->type == SYMBOLE) {
                printf("%s\n", node->affectation.variable->symbole.nom);
            } else if (node->affectation.variable->type == ACCES_TABLEAU) {
                printf("%s\n", node->affectation.variable->acces_tableau.variable->symbole.nom);
                afficher_node2(header2, node->affectation.variable);
            } else {
                printf("NON DEFINI\n");
            }
            afficher_node2(header2, node->affectation.expression);
            break;
        case APPEL_FONCTION :
            printf("%s├── Appel de fonction : %s\n", header, node->appel_fonction.nom);
            afficher_instructions2(header2, node->appel_fonction.liste_expressions);
            break;
        case ACCES_TABLEAU :
            printf("%s├── Accès tableau : %s\n", header, node->acces_tableau.variable->symbole.nom);
            afficher_instructions2(header2, node->acces_tableau.liste_expressions);
            break;
        case BLOC :
            char *header3;
            header3 = malloc(strlen(header2) + strlen("│   ") + 1); // Utilisez header2 pour le calcul de la taille
            if (header3 == NULL) {
                fprintf(stderr, "Erreur : allocation mémoire échouée\n");
                free(header2);
                return;
            }
            snprintf(header3, strlen(header2) + strlen("│   ") + 1, "%s│   ", header2);

            printf("%s├── Bloc :\n", header);
            printf("%s├── Déclarations :\n", header2);
            
            NodeList **table_declarations = node->bloc.table_declarations;
            for (int i = 0; i < TAILLE; i++) {
                if (table_declarations[i] != NULL) {
                    NodeList *temp = table_declarations[i];
                    while (temp != NULL) {
                        afficher_node2(header3, temp->node);
                        temp = temp->suivant;
                    }
                }
            }
            printf("%s├── Instructions :\n", header2);
            afficher_instructions2(header2, node->bloc.liste_instructions);
            free(header3);
            break;
        case TEST :
            printf("%s├── Test : %s\n", header, node->test.txt);
            break;
        default:
            printf("%sNON DEFINI", header);
            printf(" : %d\n", node->type);
            break;
    }
    free(header2);
}


/*

int evaluer(int operateur, Node *gauche, Node *droite, int *resultat, int *evaluable) {
    int valeur_gauche = 0;
    int valeur_droite = 0;
    if (gauche->type == SYMBOLE) {
        if (gauche->symbole.evaluable) {
            valeur_gauche = gauche->symbole.valeur;
        } else {
            *evaluable = 0;
            return 1; // Erreur d'évaluation
        }
    } else if (gauche->type == EXPRESSION) {
        if (gauche->expression.evaluable) {
            valeur_gauche = gauche->expression.valeur;
        } else {
            *evaluable = 0;
            return 1; // Erreur d'évaluation
        }
    } else {
        *evaluable = 0;
        return 1; // Erreur d'évaluation
    }
    if (droite->type == SYMBOLE) {
        if (droite->symbole.evaluable) {
            valeur_droite = droite->symbole.valeur;
        } else {
            *evaluable = 0;
            return 1; // Erreur d'évaluation
        }
    } else if (droite->type == EXPRESSION) {
        if (droite->expression.evaluable) {
            valeur_droite = droite->expression.valeur;
        } else {
            *evaluable = 0;
            return 1; // Erreur d'évaluation
        }
    } else {
        *evaluable = 0;
        return 1; // Erreur d'évaluation
    }
    switch (operateur) {
        case '+':
            *resultat = valeur_gauche + valeur_droite;
            break;
        case '-':
            *resultat = valeur_gauche - valeur_droite;
            break;
        case '*':
            *resultat = valeur_gauche * valeur_droite;
            break;
        case '/':
            if (valeur_droite == 0) {
                *evaluable = 0;
                return 1; // Erreur de division par zéro
            }
            *resultat = valeur_gauche / valeur_droite;
            break;
        case '&':
            *resultat = valeur_gauche & valeur_droite;
            break;
        case '|':
            *resultat = valeur_gauche | valeur_droite;
            break;
        case '<':
            printf("aaaaaaaaaaaaaaaaaaaa\n");
            *resultat = valeur_gauche << valeur_droite;
            break;
        case '>':
            *resultat = valeur_gauche >> valeur_droite;
            break;
        default:
            *evaluable = 0;
            return 1; // Erreur d'évaluation
    }
    *evaluable = 1;
    return 0; // Évaluation réussie
}

Node *reduire_expression(Node *node) {
    if (node->type == EXPRESSION) {
        if (node->expression.type == EXPRESSION_BINAIRE) {
            node->expression.gauche = reduire_expression(node->expression.gauche);
            node->expression.droite = reduire_expression(node->expression.droite);
            if (node->expression.gauche->type == EXPRESSION && node->expression.droite->type == EXPRESSION) {
                int resultat;
                int evaluable;
                if (evaluer(node->expression.operateur[0], node->expression.gauche, node->expression.droite, &resultat, &evaluable) == 0) {
                    Node *nouveau = nouveau_node(EXPRESSION);
                    nouveau->expression.type = EXPRESSION_CONSTANTE;
                    nouveau->expression.valeur = resultat;
                    nouveau->expression.evaluable = 1;
                    free(node->expression.gauche);
                    free(node->expression.droite);
                    free(node->expression.operateur);
                    free(node);
                    return nouveau;
                }
                printf("Non evaluable 1\n");
            }
            if (node->expression.gauche->type == EXPRESSION && node->expression.droite->type == SYMBOLE) {
                int resultat;
                int evaluable;
                if (evaluer(node->expression.operateur[0], node->expression.gauche, node->expression.droite, &resultat, &evaluable) == 0) {
                    Node *nouveau = nouveau_node(EXPRESSION);
                    nouveau->expression.type = EXPRESSION_CONSTANTE;
                    nouveau->expression.valeur = resultat;
                    nouveau->expression.evaluable = 1;
                    free(node->expression.gauche);
                    free(node->expression.operateur);
                    free(node);
                    return nouveau;
                }
                printf("Non evaluable 2\n");
            }
            if (node->expression.gauche->type == SYMBOLE && node->expression.droite->type == EXPRESSION) {
                int resultat;
                int evaluable;
                if (evaluer(node->expression.operateur[0], node->expression.gauche, node->expression.droite, &resultat, &evaluable) == 0) {
                    Node *nouveau = nouveau_node(EXPRESSION);
                    nouveau->expression.type = EXPRESSION_CONSTANTE;
                    nouveau->expression.valeur = resultat;
                    nouveau->expression.evaluable = 1;
                    free(node->expression.droite);
                    free(node->expression.operateur);
                    free(node);
                    return nouveau;
                }
                printf("Non evaluable 3\n");
            }
            if (node->expression.gauche->type == SYMBOLE && node->expression.droite->type == SYMBOLE) {
                int resultat;
                int evaluable;
                if (evaluer(node->expression.operateur[0], node->expression.gauche, node->expression.droite, &resultat, &evaluable) == 0) {
                    Node *nouveau = nouveau_node(EXPRESSION);
                    nouveau->expression.type = EXPRESSION_CONSTANTE;
                    nouveau->expression.valeur = resultat;
                    nouveau->expression.evaluable = 1;
                    free(node->expression.operateur);
                    free(node);
                    return nouveau;
                }
                printf("Non evaluable 4\n");
            }
            printf("Non evaluable Type gauche : %d Type droite : %d\n", node->expression.gauche->type , node->expression.droite->type);
            // Si l'évaluation échoue, on retourne l'expression d'origine
            return node;
        
        }
    }
    return node;
}
*/