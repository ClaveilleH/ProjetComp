#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#define TAILLE 103

// typedef enum {
//     TYPE_INT,
//     TYPE_VOID
// } type;

// une variable n'a qu'un type possible
typedef struct _variable {
    char *varName;
    struct _variable *nextVar;
    
} variable;

typedef struct _function {
    char *varName;
    // type funcType;
    char *funcType;
    variable **varTable; // liste des variables locales
    struct _function *nextFunc;
} function;

int hash(char *nom);


variable *new_variable(void);
int append_variable(variable *var1, variable *var2);

variable **new_varTable(void);
int add_var(variable *table, variable *var);

void print_var_list(variable *var);
void print_var_table(variable **table);


// function *insert_function(const char *name, char *funcType);
// var *insert_var(const char *name);
// void reset_tables(void);
// int var_exists(const char *name);
// int function_exists(const char *name);

// void print_function_table();
    


char * concat(const char *s1, const char *s2);
char * concat3(const char *s1, const char *s2, const char *s3);

char *get_random_name();

#endif // FUNCTIONS_H