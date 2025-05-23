Projet Compilation — miniC → DOT (2025)
========================================

Description
-----------
Ce projet implémente un compilateur simplifié en deux passes pour un sous-ensemble du langage C, appelé "miniC".
Le but est de générer une représentation graphique (au format DOT) de l’arbre abstrait (AST) d’un programme écrit en miniC.

Fonctionnement
--------------
Le compilateur fonctionne en deux étapes :
1. Analyse lexicale, syntaxique et sémantique du programme miniC.
2. Génération d’un fichier .dot représentant l’arbre abstrait.

Il comprend :
- Un analyseur lexical (.l)
- Un analyseur syntaxique et sémantique (miniC.y)
- Une table des symboles
- Un générateur DOT

Langage miniC — Caractéristiques
--------------------------------
- Types : void, int, tableaux d'entiers
- Fonctions avec paramètres de type int
- Instructions : if, else, while, for, switch/case/default, break, return
- Affectations et expressions arithmétiques
- Appels de fonctions

Limitations :
- Pas de pointeurs
- Pas de surcharge de fonctions
- Pas d’imbrication de fonctions
- Toute variable doit être déclarée avant utilisation
- Toute variable doit être initialisée avant d’être utilisée (warning)


Utilisation
-----------
Compilation :
    make
    make debug # pour afficher l'arbre abstrait
    make clean # pour nettoyer les fichiers générés

Exécution :
    ./comp.out "fichier"
    ou passer le fichier dans l'entree standard

Sortie :
    graphe.dot (fichier DOT à visualiser avec Graphviz)

Visualisation :
    dot -Tpng graphe.dot -o graphe.png

Exemple
-------
Code miniC :
    int main() {
        int x;
        x = 3 + 4;
        return x;
    }

Fichiers test :
    Dans le dossier Tests/
    Contient les tests fournis et apportés par certains etudiants
    Ainsi que ceux créés pour valider nos modifications et corrections de bugs

Ce code produit un arbre abstrait représentant la structure du programme.

Erreurs gérées
--------------
- Redéfinition de variable ou de fonction
- Fonction non déclarée
- Return invalide selon le type de retour
- Utilisation de variable non déclarée
- Utilisation de variable non initialisée (warning)
- Division par zéro détectable statiquement (warning)

Fichiers importants
-------------------
- miniC.y : analyse syntaxique + sémantique
- main.l : analyse lexicale
- arbre.c/h : gestion de la generation de l’arbre abstrait
- genererDot.c/h : génération du fichier DOT

Auteurs
-------
CLAVEILLE Hugo
CUVILLEZ Fiona
Projet de compilation 2025 — Université de Nice Côte d'Azur