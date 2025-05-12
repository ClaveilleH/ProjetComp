#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"
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

int noeudPerso(const char* label, const char* shape, const char* color, const char* style) {
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

    switch (node->type) {
        case SYMBOLE:
            snprintf(label, sizeof(label), "%s", node->symbole.nom);
            id = noeudPerso(label, "ellipse", "black", NULL);
            break;

        case PARAMETRE:
            snprintf(label, sizeof(label), "%s", node->parametre.nom);
            id = noeudPerso(label, "box", "blue", NULL);
            break;



        case EXPRESSION: {
            if (node->expression.gauche && node->expression.droite) {
                // Cas affectation := entre deux expressions
                id = noeudPerso(":=", "circle", "black", NULL);

                // Gauche de l'affectation
                int g = generer_dot_node(node->expression.gauche);
                fleche(id, g);

                // Droite de l'affectation
                int d = generer_dot_node(node->expression.droite);
                fleche(id, d);

        

            } else {
            // Expression seule (constante, variable ou sous-expression)
                snprintf(label, sizeof(label), "%d", node->expression.valeur);
                id = noeudPerso(label, "ellipse", "black", NULL);

            }
            break;
        }



        case IF_NODE:
            id = noeudPerso("IF", "diamond", "black", NULL);

            if (node->if_node.condition) {
                int cond = generer_dot_node(node->if_node.condition);
                fleche(id, cond);
            }
            if (node->if_node.instruction) {
                int instr = generer_dot_node(node->if_node.instruction);
                fleche(id, instr);
            }
            break;

        case RETURN_NODE:
            id = noeudPerso("RETURN", "trapezium", "blue", NULL);
            if (node->expression.expression) {
                int expr = generer_dot_node(node->expression.expression);
                fleche(id, expr);
            }
            break;

        case BREAK_NODE:
            id = noeudPerso("BREAK", "ellipse", "red", NULL);
            break;
        
        

        case CONDITION_BINAIRE: {
            const char *op = node->condition_binaire.operateur;
            id = noeudPerso(op, "ellipse", "black", NULL);

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


    


        case FONCTION:
            {
            const char* typeFonction = node->fonction.type == ENTIER ? "int" : "void";
            char label[128];
            strcpy(label, node->fonction.nom);
            strcat(label, ", ");
            strcat(label, typeFonction);

            int func = noeudPerso(label, "invtrapezium", "blue", NULL);
            id = noeudPerso("BLOC", "ellipse", "black", NULL);
            fleche(func, id);


            // Paramètres
            NodeList *param = node->fonction.liste_parametres;
            while (param) {
                int id_param = generer_dot_node(param->node);
                fleche(id, id_param);
                param = param->suivant;
            }

            // Déclarations locales
            if (node->fonction.table_declarations) {
                for (int i = 0; i < TAILLE; ++i) {
                    NodeList *decl = node->fonction.table_declarations[i];
                    while (decl) {
                        int id_decl = generer_dot_node(decl->node);
                        fleche(id, id_decl);
                        decl = decl->suivant;
                    }
                }
            }

            // Instructions
            NodeList *instr = node->fonction.liste_instructions;
            while (instr) {
                int id_instr = generer_dot_node(instr->node);
                fleche(id, id_instr);
                instr = instr->suivant;
            }
            break;

        default:
            id = noeudPerso("Inconnu", "ellipse", "gray", "dashed");
            break;
        }
    }

    return id;
}
