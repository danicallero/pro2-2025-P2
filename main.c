/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: 3.3
 * DATE: 26 / 03 / 25
 */

#include <stdio.h>
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
char *consoleBrand2String(tConsoleBrand brand) {
    switch (brand) {
        case nintendo: return "nintendo";
        case sega: return "sega";
        default: return "unknown";
    }
}

/*  Especificación:
*   Objetivo: transforma las mayúsculas de una cadena de caracteres a minúsculas.
*   Entrada:
*    - str: puntero al string que se quiere transoformar.
*   PreCD: str debe ser un puntero válido que apunte a un string existente
*   PosCD: str ahora no tiene mayúsculas.
*   Nota: esta función modifica src directamente, no devuelve un duplicado
*/
void toLower(char *str) {
    for (; *str; str++) {
        if (*str >= 'A' && *str <= 'Z') { //Solo converimos de la A a la Z, minúsculas y otros chars quedan tal cual
            *str += 'a' - 'A'; //convertimos con ASCII
        }
    }
}

/*  Especificación:
*   Objetivo: transforma una cadena de caracteres a un valor enum.
*   Entrada:
*    - source: string
*    - dest: puntero de la variable donde vamos a guardar el enum
*   Salida: true si la marca existe en el enum tconsolebrand, false si no.
*   Salida por referencia: dest tomará el valor del enum correspondiente en caso de existir.
*   PosCD: str se pasará a minúsculas al llamar a la función.
*/
bool string2ConsoleBrand(char *source, tConsoleBrand *dest) {
    bool out = false;
    toLower(source);
    if (strcmp(source, "nintendo") == 0) {
        *dest = nintendo;
        out = true;
    } else if (strcmp(source, "sega") == 0) {
        *dest = sega;
        out = true;
    }
    if (!out) printf("+ Error: Console brand \"%s\" is not among expected brands.\n", source);
    return out; //error flag
}

/*Especificación:
 * Objetivo: transformar de forma segura un string a float.
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

/*Especificación
 * Objetivo: vaciar los contenidos de un stack
 * Entrada: Stack
 * PreCD: El stack debe estar inicializado
 * PosCD: El stack queda vaciado
 */
void clearStack(tStack *stack) {
    while (!isEmptyStack(*stack)) {
        pop(stack);
    }
}

/*Especificación
 * Objetivo: gestionar la eliminación de consolas
 * Entradas:
 *  -pos: posición del elemento a eliminar
 *  -list: lista en la que eliminaremos el item
 * PreCD: la lista debe estar inicializada, la posición del elemento a eliminar es válida
 * PosCD: los elementos de la lista pueden haber cambiado de orden
 */
void handleDeleteConsole(tPosL pos, tList *list) {
    if (pos == LNULL) { //pos tiene que ser válida
        printf("+ Error: Delete not possible.\n");
        return;
    }

    clearStack(&pos->data.bidStack);

    deleteAtPosition(pos, list);
}

/*Especificación
 * Objetivo: Copiar de manera un string a un destino, asegurando que no haya overflow o errores
 * Entradas:
 *  - dest: string de destino
 *  - src: string de origen
 *  - size: tamaño máximo, incluyendo null termination del string de destino
 *  - label: identificador para los errores
 * Salida: true en caso de q
 */
