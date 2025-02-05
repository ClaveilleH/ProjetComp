#!/bin/bash
SEARCH_DIR="fichiersProf/Tests"
DONE=0
ALL_OPTION=false
HIDE_OPTION=false
TEST_OPTION=false
DOUBLE=false
AA_OPTION=false

for arg in "$@"; do
    if [ -f "$arg" ]; then
        echo $arg
    else
        if [ "$arg" == "--all" ]; then
            ALL_OPTION=true
            DOUBLE=true
            continue
        fi
        if [ "$arg" == "--hide" ]; then
            HIDE_OPTION=true
            DOUBLE=true
            continue
        fi
        if [ "$arg" == "--aa" ]; then
            AA_OPTION=true
            HIDE_OPTION=true
            DOUBLE=true
            continue
        fi
    fi
done

if [ $DOUBLE = false ]; then
    while getopts "aht" opt; do
        case $opt in
            a)
                ALL_OPTION=true
                ;;
            h)
                HIDE_OPTION=true
                ;;
            t)
                TEST_OPTION=true
                ;;
            *)  
                echo "Usage: $0 [-a] [-h]"
                exit 1
                ;;
        esac
    done
fi
lex main.l && gcc lex.yy.c -o comp.out -ll && DONE=1




# SI on a reussi a compiler
if [ $DONE -eq 1 ]; then
    if [ $ALL_OPTION = true ]; then
        for file in "$SEARCH_DIR"/*; do
            if [ -f "$file" ]; then
                if [[ -f "$file" && "$file" == *.c ]]; then # si fichier .c
                    if [ $TEST_OPTION = true ]; then
                        echo aa
                    else
                        if [ $HIDE_OPTION = false ]; then
                            echo "Processing $file"
                        fi
                        if [ $AA_OPTION = true ]; then
                            # echo "Processing $file"
                            printf "Processing %s" $file
                        fi
                        cat "$file" | ./comp.out
                    fi
                fi
            fi
        done
    else
        if [ $TEST_OPTION = true ]; then
            printf "Processing fichiersProf/exempleminiC.c -->"
            cat fichiersProf/exempleminiC.c | ./comp.out 
            echo
        else
            if [ "$HIDE_OPTION" = false ]; then
                echo "Processing fichiersProf/exempleminiC.c"
            fi
            cat fichiersProf/exempleminiC.c | ./comp.out
        fi
    fi
fi
