#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"
#include "genererDot.h"

static int compteur = 0;
static FILE *f_dot = NULL;

void ouvrir_graphe() {
    f_dot = fopen("graphe.dot", "w");
    if (!f_dot) {
        perror("Erreur d'ouverture du fichier graphe.dot");
        return;
    }
    fprintf(f_dot, "digraph G {\n");
}

void fermer_graphe() {
    if (f_dot) {
        fprintf(f_dot, "}\n");
        fclose(f_dot);
        f_dot = NULL;
    }
}

void fleche(int from, int to) {
    fprintf(f_dot, "node%d -> node%d;\n", from, to);
}

int noeudPersonnalisable(const char* label, const char* shape, const char* color, const char* style) {
    int id = compteur++;
    fprintf(f_dot, "node%d [label=\"%s\"", id, label);
    if (shape) fprintf(f_dot, " shape=%s", shape);
    if (color) fprintf(f_dot, " color=%s", color);
    if (style) fprintf(f_dot, " style=%s", style);
    fprintf(f_dot, "];\n");
    return id;
}

int generer_dot_node(Node *node) {
    if (!node) return -1;

    char label[128];
    int id = -1;

    switch (node->type){

        case SYMBOLE :
            // if (node->symbole.evaluable) {

            id = noeudPersonnalisable(node->symbole.nom, "ellipse", "black", NULL);

            
            break;

        case FONCTION :{
			// on teste si la fonction est externe si oui on ne la déclare pas
            if (!(node->fonction.externe)){
            const char* typeFonction = node->fonction.type == ENTIER ? "int" : "void";
            char label[128];
            strcpy(label, node->fonction.nom);
            strcat(label, ", ");
            strcat(label, typeFonction);
            id = noeudPersonnalisable(label, "invtrapezium", "blue", NULL);
            NodeList *paramFonction = node->fonction.liste_parametres;
            while (paramFonction) {
                int id_param = generer_dot_node(paramFonction->node);
                fleche(id, id_param);
                paramFonction = paramFonction->suivant;
            }
            if (node->fonction.bloc) {
                int id_bloc = generer_dot_node(node->fonction.bloc);
                fleche(id, id_bloc);
            }
            }
            break;
        }
        case ACCES_TABLEAU:
            id = noeudPersonnalisable("TAB", "ellipse", "black", NULL);
            int nom = noeudPersonnalisable(node->acces_tableau.variable->symbole.nom, "ellipse", "black", NULL);
            // int nom = generer_dot_node(node->symbole.nom);
            fleche(id, nom);
            // indices du tableau
            NodeList *indice = node->acces_tableau.liste_expressions;
            while (indice != NULL) {
                int id_indice = generer_dot_node(indice->node); // affiche la valeur d’indice 
                fleche(id, id_indice);
                indice = indice->suivant;
            }

            break;

        case PARAMETRE :
            id = noeudPersonnalisable(node->parametre.nom, "ellipse", "blue", NULL);
            break;

        case IF_NODE :
            id = noeudPersonnalisable("IF", "diamond", "black", NULL);
             if (node->if_node.condition) {
                int cond = generer_dot_node(node->if_node.condition);
                fleche(id, cond);
            }
            if (node->if_node.instruction) {
                int instrIF = generer_dot_node(node->if_node.instruction);
                fleche(id, instrIF);
            }
            break;

        case IF_ELSE_NODE :
            id = noeudPersonnalisable("IF", "diamond", "black", NULL);
            if (node->if_else_node.condition) {
                int cond = generer_dot_node(node->if_else_node.condition);
                fleche(id, cond);
            }
            if (node->if_else_node.instruction) {
                int instrIfElse = generer_dot_node(node->if_else_node.instruction);
                fleche(id, instrIfElse);
            }

            int idElse = noeudPersonnalisable("ELSE", "diamond", "black", NULL);
            fleche(id, idElse);
            
            if (node->if_else_node.instruction) {
                int instrElse = generer_dot_node(node->if_else_node.instruction_else);
                fleche(idElse, instrElse);
            }
            break;
        
        case SWITCH_NODE :
            id = noeudPersonnalisable("SWITCH", "ellipse", "black", NULL);
            int expr = generer_dot_node(node->switch_node.expression);
            fleche(id, expr);
            
            NodeList *casesListe = node->switch_node.liste_instructions;
            while (casesListe != NULL) {
                if (casesListe->node) {
                    int id_case = generer_dot_node(casesListe->node);
                    fleche(id, id_case);
                }
                casesListe = casesListe->suivant;
            }
            break;

        case CASE_NODE :

            if (node->case_node.constante->type == SYMBOLE) {
                snprintf(label, sizeof(label), "%s", node->case_node.constante->symbole.nom);
            }
            else {
                snprintf(label, sizeof(label), "CASE %d",node->case_node.constante->expression.valeur);
            }
            id = noeudPersonnalisable(label, "ellipse", "black", NULL);

            NodeList *caseInstructions = node->case_node.liste_instructions;
            while (caseInstructions != NULL) {
                if (caseInstructions->node) {
                    int id_case = generer_dot_node(caseInstructions->node);
                    fleche(id, id_case);
                }
                caseInstructions = caseInstructions->suivant;
            }
            break;

        case DEFAULT_NODE :
            id = noeudPersonnalisable("case default", "ellipse", "black", NULL);
            NodeList *defaultInstructions = node->case_node.liste_instructions;
            while (defaultInstructions != NULL) {
                if (defaultInstructions->node) {
                    int id_case = generer_dot_node(defaultInstructions->node);
                    fleche(id, id_case);
                }
                defaultInstructions = defaultInstructions->suivant;
            }
            break;

        case FOR_NODE :
            id = noeudPersonnalisable("FOR", "ellipse", "black", NULL);

            int initialisation = generer_dot_node(node->for_node.init);
            fleche(id, initialisation);

            int condition = generer_dot_node(node->for_node.condition);
            fleche(id, condition);

            int incrementation = generer_dot_node(node->for_node.incr);
            fleche(id, incrementation);

            int instruc = generer_dot_node(node->for_node.instruction);
            fleche(id, instruc);

            break;

        case WHILE_NODE :
            id = noeudPersonnalisable("WHILE", "ellipse", "black", NULL);

            int conditionWhile = generer_dot_node(node->while_node.condition);
            fleche(id, conditionWhile);

            int instrWhile = generer_dot_node(node->while_node.instruction);
            fleche(id, instrWhile);
           
            break;

        case CONDITION_BINAIRE : {
            const char *op = node->condition_binaire.operateur;
            id = noeudPersonnalisable(op, "ellipse", "black", NULL);

            if (node->condition_binaire.gauche) {
                int g = generer_dot_node(node->condition_binaire.gauche);
                fleche(id, g);
            }
            if (node->condition_binaire.droite) {
                int d = generer_dot_node(node->condition_binaire.droite);
                fleche(id, d);
            }
            break;
        }
        case EXPRESSION :
            switch (node->expression.type) {

                case EXPRESSION_BINAIRE : {
                    const char *op = node->expression.operateur;
                    id = noeudPersonnalisable(op, "ellipse", "black", NULL);

                    if (node->expression.gauche) {
                        int g = generer_dot_node(node->expression.gauche);
                        fleche(id, g);
                    }
                    if (node->expression.droite) {
                        int d = generer_dot_node(node->expression.droite);
                        fleche(id, d);
                    }
                    break;
                }
                case EXPRESSION_MOINS_UNAIRE :
                    id = noeudPersonnalisable("-", "ellipse", "black", NULL);
                    int g = generer_dot_node(node->expression.expression);
                    fleche(id, g);
                    break;

                case EXPRESSION_PARENTHESE :
                    id = generer_dot_node(node->expression.expression);
                    break;

                case EXPRESSION_CONSTANTE :
                    snprintf(label, sizeof(label), "%d", node->expression.valeur);
                    id = noeudPersonnalisable(label, "ellipse", "black", NULL);
                    break;

                default:
                    id = noeudPersonnalisable("NON DEFINI", "ellipse", "gray", "dashed");
            }
            break;

        case BREAK_NODE :
            id = noeudPersonnalisable("BREAK", "rectangle", "black", NULL);
            break;

        case RETURN_NODE :
            id = noeudPersonnalisable("RETURN", "trapezium", "blue", NULL);
            if (node->return_node.expression != NULL) {
                int expr = generer_dot_node(node->return_node.expression);
                fleche(id, expr);
            } 
            break;

        case AFFECTATION :
            id = noeudPersonnalisable(":=", "ellipse", "black", NULL);
            // Partie gauche de l'affectation
            int g = generer_dot_node(node->affectation.variable);
            fleche(id, g);

            // Partie droite de l'affectation
            int d = generer_dot_node(node->affectation.expression);
            fleche(id, d);
            break;

        case APPEL_FONCTION :
            id = noeudPersonnalisable(node->appel_fonction.nom, "septagon", "black", NULL);
            
            // Paramètres de l'appel de fonction
            NodeList *param = node->appel_fonction.liste_expressions;
            while (param) {
                int id_param = generer_dot_node(param->node);
                fleche(id, id_param);
                param = param->suivant;
            }
            break;

        case BLOC :
            id = noeudPersonnalisable("BLOC", "ellipse", "black", NULL);
            
            
            // Paramètres de l'appel de fonction
            NodeList *instrucBloc = node->bloc.liste_instructions;
            while (instrucBloc) {
                int id_instruc = generer_dot_node(instrucBloc->node);
                fleche(id, id_instruc);
                instrucBloc = instrucBloc->suivant;
            }
            
            break;

        case TEST :
            id = noeudPersonnalisable(node->test.txt, "ellipse", "black", NULL);

            break;

        default:
            id = noeudPersonnalisable("NON DEFINI", "ellipse", "gray", "dashed");
            break;
    }
    return id;
}