bool safeStrCpy(char *dest, const char *src, size_t size, char *label) {
    bool out = false;
    if (dest == NULL || src == NULL || size == 0) {
        printf("+ Error: missing parameters for: %s\n", label);
    } else if (strlen(src) >= size) {
        printf("+ Error: Invalid string size for parameter: %s\n", label);
    } else {
        strncpy(dest, src, size - 1);
        dest[size - 1] = '\0';
        out = true;
    }
    return out;
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
    tConsoleBrand brand; //variable donde guardamos la conversión de string a enum, lo usamos para añadir verificación
    float param4Float = safeStr2float(param4); //variable precio como float para poder pasarlo al TAD.

    printf("********************\n");
    printf("%s N: console %s seller %s brand %s price %.2f\n", commandNumber, param1, param2, param3, param4Float);

    if (param4Float < 0) { // error en conversión (o han metido un precio negativo)
        printf("+ Error: Invalid price value\n");
        return;
    }

    if (!isEmptyList(*list)) { //totalmente innecesario, pero es lo que me piden para aprobar...
        pos = findItem(param1, *list); //mi finditem funciona con lista vacía (devuelve LNULL en O(1)), pero añadimos redundancia para que no nos bajen nota
        if (pos != LNULL) { //control de duplicados
            printf("+ Error: New not possible\n"); //item already exists
            return;
        }
    }

    if (!safeStrCpy(newItem.consoleId, param1, NAME_LENGTH_LIMIT, "ConsoleId") ||
        !safeStrCpy(newItem.seller, param2, NAME_LENGTH_LIMIT, "SellerId")) {
        printf("+ Error: New not possible\n"); //string assign failed
        return;
    }

    if (!string2ConsoleBrand(param3, &brand)) {
        printf("+ Error: New not possible\n"); //invalid brand
        return;
    }

    newItem.consoleBrand = brand;
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

    if (isEmptyList(*list)) { //totalmente innecesario, porque finditem devuelve LNULL con lista vacía, y lo hace en O(1) pero la redundancia es necesaria para adherirnos a los criterios de evaluación
        printf("+ Error: Delete not possible\n");
        return;
    }

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
    tItemS stackItem; //metadatos de la puja
    float highestBid; //valor numérico de la puja más alta (si no hay pujas será el precio base)
    char *highestBidStr; //string del mayor pujador (si no hay pujas será el vendedor)
    float bidPrice = safeStr2float(param3); //variable precio como float para poder pasarlo al TAD.

    printf("********************\n");
    printf("%s B: console %s bidder %s price %.2f\n", commandNumber, param1, param2, bidPrice);

    if (bidPrice < 0 || isEmptyList(*list)) { //error en conversión (o han metido un precio negativo). Salimos ya porque con esto no podemos trabajar
        printf("+ Error: Bid not possible\n");
        return;
    }

    pos = findItem(param1, *list);
    if (pos == LNULL) { //no se ha encontrado item
        printf("+ Error: Bid not possible\n");
        return;
    }

    item = getItem(pos, *list);
    stackItem = isEmptyStack(item.bidStack)?(tItemS){.consolePrice = -1, .bidder = ""}:peek(item.bidStack); //modificado para verificar que el pujador actual no vuelve a pujar
    highestBid = (stackItem.consolePrice == -1)?item.consolePrice:stackItem.consolePrice; //Usamos valor -1 para marcar que no hay puja porque ya hemos marcado antes valores negativos como error, así que ponerlo ahora no modifica nada
    highestBidStr = (stackItem.consolePrice == -1)?item.seller:stackItem.bidder;

    if (strcmp(highestBidStr, param2) == 0 || strcmp(item.seller, param2) == 0 ||
        bidPrice <= highestBid) { //el vendedor de un item, o el pujador más alto, no puede pujar en su propio item
        printf("+ Error: Bid not possible\n");
        return;
    }

    if (!safeStrCpy(stackItem.bidder, param2, NAME_LENGTH_LIMIT, "BidderId")) {
        printf("+ Error: Bid not possible\n");
        return; //fallo tamaño de string
    }
    stackItem.consolePrice = bidPrice;

    if (push(stackItem, &item.bidStack)) {
        item.bidCounter++;
        updateItem(item, pos, list);
    } else {
        printf("+ Error: Bid not possible\n");
    }

    printf("* Bid: console %s bidder %s brand %s price %.2f bids %d\n", item.consoleId, stackItem.bidder, consoleBrand2String(item.consoleBrand), stackItem.consolePrice, item.bidCounter);
}

/*Especificación:
 *  Objetivo: Procesa el comando 'A' para adjudicar una consola al mayor postor
 *  Entradas:
 *   - commandNumber: número de comando
 *   - param1: consoleId de la consola a pujar
 *   - list: lista donde se encuentra la consola
 *   PostCD: se imprime el usuario al que se le adjudica la consola y, posteriormente, se elimina de la lista. En caso de no existir elemento o pujas se imprimirá un error.
 */
void processAwardCommand(char *commandNumber, char *param1, tList *list ) {
    printf("********************\n");
    printf("%s A: console %s\n", commandNumber, param1);

    if (isEmptyList(*list)) { //totalmente innecesario, porque finditem devuelve LNULL con lista vacía, y lo hace en O(1), pero la redundancia es necesaria para adherirnos a los criterios de evaluación
        printf("+ Error: Delete not possible\n");
        return;
    }

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
 * PostCD: Imprime una lista con todas las consolas, junto con su mayor pujador (No bids en caso de no tener ninguna).
 * También imprime el número total de consolas, la suma de precios y el precio promedio por marca. Además, imprimirá la
 * "Top Bid", aquella consola cuyo incremento en precio (original/puja) sea el mayor
 * Nota: Imprime Error si la lista está vacía.
 */
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

/* Especificación:
 * Objetivo: Eliminar de la lista aquellas consolas que no tengan ninguna puja.
 * Entradas:
 *   - commandNumber: número de comando
 *   - list: lista en la que se quiere hacer la purga.
 * PostCD: se elimina de la lista aquellas consolas que no tengan pujas. En caso de no haber consolas, o que ninguna
 * esté libre de pujas, se imprimirá un error; de lo contrario se imprimirá que consolas se eliminan.
 */
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

/* Especificación:
 * Objetivo: Procesa el comando 'I' para invalidar las consolas con pujas irregulares.
 * Entradas:
 *   - commandNumber: número de comando
 *   - list: lista sobre la que se calculan las estadísticas
 *
 * PostCD: imprime el número total de consolas, la suma de precios y el precio promedio por marca.
 * Nota: Imprime Error si la lista está vacía.
 */
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
            clearStack(&item.bidStack);

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