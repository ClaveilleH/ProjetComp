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

// typedef struct _function {
//     char *name;
//     char *type; // peut être passer en enum pour de l'optimisation
//     // type funcType;
//     char *funcType;
//     variable **varTable; // liste des variables locales
//     struct _function *nextFunc;
// } function;

typedef enum { FUNCTION, VARIABLE, FUNCTION_CALL, CONSTANT, TEST, BLOCK, } NodeType;

typedef struct Node {
    NodeType type;
    union {
        struct { char *name; char *type; variable **varTable; variable *params; struct Node *body; } function;
        struct { char *name; } variable;
        struct { char *name; variable *params; } fctCall;
        struct { char *value; } constant;


        struct { struct _nodeList *nodeList; int len; } block; //passer a un tableau ?

    };
    
} Node;

typedef struct _nodeList {
    Node *node;
    struct _nodeList *next;
} nodeList;

typedef struct _functionList {
    Node *function;
    struct _functionList *next;
} functionList;


int hash(char *nom);


variable *new_variable(void);
int append_variable(variable *var1, variable *var2);

variable **new_varTable(void);
int add_var(variable *table, variable *var);

void print_var_list(variable *var);
void print_var_table(variable **table);

// void final_print(variable **varTable, function *funcList);
Node *new_node(NodeType type);

Node *new_function_node(void);

functionList *new_functionList(void);
functionList **new_functionTable(void);
int append_functionList(functionList *func1, functionList *func2);
int add_function(functionList **funcTable, functionList *funcNode);


nodeList *new_nodeList(void);
int append_nodeList(nodeList *node1, nodeList *node2);
// function *new_function(void);
// int append_function(function *func1, function *func2);

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