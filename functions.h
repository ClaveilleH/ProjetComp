#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#define TAILLE 103

// typedef enum {
//     TYPE_INT,
//     TYPE_VOID
// } type;

// une variable a qu'un type possible
typedef struct _var {
    char *varName;
    struct _var *nextVar;
    
} var;

typedef struct _function {
    char *varName;
    // type funcType;
    char *funcType;
    var *varList[TAILLE]; // liste des variables locales
    struct _function *nextFunc;
} function;

var *new_varlist(void);
var *new_var(void);

function *insert_function(const char *name, char *funcType);
var *insert_var(const char *name);
void reset_tables(void);
int var_exists(const char *name);
int function_exists(const char *name);

void print_var_table();
void print_function_table();
    


char * concat(const char *s1, const char *s2);
char * concat3(const char *s1, const char *s2, const char *s3);

char *get_random_name();

#endif // FUNCTIONS_H