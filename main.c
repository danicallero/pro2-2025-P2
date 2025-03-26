/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: 3.3
 * DATE: 26 / 03 / 25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "console_list.h"

#define MAX_BUFFER 255


/*   Especificación:
*
*   Objetivo: transforma un valor enum a una cadena de caracteres.
*   Entrada: enum (nintendo o sega)
*   Salida: string (nintendo o entrega)
*   Nota: si se le mete cualquier entrada de marca no conocida devolverá "unknown"
*/
const char *consoleBrand2String(tConsoleBrand brand) {
    switch (brand) {
        case nintendo: return "nintendo";
        case sega: return "sega";
        default: return "unknown";
    }
}
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

//todo especificación de fc
void handleDeleteConsole(tPosL pos, tList *list) {
    tItemL *item = &pos->data;

    while (!isEmptyStack(item->bidStack)) {
        pop(&item->bidStack);
    }

    deleteAtPosition(pos, list);
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

/* Especificación:
 * Objetivo: Procesa el comando 'D' para eliminar una consola de la lista.
 * Entradas:
 *   - commandNumber: número de comando
 *   - param1: consoleId a eliminar
 *   - list: lista en la que buscar y eliminar el elemento
 *
 * PostCD: imprime el resultado y modifica la lista si el comando es válido.
 * Imprime Error si el elemento no se encuentra.
 */
void processDeleteCommand(char *commandNumber, char *param1, tList *list) {
    printf("********************\n");
    printf("%s D: console %s\n", commandNumber, param1);

    tPosL pos = findItem(param1, *list);
    if (pos == LNULL) {
        printf("+ Error: Delete not possible\n"); //cannot delete an invalid item
        return;
    }
    tItemL *item = &pos->data; //accedemos a memoria para poder vaciar el stack directamente

    //tenemos que imprimir antes de eliminar porque ahora item se modifica accediendo a su dirección de memoria (no es copia local) para permitir el vaciado del stack de forma directa
    printf("* Delete: console %s seller %s brand %s price %.2f bids %d\n", item->consoleId, item->seller, consoleBrand2String(item->consoleBrand), item->consolePrice, item->bidCounter);

    handleDeleteConsole(pos, list);
}

/* Especificación:
 * Objetivo: Procesa el comando 'B' para realizar una puja sobre una consola existente.
 * Entradas:
 *   - commandNumber: número de comando
 *   - param1: consoleId de la consola a pujar
 *   - param2: nombre del pujador
 *   - param3: precio de la puja (string)
 *   - list: lista donde se actualiza la consola
 *
 * PostCD: imprime el resultado y actualiza el precio y contador de pujas si es válido.
 * Nota: Imprime Error si la consola no existe, el pujador es el vendedor o el precio es menor o igual al actual.
 */
void processBidCommand(char *commandNumber, char *param1, char *param2, char *param3, tList *list) {
    tPosL pos; //posición del item sobre el que pujar.
    tItemL item; //item sobre el que se puja
    tItemS stackItem;

    printf("********************\n");

    float bidPrice = safeStr2float(param3); //variable precio como float para poder pasarlo al TAD.

    printf("%s B: console %s bidder %s price %.2f\n", commandNumber, param1, param2, bidPrice);

    if (bidPrice < 0) { // error
        printf("+ Error: Invalid price value\n");
        return;
    }

    pos = findItem(param1, *list);
    if (pos == LNULL) {
        printf("+ Error: Bid not possible\n"); //no se ha encontrado el item en la lista, no se puede pujar.
        return;
    }

    item = getItem(pos, *list);
    if (strcmp(item.seller, param2) == 0) {
        printf("+ Error: Bid not possible\n"); //vendedor del item cannot bid en su propio objeto (inflar su puja)
        return;
    }

    if (param2 == NULL || strlen(param2) >= NAME_LENGTH_LIMIT) { //verificamos que los strings se ajustan a los tamaños máximos
        printf("+ Error: Invalid bidder ID\n");
        return;
    }

    if (!isEmptyStack(item.bidStack)) { //segunda condición solo se debería comprobar si la primera es cierta
        if (bidPrice <= peek(item.bidStack).consolePrice) {
            printf("+ Error: Bid not possible\n"); //no se puede pujar un valor inferior o igual al actual
            return;
        }
    } else {
        if (bidPrice <= item.consolePrice) {
            printf("+ Error: Bid not possible\n"); //no se puede pujar un valor inferior o igual al actual
            return;
        }
    }

    strncpy(stackItem.bidder, param2, NAME_LENGTH_LIMIT - 1);
    stackItem.bidder[NAME_LENGTH_LIMIT - 1] = '\0'; // aseguramos la terminación como fin de cadena de caracteres
    stackItem.consolePrice = bidPrice;

    if (push(stackItem, &item.bidStack)) {
        item.bidCounter++;
    }

    updateItem(item, pos, list);

    printf("* Bid: console %s bidder %s brand %s price %.2f bids %d\n", item.consoleId, stackItem.bidder, consoleBrand2String(item.consoleBrand), stackItem.consolePrice, item.bidCounter);
}

//todo especificación de fc
void processAwardCommand(char *commandNumber, char *param1, tList *list ) {
    printf("********************\n");
    printf("%s A: console %s\n", commandNumber, param1);
    tPosL pos = findItem(param1, *list);
    tItemS top;
    if (pos == LNULL) {
        printf("+ Error: Award not possible\n"); //cannot delete an invalid item
        return;
    }
    tItemL *item = &pos->data;
    if (isEmptyStack(item->bidStack)) {
        printf("+ Error: Award not possible\n");
    } else {
        top = peek(item->bidStack);
        printf("* Award: console %s bidder %s brand %s price %.2f\n", item->consoleId, top.bidder, consoleBrand2String(item->consoleBrand) ,top.consolePrice);
        handleDeleteConsole(pos, list);
    }
}

/* Especificación:
 * Objetivo: Procesa el comando 'S' para mostrar estadísticas y listing de las consolas registradas.
 * Entradas:
 *   - commandNumber: número de comando
 *   - list: lista sobre la que se calculan las estadísticas
 *
 * PostCD: imprime el número total de consolas, la suma de precios y el precio promedio por marca.
 * Nota: Imprime Error si la lista está vacía.
 */ //todo especificar
void processStatsCommand(char *commandNumber, tList *list) {
    printf("********************\n");
    printf("%s S\n", commandNumber);

    if (isEmptyList(*list)) {
        printf("+ Error: Stats not possible\n"); //list is empty
        return;
    }

    int countNintendo = 0, countSega = 0;       //inicializamos contadores
    float sumNintendo = 0.0f, sumSega = 0.0f;

    tPosL topItemPos = NULL;
    float maxIncrease = 0.0f;
    tPosL pos = first(*list);
    while (pos != LNULL) { //atravesamos toda la lista hasta llegar al final para calcular # de consolas por marca, y hacer el listing de todas en consola.
        tItemL item = getItem(pos, *list);

        printf("Console %s seller %s brand %s price %.2f", item.consoleId, item.seller, consoleBrand2String(item.consoleBrand), item.consolePrice);

        if (isEmptyStack(item.bidStack)) {
            printf(". No bids\n");
        } else {
            float topBid = peek(item.bidStack).consolePrice;
            float increase = ((topBid - item.consolePrice) / item.consolePrice) * 100.0f;

            if (increase > maxIncrease) {
                maxIncrease = increase;
                topItemPos = pos;
            }
            printf(" bids %d top bidder %s\n", item.bidCounter, peek(item.bidStack).bidder);
        }


        if (item.consoleBrand == nintendo) { //aumentamos contador y sumamos contador para contador y promedio
            countNintendo++;
            sumNintendo += item.consolePrice;
        } else if (item.consoleBrand == sega) {
            countSega++;
            sumSega += item.consolePrice;
        }

        pos = next(pos, *list);
    }

    float avgNintendo = (countNintendo > 0) ? sumNintendo / (float)countNintendo : 0; //cálculo de promedios, si no hay nada marcamos 0.
    float avgSega = (countSega > 0) ? sumSega / (float)countSega : 0;

    printf("Brand     Consoles    Price  Average\n");
    printf("Nintendo  %8d %8.2f %8.2f\n", countNintendo, sumNintendo, avgNintendo);
    printf("Sega      %8d %8.2f %8.2f\n", countSega, sumSega, avgSega);

    if (topItemPos != NULL) {
        tItemL topItem = getItem(topItemPos, *list);
        printf("Top bid: console %s seller %s brand %s price %.2f bidder %s top price %.2f increase %.2f%%\n", topItem.consoleId, topItem.seller, consoleBrand2String(topItem.consoleBrand), topItem.consolePrice, peek(topItem.bidStack).bidder, peek(topItem.bidStack).consolePrice, maxIncrease);
    } else {
        printf("Top bid not possible\n");
    }
}

//todo especificar
void processRemoveCommand(char *commandNumber, tList *list) {
    bool removed = false;

    printf("********************\n");
    printf("%s R\n", commandNumber);

    if (isEmptyList(*list)) {
        printf("+ Error: Remove not possible\n"); //list is empty
        return;
    }

    tPosL pos = first(*list);
    while (pos != LNULL) { //atravesamos toda la lista hasta llegar al final para calcular # de consolas por marca, y hacer el listing de todas en consola.
        tItemL item = getItem(pos, *list);
        tPosL nextPos = next(pos, *list);

        if (isEmptyStack(item.bidStack)) {
            removed = true;
            printf("Removing console %s seller %s brand %s price %.2f bids %d\n", item.consoleId, item.seller, consoleBrand2String(item.consoleBrand), item.consolePrice, item.bidCounter);
            handleDeleteConsole(pos, list);
        }

        pos = nextPos;//si accedemos tenemos bad access
    }

    if (!removed) {
        printf("* Error: Remove not possible\n");
    }
}

//todo especificar
void processInvalidateBidsCommand(char *commandNumber, tList *list) {
    printf("********************\n");
    printf("%s I\n", commandNumber);

    if (isEmptyList(*list)) {
        printf("+ Error: InvalidateBids not possible\n");
        return;
    }

    int totalBids = 0, numConsoles = 0;
    tPosL pos = first(*list);

    //atravesamos para #pujas #consolas
    while (pos != LNULL) {
        tItemL item = getItem(pos, *list);
        totalBids += item.bidCounter;
        numConsoles++;
        pos = next(pos, *list);
    }

    float averageBids = (float)totalBids / (float)numConsoles;
    float range = 2 * averageBids;
    bool invalidated = false;

    pos = first(*list);

    //atravesamos la lista y eliminamos las pujas si se supera el rango
    while (pos != LNULL) {
        tItemL item = getItem(pos, *list);

        if ((float)item.bidCounter > range) {
            while (!isEmptyStack(item.bidStack)) {
                pop(&item.bidStack);
            }

            printf("* InvalidateBids: console %s seller %s brand %s price %.2f bids %d average bids %.2f\n", item.consoleId, item.seller, consoleBrand2String(item.consoleBrand), item.consolePrice, item.bidCounter, averageBids);
            item.bidCounter = 0;
            updateItem(item,pos,list);
            invalidated = true;
        }

        pos = next(pos, *list);
    }

    if (!invalidated) {
        printf("+ Error: InvalidateBids not possible\n"); // Ninguna consola fue invalida
    }
}


void processCommand(char *commandNumber, char command, char *param1, char *param2, char *param3, char *param4, tList *list) {

    switch (command) {
        case 'N':
            processNewCommand(commandNumber, param1, param2, param3, param4, list);
            break;
        case 'D':
            processDeleteCommand(commandNumber, param1, list);
            break;
        case 'B':
            processBidCommand(commandNumber, param1, param2, param3, list);
            break;
        case 'A':
            processAwardCommand(commandNumber, param1, list);
            break;
        case 'R':
            processRemoveCommand(commandNumber,list);
            break;
        case 'S':
            processStatsCommand(commandNumber, list);
            break;
        case 'I':
            processInvalidateBidsCommand(commandNumber, list);
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