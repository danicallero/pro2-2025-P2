/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "console_list.h"

#define MAX_BUFFER 255

/*Especificación:
 * Objetivo, transformar de forma segura un string a float.
 * Entrada: string
 * Salida: el string transformado a float o -1.0f en caso de fallo en la conversión.
 */
float safeStr2float(const char *str) {
    char *endptr; //variable donde guardamos el último puntero en strtof
    float value = strtof(str, &endptr);
    if (*endptr != '\0') {
        printf("Error: Invalid number format: %s\n", str);
        return -1.0f; //definimos un valor negativo para esta aplicación, no hay precios negativos -> uno será que hubo un error.
    }
    return value;
}


/* Especificación:
 * Objetivo: Procesa el comando 'N' para añadir una nueva consola a la lista.
 * Entradas:
 * - commandNumber: número de comando
 * - param1: consoleId
 * - param2: vendedor
 * - param3: marca
 * - param4: precio
 * - list: lista en la que se guardan los valores
 *
 * PostCD: imprime el resultado y modifica la lista si el comando es válido. Imprime Error seguido de una explicación en caso de fallo.
 *
 * Nota: no permite insertar duplicados, valores ID o seller con strings de longitud inválida y errores de precio.
 */
void processNewCommand(char *commandNumber, char *param1, char *param2, char *param3, char *param4, tList *list) {
    tItemL newItem; //item a insertar
    tPosL pos; //posición del nuevo item

    printf("********************\n");

    float param4Float = safeStr2float(param4); //variable precio como float para poder pasarlo al TAD.

    printf("%s N: console %s seller %s brand %s price %.2f\n", commandNumber, param1, param2, param3, param4Float);

    if (param4Float < 0) { // error
        printf("+ Error: Invalid price value\n");
        return;
    }

    pos = findItem(param1, *list);
    if (pos != LNULL) { //control de duplicados
        printf("+ Error: New not possible\n"); //item already exists
        return;
    }

    if (param1 == NULL || strlen(param1) >= NAME_LENGTH_LIMIT) { //verificamos que los strings se ajustan a los tamaños máximos
        printf("+ Error: Invalid console ID\n");
        return;
    }
    if (param2 == NULL || strlen(param2) >= NAME_LENGTH_LIMIT) { //verificamos que los strings se ajustan a los tamaños máximos
        printf("+ Error: Invalid seller ID\n");
        return;
    }

    strncpy(newItem.consoleId, param1, NAME_LENGTH_LIMIT - 1);
    newItem.consoleId[NAME_LENGTH_LIMIT - 1] = '\0'; // aseguramos la terminación como fin de cadena de caracteres just in case

    strncpy(newItem.seller, param2, NAME_LENGTH_LIMIT - 1);
    newItem.seller[NAME_LENGTH_LIMIT - 1] = '\0'; // aseguramos la terminación como fin de cadena de caracteres

    newItem.consoleBrand = (strcmp(param3, "sega") == 0) ? sega : nintendo; //pasamos de string a enum, solo lo hacemos una vez->no se justifica fc auxiliar
    newItem.consolePrice = param4Float;
    newItem.bidCounter = 0;
    createEmptyStack(&newItem.bidStack); //inicializamos el stack vacío

    if (insertItem(newItem, list)) //insertamos
        printf("* New: console %s seller %s brand %s price %.2f\n", newItem.consoleId, newItem.seller, param3, newItem.consolePrice);
    else
        printf("+ Error: New not possible\n"); //TAD falló (returned false)
}

void processCommand(char *commandNumber, char command, char *param1, char *param2, char *param3, char *param4, tList *list) {

    switch (command) {
        case 'N':
            processNewCommand(commandNumber, param1, param2, param3, param4, list);
            break;
        case 'D':
            break;
        case 'B':
            break;
        case 'A':
            break;
        case 'R':
            break;
        case 'S':
            break;
        case 'I':
            break;
        default:
            break;
    }
}


void readTasks(char *filename, tList *list) {

    FILE *f = NULL;
    char *commandNumber, *command, *param1, *param2, *param3, *param4;
    const char delimiters[] = " \n\r";
    char buffer[MAX_BUFFER];

    f = fopen(filename, "r");

    if (f != NULL) {

        while (fgets(buffer, MAX_BUFFER, f)) {
            commandNumber = strtok(buffer, delimiters);
            command = strtok(NULL, delimiters);
            param1 = strtok(NULL, delimiters);
            param2 = strtok(NULL, delimiters);
            param3 = strtok(NULL, delimiters);
            param4 = strtok(NULL, delimiters);

            processCommand(commandNumber, command[0], param1, param2, param3, param4, list);
        }

        fclose(f);

    } else {
        printf("Cannot open file %s.\n", filename);
    }
}


int main(int nargs, char **args) {
    tList list; //variable lista que pasaremos a las fcs
    createEmptyList(&list); //inicializamos la lista

    char *file_name = "new.txt";

    if (nargs > 1) {
        file_name = args[1];
    } else {
        #ifdef INPUT_FILE
        file_name = INPUT_FILE;
        #endif
    }

    readTasks(file_name, &list);

    return 0;
}