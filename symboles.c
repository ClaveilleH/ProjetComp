#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "symboles.h"



//table de symboles pour un bloc (et permet des blocs imbriqués)

NodePile *pile_variables = NULL;    // le sommet de la pile de variables
NodePile *pile_parametres = NULL;   // le sommet de la pile de paramètres 
NodePile *pile_fonctions = NULL;    // le sommet de la pile de fonctions
NodeList **table_fonctions = NULL; // table de fonctions


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
    nouvelle_pile->node = NULL;
    nouvelle_pile->suivant = pile_variables;
    pile_variables = nouvelle_pile;

    nouvelle_pile = malloc(sizeof(NodePile));
    nouvelle_pile->node = NULL;
    nouvelle_pile->suivant = pile_parametres;
    pile_parametres = nouvelle_pile;
}
//fermer un bloc en dépilant les symboles
void pop_table() {
    if (pile_variables) {
        NodePile *temp = pile_variables;
        pile_variables = pile_variables->suivant;
        free(temp);
    }
    if (pile_parametres) {
        NodePile *temp = pile_parametres;
        pile_parametres = pile_parametres->suivant;
        free(temp);
    }
}


Node *nouveau_node(NodeType type) {
    Node *node = malloc(sizeof(Node));
    assert(node != NULL);
    node->type = type;
    if (type == SYMBOLE) {
        node->symbole.nom = NULL;
        node->symbole.type = ENTIER;
        node->symbole.valeur = 0;
        node->symbole.dimension = 0;
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
    nodeList->precedent = NULL;
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
    if (temp->node->type == FONCTION && node->type == FONCTION) { // si le type est FONCTION
        if (strcmp(temp->node->fonction.nom, node->fonction.nom) == 0) { // Vérifie si le nom est déjà présent
            return 1; // Erreur d'ajout
        }
    }
    temp->suivant = nouveau_node_list(node);
    return 0; // Ajout réussi
}

Node *construire_expr_binaire(Node *gauche, Node *droite, char *op, char op2) {
    Node *node = nouveau_node(EXPRESSION);
    node->expression.type = EXPRESSION_BINAIRE;
    node->expression.gauche = gauche;
    node->expression.droite = droite;
    node->expression.operateur = strdup(op);
    evaluer(op2, gauche, droite, &node->expression.valeur, &node->expression.evaluable);

    return node;
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
        courant = pile_parametres->node;
    }
    int h = hash(node->parametre.nom);
    if (courant[h] == NULL) {
        courant[h] = nouveau_node_list(node);
        return 0; // Ajout réussi
    }
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
        courant = table_fonctions;
    }
    int h = hash(node->fonction.nom);
    if (courant[h] == NULL) {
        courant[h] = nouveau_node_list(node);
        return 0; // Ajout réussi
    }
    NodeList *temp = courant[h];
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

void afficher_node2(char *header, Node *node) {
    char *header2;
    switch (node->type){
        case SYMBOLE :
            if (node->symbole.evaluable) {
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
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->if_node.condition);
            afficher_node2(header2, node->if_node.instruction);
            free(header2);
            break;
        case IF_ELSE_NODE :
            printf("%s├── If :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->if_else_node.condition);
            afficher_node2(header2, node->if_else_node.instruction);
            printf("%s├── Else :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->if_else_node.instruction_else);
            free(header2);
            break;
        
        case SWITCH_NODE :
            printf("%s├── Switch :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->switch_node.expression);
            afficher_node2(header2, node->switch_node.instruction);
            free(header2);
            break;
        case CASE_NODE :
            printf("%s├── Case :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->case_node.constante);
            afficher_node2(header2, node->case_node.instruction);
            free(header2);
            break;
        case DEFAULT_NODE :
            printf("%s├── Default :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->default_node.instruction);
            free(header2);
            break;
        case FOR_NODE :
            printf("%s├── For :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->for_node.init);
            afficher_node2(header2, node->for_node.condition);
            afficher_node2(header2, node->for_node.incr);
            afficher_node2(header2, node->for_node.instruction);
            free(header2);
            break;
        case WHILE_NODE :
            printf("%s├── While :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->while_node.condition);
            afficher_node2(header2, node->while_node.instruction);
            free(header2);
            break;
        case CONDITION_BINAIRE :
        // printf("aaaaaaa");
            printf("%s├── Condition binaire : %s\n", header, node->condition_binaire.operateur);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->condition_binaire.gauche);
            afficher_node2(header2, node->condition_binaire.droite);
            free(header2);
            break;
        case EXPRESSION :
            switch (node->expression.type) {
                case EXPRESSION_BINAIRE :
                    printf("%s├── Expression binaire : %s", header, node->expression.operateur);
                    if (node->expression.evaluable) {
                        printf("  (%d)", node->expression.valeur);
                    }
                    printf("\n");
                    header2 = malloc(strlen(header) + 10);
                    sprintf(header2, "%s│   ", header);
                    afficher_node2(header2, node->expression.gauche);
                    afficher_node2(header2, node->expression.droite);
                    free(header2);
                    break;
                case EXPRESSION_MOINS_UNAIRE :
                    printf("%s├── Expression moins unaire :\n", header);
                    header2 = malloc(strlen(header) + 10);
                    sprintf(header2, "%s│   ", header);
                    afficher_node2(header2, node->expression.expression);
                    free(header2);
                    break;
                case EXPRESSION_PARENTHESE :
                    printf("%s├── Expression entre parenthèses\n", header);
                    header2 = malloc(strlen(header) + 10);
                    sprintf(header2, "%s│   ", header);
                    afficher_node2(header2, node->expression.expression);
                    free(header2);
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
                header2 = malloc(strlen(header) + 10);
                sprintf(header2, "%s│   ", header);
                afficher_node2(header2, node->return_node.expression);
                free(header2);
            } else {
                printf("%s├── Return\n", header);
            }
            break;
        case AFFECTATION :
            printf("%s├── Affectation : %s\n", header, node->affectation.variable->symbole.nom);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_node2(header2, node->affectation.expression);
            free(header2);
            break;
        case APPEL_FONCTION :
            printf("%s├── Appel de fonction : %s\n", header, node->appel_fonction.nom);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            afficher_instructions2(header2, node->appel_fonction.liste_expressions);
            free(header2);
            break;
        case BLOC :
            char *header3;
            printf("%s├── Bloc :\n", header);
            header2 = malloc(strlen(header) + 10);
            sprintf(header2, "%s│   ", header);
            header3 = malloc(strlen(header) + 10);
            sprintf(header3, "%s│   ", header2);
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
            free(header2);
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
}
