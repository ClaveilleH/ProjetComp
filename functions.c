#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compteur = 0;

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
