#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int compteur = 0;
// var **varTable[TAILLE];
function **functionTable[TAILLE]; //un seul niveau de declaration possible

int hash(char *nom) {
    int h = 0;
    for (int i = 0; nom[i] != '\0'; i++) {
        h += nom[i];
    }
    return h % TAILLE;
}

variable *new_variable(void) {
    variable *newVar = malloc(sizeof(variable));
    if (newVar == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    newVar->varName = NULL;
    newVar->nextVar = NULL;
    return newVar;
}

int append_variable(variable *var1, variable *var2) {
    if (var2 == NULL) {
        return 0;
    }
    if (var1 == NULL) {
        var1 = var2;
        return 0;
    }
    variable *temp = var1;
    while (temp->nextVar != NULL) {
        if (strcmp(temp->varName, var2->varName) == 0) {
            return 1; // La variable existe déjà
        }
        temp = temp->nextVar;
    }
    if (strcmp(temp->varName, var2->varName) == 0) {
        return 1; // La variable existe déjà
    }
    temp->nextVar = var2;
    return 0;
}

variable **new_varTable(void) {
    variable **newTable = malloc(TAILLE * sizeof(variable *));
    if (newTable == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    return newTable;
}

int add_var(variable *table, variable *var) {
    int h = hash(var->varName);
    if (table[h].varName == NULL) {
        printf("Ajout de la variable %s à l'index %d\n", var->varName, h);
        table[h] = *var;
        return 0;
    }
    return append_variable(table[h].nextVar, var);
}

void print_var_list(variable *var) {
    if (var == NULL) {
        printf("Aucune variable dans la liste\n");
        return;
    }
    printf("Liste des variables :\n");
    while (var != NULL) {
        printf("  %s\n", var->varName);
        var = var->nextVar;
    }
}

void print_var_table(variable **table) {
    printf("Table des variables :\n");
    for (int i = 0; i < TAILLE; i++) {
        if (table[i] != NULL) {
            printf("[%d]", i);
            variable *current = table[i];
            while (current != NULL) {
                printf("  %s", current->varName);
                current = current->nextVar;
            }
            printf("\n");
        }
    }
}


/*===================================Partie DOT===================================*/

char * concat(const char *s1, const char *s2) {
	char *result = malloc(strlen(s1) + strlen(s2));
	if (result == NULL) {
		fprintf(stderr, "Erreur d'allocation mémoire\n");
		exit(1);
	}
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

char * concat3(const char *s1, const char *s2, const char *s3) {
	char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3));
	if (result == NULL) {
		fprintf(stderr, "Erreur d'allocation mémoire\n");
		exit(1);
	}
	strcpy(result, s1);
	strcat(result, s2);
	strcat(result, s3);
	return result;
}

char *get_random_name() {
    char *name = malloc( (compteur%10) * sizeof(char) + 1);
    if (name == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    sprintf(name, "%d", compteur);
    printf("name = %s\n", name);
    compteur++;
    return name;
}



/*

void reset_tables() {
    for (int i = 0; i < TAILLE; i++) {
        varTable[i] = NULL;
        functionTable[i] = NULL;
    }
}

var *create_var(const char *name) {
    var *newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    newVar->varName = strdup(name);
    newVar->nextVar = NULL;
    return newVar;
}

function *insert_function(const char *name, char *funcType) {
    int h = hash((char *)name);
    function *newFunc = malloc(sizeof(function));
    if (newFunc == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    newFunc->varName = strdup(name);
    newFunc->funcType = funcType;
    newFunc->nextFunc = functionTable[h];
    functionTable[h] = newFunc;
    return newFunc;
}

var *insert_var(const char *name) {
    int h = hash((char *)name);
    var *newVar = malloc(sizeof(var));
    if (newVar == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(1);
    }
    newVar->varName = strdup(name);
    newVar->nextVar = varTable[h];
    varTable[h] = newVar;
    return newVar;
}

int var_exists(const char *name) {
    int h = hash((char *)name);
    var *current = varTable[h];
    while (current != NULL) {
        if (strcmp(current->varName, name) == 0) {
            return 1; // La variable existe
        }
        current = current->nextVar;
    }
    return 0; // La variable n'existe pas
}

int function_exists(const char *name) {
    int h = hash((char *)name);
    function *current = functionTable[h];
    while (current != NULL) {
        if (strcmp(current->varName, name) == 0) {
            return 1; // La fonction existe
        }
        current = current->nextFunc;
    }
    return 0; // La fonction n'existe pas
}

void print_var_table() {
    printf("Table des variables :\n");
    for (int i = 0; i < TAILLE; i++) {
        if (varTable[i] != NULL) {
            var *current = varTable[i];
            while (current != NULL) {
                printf("  %s\n", current->varName);
                current = current->nextVar;
            }
        }
    }
}

void print_function_table() {
    printf("Table des fonctions :\n");
    for (int i = 0; i < TAILLE; i++) {
        if (functionTable[i] != NULL) {
            function *current = functionTable[i];
            while (current != NULL) {
                printf("  %s (%s)\n", current->varName, current->funcType);
                current = current->nextFunc;
            }
        }
    }
}

*/