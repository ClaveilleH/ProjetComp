/*
fichier utilisé pour tout tester
*/

int ab, a, b, c; // déclaration de variables globales
//int a; // leve l'erreur de compilation -> variable déjà déclarée
// int ab; // leve l'erreur de compilation -> variable déjà déclarée
int ac;



int foo() {
    int b;
    a = 0; // a est une variable globale
    a = 0; // a est une variable globale
}

void foo2(int a, int b) {
    // main();
    int c, d;
    int e;
    a = 0; // a est une variable globale
}


int main(){
    int a;
    foo2(a, 65);
    a = 0;
    // b = 0; // leve l'erreur de compilation -> variable non déclarée
    // int ab; // leve l'erreur de compilation PAS NORMAL
}

// foo();

// int main() { // leve l'erreur de compilation

// }


//warnings:
// Variable non utilisée
// Paramètre masqué par une variable
// Appel à une fonction déclarée mais jamais définie
// Variable et fonction de même nom
// Variable non initialisée != Variable non déclarée


//pas de return dans une fonction -> erreur ?
// les commentaires etoilés une erreur de compilation PAS NORMAL