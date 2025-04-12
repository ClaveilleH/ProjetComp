#!/bin/bash

TEST_DIR="./Tests"

# Compile le projet
make

# Vérifie si la compilation a réussi
if [ $? -ne 0 ]; then
    echo "La compilation a échoué. Abandon."
    exit 1
fi


# Exécute comp.out sur chaque fichier du dossier Tests
for test_file in "$TEST_DIR"/*; do
    if [ -f "$test_file" ]; then
        if [[ "$test_file" == *".c" ]]; then
            echo "Exécution de comp.out sur $test_file..."
            ./comp.out "$test_file"
            echo "----------------------------------------"
        fi
    fi
done