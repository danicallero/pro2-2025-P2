#!/bin/bash

# declaración de colores
ROJO=$(tput setaf 1)
VERDE=$(tput setaf 2)
AMARILLO=$(tput setaf 3)
RESET=$(tput sgr0)
SUBR=$(tput smul)

# variables
MAIN_NAME=""
VERBOSE=false

test_files=(test_bid_stack test_console_list)
test_refs=(references/test_bid_stack_ref.txt references/test_console_list_ref.txt)

# funciones
usage() {
    echo "Usage: $0 [-p <main|test>] [-v]" 1>&2
    exit 1
}

# args
while getopts "p:v" opt; do
    case "$opt" in
        p)
            MAIN_NAME=$OPTARG
            [[ "$MAIN_NAME" != "main" && "$MAIN_NAME" != "test" ]] && usage
            ;;
        v)
            VERBOSE=true
            ;;
        *)
            usage
            ;;
    esac
done

# aseguramos que existen las carpetas de salida, si no las creamos
mkdir -p outputs adt_test bin

# si no hay -p... ejecutamos las dos pruebas
if [ -z "$MAIN_NAME" ]; then
    "$0" -p test $([ "$VERBOSE" = true ] && echo "-v")
    echo ""
    "$0" -p main $([ "$VERBOSE" = true ] && echo "-v")
    exit 0
fi

# cabecera
printf "\n"
echo "$==============================$"
echo "    ${SUBR}Running tests for: ${MAIN_NAME}${RESET}"
echo "$==============================$"

# prueba del tad
if [ "$MAIN_NAME" = "test" ]; then
    echo -e "${AMARILLO}Compiling test program for bid_stack${RESET}"
    gcc -Wall -Wextra -I. -I.. bid_stack.c adt_test/test_bid_stack.c -o adt_test/test_bid_stack

    echo -e "${AMARILLO}Compiling test program for console_list${RESET}"
    gcc -Wall -Wextra -I. -I.. console_list.c adt_test/test_console_list.c -o adt_test/test_console_list

    echo -e "\n${SUBR}Checking test program output...${RESET}\n"
    printf "${SUBR}Executable${RESET}                          ${SUBR}Result${RESET}  ${SUBR}Notes${RESET}\n"

    for i in "${!test_files[@]}"; do
        exe="./adt_test/${test_files[$i]}"
        ref="${test_refs[$i]}"
        out="outputs/${test_files[$i]}_out.txt"
        diff_out="bin/${test_files[$i]}_diff.txt"

        if [ ! -x "$exe" ]; then
            printf "%-35s ${ROJO}FAIL${RESET}     Not compiled\n" "${test_files[$i]}"
            continue
        fi

        "$exe" > "$out"

        if [ ! -f "$ref" ]; then
            printf "%-35s ${AMARILLO}NOREF${RESET}   Reference missing\n" "${test_files[$i]}"
            continue
        fi

        diff -w -B -b -y --suppress-common-lines --width=170 "$ref" "$out" > "$diff_out"

        if [ -s "$diff_out" ]; then
            printf "%-35s ${ROJO}FAIL${RESET}     Check diff in $diff_out\n" "${test_files[$i]}"
            if $VERBOSE; then
                printf '\nFile: %-77s |   File: %s\n' "$ref" "$out"
                printf '=%.0s' {1..170}; echo
                cat "$diff_out"; echo
            fi
        else
            printf "%-35s ${VERDE}OK${RESET}\n" "${test_files[$i]}"
        fi
    done

# prueba del main
else
    echo -e "${AMARILLO}Compiling main program${RESET}"
    gcc -Wall -Wextra main.c bid_stack.c console_list.c -o main

    if [ ! -x ./main ]; then
        echo -e "${ROJO}Main compilation failed${RESET}\n"
        exit 1
    fi

    echo -e "\n${SUBR}Checking main program output...${RESET}\n"
    printf "${SUBR}Input file${RESET}                          ${SUBR}Result${RESET}  ${SUBR}Notes${RESET}\n"

    for f in commands/*.txt; do
        base=$(basename "$f" .txt)
        out_file="outputs/${base}_out.txt"
        ref_file="references/${base}_ref.txt"
        diff_out="bin/${base}_diff.txt"

        ./main "$f" > "$out_file"

        if [ ! -f "$ref_file" ]; then
            printf "%-35s ${AMARILLO}NOREF${RESET}   Reference missing\n" "$f"
            continue
        fi

        diff -w -B -b -y --suppress-common-lines --width=170 "$ref_file" "$out_file" > "$diff_out"

        if [ -s "$diff_out" ]; then
            printf "%-35s ${ROJO}FAIL${RESET}     Check diff in $diff_out or run with -v\n" "$f"
            if $VERBOSE; then
                printf '\nFile: %-77s |   File: %s\n' "$ref_file" "$out_file"
                printf '=%.0s' {1..170}; echo
                cat "$diff_out"; echo
            fi
        else
            printf "%-35s ${VERDE}OK${RESET}\n" "$f"
        fi
    done
fi

exit 0
