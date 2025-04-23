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


/* Especificación:
 * Objetivo: Transformar un valor enum a una cadena de caracteres.
 * Entrada:
 *  - brand: enum (tConsoleBrand)
 * Salida:
 *  - string (nintendo o sega)
 * Nota: si se le mete cualquier entrada de marca no conocida devolverá "unknown"
 */
char *consoleBrand2String(tConsoleBrand brand) {
    switch (brand) {
        case nintendo: return "nintendo";
        case sega: return "sega";
        default: return "unknown";
    }
}

/* Especificación:
 * Objetivo: Transformar las mayúsculas de una cadena de caracteres a minúsculas.
 * Entrada:
 *  - str: Puntero al string que se quiere transformar.
 * PreCD:
 *  - 'str' debe ser un puntero válido que apunte a un string existente.
 * PosCD:
 *  - 'str' ahora no tiene mayúsculas.
 * WARNING: Esta función modifica 'src' directamente, no devuelve un duplicado.
 */
void toLower(char *str) {
    for (; *str; str++) {
        if (*str >= 'A' && *str <= 'Z') { /* Solo se convierten caracteres de la 'A' a la 'Z', minúsculas y otros
                                           *   caracteres quedan tal cual.
                                           */
            *str += 'a' - 'A'; //Se convierten los caracteres usando su ASCII (gracias C por hacerlo tu solito).
        }
    }
}

/* Especificación:
 * Objetivo: Transformar una cadena de caracteres a un valor enum.
 * Entrada:
 *  - source: String.
 *  - dest: Puntero de la variable donde vamos a guardar el enum.
 * Salida:
 *  - La función devuelve true si la marca existe entre los datos del enum tConsoleBrand (types.h), false si no.
 * PosCD:
 *  - 'str' se pasará a minúsculas al llamar a la función.
 *  - 'dest' tomará el valor del enum correspondiente en caso de existir.
 */
bool string2ConsoleBrand(char *source, tConsoleBrand *dest) {
    bool out = false; //Auxiliar donde se recoge el output que devolverá return.

    toLower(source);  //Se pasa a minúsculas el source para simplificar la comprobación.
    if (strcmp(source, "nintendo") == 0) {
        *dest = nintendo;
        out = true;
    } else if (strcmp(source, "sega") == 0) {
        *dest = sega;
        out = true;
    }
    if (!out) printf("+ Error: Console brand \"%s\" is not among expected brands.\n", source);
    return out;
}

/* Especificación:
 * Objetivo: Transformar de forma segura un string a float.
 * Entrada:
 *  - str: String.
 * Salida:
 *  - El string transformado a float o -1.0f en caso de fallo en la conversión.
 */
float safeStr2float(const char *str) {
    char *endptr; //Variable donde 'strtof' guarda el último puntero del string.
    const float value = strtof(str, &endptr); //Valor convertido a float.

    if (*endptr != '\0') {
        printf("Error: Invalid number format: %s\n", str);
        return -1.0f; //Se define un valor negativo para esta aplicación, los precios negativos no existen.
    }
    return value;
}

/* Especificación
 * Objetivo: Vaciar los contenidos de un stack.
 * Entrada:
 *  - stack: El stack que se desea vaciar (tStack).
 * PreCD:
 *  - El stack debe estar inicializado.
 * PosCD:
 *  - El stack queda vaciado.
 */
void clearStack(tStack *stack) {
    while (!isEmptyStack(*stack)) { //Se atraviesa el stack mientras no esté vacío.
        pop(stack);
    }
}

/* Especificación
 * Objetivo: Gestionar de forma unificada la eliminación de consolas y el vaciado de sus stacks de pujas.
 * Entradas:
 *  - pos: Posición del elemento a eliminar (tPosL).
 *  - list: Lista en la que se encuentra el elemento que se quiere eliminar (tList).
 * PreCD:
 *  - La lista debe estar inicializada.
 *  - La posición debe ser una posición correcta que contenga un elemento de la lista.
 *  - El item debe ser el elemento contenido en la posición 'pos'.
 * PosCD:
 *  - Los elementos de la lista pueden haber cambiado de orden.
 */
void handleDeleteConsole(tPosL pos, tItemL *item, tList *list) {
    clearStack(&item->bidStack);
    updateItem(*item, pos, list); //Actualizamos el item con la pila borrada.
    deleteAtPosition(pos, list);
}

/* Especificación
 * Objetivo: Copiar de forma segura un string a un destino, asegurando que no haya overflow o errores.
 * Entradas:
 *  - dest: String de destino.
 *  - src: String de origen.
 *  - size: Tamaño máximo, incluyendo la terminación null, del string de destino.
 *  - label: Identificador del string que se imprimirá en los mensajes de error.
 * Salida:
 *  - La función devuelve true en caso de que se haya copiado con éxito, false en caso contrario.
 */
bool safeStrCpy(char *dest, const char *src, size_t size, char *label) {
    bool out = false; //Auxiliar donde se recoge el output que devolverá return.

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
 * Objetivo: Procesar el comando 'N' para añadir una nueva consola a la lista.
 * Entradas:
 *  - commandNumber: Número de comando.
 *  - consoleId_p: consoleId.
 *  - sellerId_p: vendedor (seller).
 *  - consoleBrand_p: marca (consoleBrand).
 *  - consolePrice_p: precio (consolePrice).
 *  - list: Lista en la que se guardan los valores (tList).
 *
 * PostCD:
 *  - Se imprime el resultado y modifica la lista si el comando es válido.
 *  - Se imprime '+ Error:' seguido de una explicación en caso de fallo.
 *
 * Nota: No permite insertar duplicados, valores ID o seller con strings de longitud inválida y prevé errores básicos
 *  en precio.
 */
void processNewCommand(char *commandNumber, char *consoleId_p, char *sellerId_p, char *consoleBrand_p,
    char *consolePrice_p, tList *list) {
    tItemL newItem;         //Elemento item que se rellenará con la información del item a insertar.
    tPosL pos;              //La posición en la lista del item a insertar.
    tConsoleBrand brand;                              //Variable donde se guarda la conversión de string a enum.
    float priceFloat = safeStr2float(consolePrice_p); //Variable precio transformado a float para poder pasarlo al TAD.

    printf("********************\n");
    printf("%s N: console %s seller %s brand %s price %.2f\n", commandNumber, consoleId_p, sellerId_p, consoleBrand_p,
        priceFloat);

    if (priceFloat < 0) { //Error en conversión (o han se ha introducido un precio negativo).
        printf("+ Error: Invalid price value\n");
        return;
    }

    pos = findItem(consoleId_p, *list); //findItem devuelve LNULL en listas vacías.
    if (pos != LNULL) { //Control de duplicados.
        printf("+ Error: New not possible\n"); //El item ya existe.
        return;
    }

    if (!safeStrCpy(newItem.consoleId, consoleId_p, NAME_LENGTH_LIMIT, "ConsoleId") ||
        !safeStrCpy(newItem.seller, sellerId_p, NAME_LENGTH_LIMIT, "SellerId")) {
        printf("+ Error: New not possible\n"); //Hubo un error, la función safeStrCpy imprime más detalle.
        return;
    }

    if (!string2ConsoleBrand(consoleBrand_p, &brand)) {
        printf("+ Error: New not possible\n"); //La marca no es válida.
        return;
    }

    newItem.consoleBrand = brand;
    newItem.consolePrice = priceFloat;
    newItem.bidCounter = 0;
    createEmptyStack(&newItem.bidStack); //El stack se inicializa como vacío.

    if (insertItem(newItem, list)) {
        printf("* New: console %s seller %s brand %s price %.2f\n", newItem.consoleId, newItem.seller, consoleBrand_p,
            newItem.consolePrice);
    } else
        printf("+ Error: New not possible\n"); //Se produjo un fallo en el back del TAD.
}

/* Especificación:
 * Objetivo: Procesa el comando 'D' para eliminar una consola de la lista.
 * Entradas:
 *  - commandNumber: Número de comando.
 *  - consoleId_p: consoleId.
 *  - list: Lista en la que se encuentra la consola (tList).
 *
 * PostCD:
 *  - Imprime el resultado y modifica la lista si el comando es válido.
 *  - Imprime Error si el elemento no se encuentra.
 */
void processDeleteCommand(char *commandNumber, char *consoleId_p, tList *list) {
    printf("********************\n");
    printf("%s D: console %s\n", commandNumber, consoleId_p);

    tPosL pos = findItem(consoleId_p, *list); //También actúa como isEmptyList si devuelve LNULL.
    if (pos == LNULL) {
        printf("+ Error: Delete not possible\n"); //No se ha encontrado item o la lista se encuentra vacía.
        return;
    }
    tItemL item = getItem(pos,*list); //Se duplica antes de borrar para poder hacer el print.

    printf("* Delete: console %s seller %s brand %s price %.2f bids %d\n", item.consoleId, item.seller,
        consoleBrand2String(item.consoleBrand), item.consolePrice, item.bidCounter);

    handleDeleteConsole(pos, &item, list);

}

/* Especificación:
 * Objetivo: Procesa el comando 'B' para realizar una puja sobre una consola existente.
 * Entradas:
 *  - commandNumber: Número de comando.
 *  - consoleId_p: consoleId.
 *  - bidderId_p: pujador (bidder).
 *  - consolePrice_p: puja (consolePrice).
 *  - list: Lista en la que se guardan los valores (tList).
 *
 * PostCD:
 *  - Se imprime el resultado y se actualiza el precio y contador de pujas si la puja es válida.
 * Nota:
 *  - Se imprime '+ Error:' si la consola no existe, el vendedor es el pujador, el mayor pujador es el pujador, o el
 *      precio de puja es inferior al mayor precio listado.
 */
void processBidCommand(char *commandNumber, char *consoleId_p, char *bidderId_p, char *consolePrice_p, tList *list) {
    tPosL pos; //Posición del item sobre el que se puja.
    tItemL item; //Item sobre el que se puja.
    tItemS stackItem; //Elemento de la pila de pujas que guarda los metadatos de la misma.
    float highestBid; //Valor numérico de la puja más alta (si no hay pujas será el precio original).
    char *highestBidStr; //String del mayor pujador (si no hay pujas será el vendedor).
    float bidPrice = safeStr2float(consolePrice_p); //Variable precio como float para poder pasarlo al TAD y hacer comparaciones.

    printf("********************\n");
    printf("%s B: console %s bidder %s price %.2f\n", commandNumber, consoleId_p, bidderId_p, bidPrice);

    if (bidPrice < 0 || isEmptyList(*list)) { //Error en conversión (o han se ha introducido un precio negativo).
        printf("+ Error: Bid not possible\n");
        return;
    }

    pos = findItem(consoleId_p, *list);
    if (pos == LNULL) { //No se ha encontrado el item o la lista está vacía.
        printf("+ Error: Bid not possible\n");
        return;
    }

    item = getItem(pos, *list);
    //Los operadores ternarios permiten operar siempre las comprobaciones, independientemente de si existen pujas o no.
    stackItem = isEmptyStack(item.bidStack) ? (tItemS){.consolePrice = -1, .bidder = ""} : peek(item.bidStack);
    highestBid = (stackItem.consolePrice == -1) ? item.consolePrice : stackItem.consolePrice;
    highestBidStr = (stackItem.consolePrice == -1) ? item.seller : stackItem.bidder;

    if (strcmp(highestBidStr, bidderId_p) == 0 || strcmp(item.seller, bidderId_p) == 0 ||
        bidPrice <= highestBid) { //El vendedor de un item, o el pujador actual, no puede pujar en su propio item.
        printf("+ Error: Bid not possible\n");
        return;
    }

    if (!safeStrCpy(stackItem.bidder, bidderId_p, NAME_LENGTH_LIMIT, "BidderId")) {
        printf("+ Error: Bid not possible\n");
        return; //Fallo en StrCpy, seguramente sea un problema de overflow.
    }
    stackItem.consolePrice = bidPrice;

    if (push(stackItem, &item.bidStack)) {
        item.bidCounter++;
        updateItem(item, pos, list);
    } else {
        printf("+ Error: Bid not possible\n");
    }

    printf("* Bid: console %s bidder %s brand %s price %.2f bids %d\n", item.consoleId, stackItem.bidder,
        consoleBrand2String(item.consoleBrand), stackItem.consolePrice, item.bidCounter);
}

/*Especificación:
 *  Objetivo: Procesa el comando 'A' para adjudicar una consola al mayor postor
 * Entradas:
 *   - commandNumber: Número de comando.
 *   - consoleId_p: consoleId de la consola sobre la que se puja.
 *   - list: Lista en la que se encuentra la consola (tList).
 * PostCD:
 *   - Se imprime el usuario al que se le adjudica la consola y, posteriormente, se elimina la consola de la lista.
 *   - En caso de no existir elemento o pujas se imprimirá un error.
 */
void processAwardCommand(char *commandNumber, char *consoleId_p, tList *list ) {
    tPosL pos;  //La posición en la lista del item a adjudicar.
    tItemL item;//El item que se quiere adjudicar.
    tItemS top; //El item en la pila de pujas que guarda los datos de la mayor puja.

    printf("********************\n");
    printf("%s A: console %s\n", commandNumber, consoleId_p);

    pos = findItem(consoleId_p, *list);
    if (pos == LNULL) { //No se ha encontrado el item o la lista está vacía.
        printf("+ Error: Award not possible\n");
        return;
    }

    item = getItem(pos, *list);
    if (isEmptyStack(item.bidStack)) { //No se han encontrado pujas.
        printf("+ Error: Award not possible\n");
    } else {
        top = peek(item.bidStack);
        printf("* Award: console %s bidder %s brand %s price %.2f\n", item.consoleId, top.bidder,
            consoleBrand2String(item.consoleBrand) ,top.consolePrice);
        handleDeleteConsole(pos, &item, list);
    }
}

/* Especificación:
 * Objetivo: Procesa el comando 'S' para mostrar estadísticas y listing de las consolas registradas.
 * Entradas:
 *   - commandNumber: Número de comando.
 *   - list: Lista en la que se guardan los valores (tList).
 *
 * PostCD:
 *  - Imprime una lista con todas las consolas, junto con su mayor pujador (No bids en caso de no tener ninguna).
 *  - Imprime el número total de consolas, la suma de precios y el precio promedio por marca.
    - Imprime la "Top Bid", aquella consola cuyo incremento en precio (original/puja) sea el mayor.
 *  - Imprime '+ Error:' si la lista está vacía.
 */
void processStatsCommand(char *commandNumber, tList list) {
    int countNintendo = 0, countSega = 0;       //Contador del número de consolas de la marca.
    float sumNintendo = 0.0f, sumSega = 0.0f;   //Sumador de la suma de precios de todas las consolas de la marca.
    float avgNintendo, avgSega;                 //Precio medio de las consolas de la marca.

    tPosL pos = first(list); //Posición del elemento para el que se realizarán comprobaciones e impresiones en consola.

    tPosL topItemPos = NULL;  //Auxuliar que guarda la posición del item con la mejor puja.
    float maxIncrease = 0.0f; //Auxiliar que guarda el precio del item con la mejor puja.

    printf("********************\n");
    printf("%s S\n", commandNumber);

    if (isEmptyList(list)) {
        printf("+ Error: Stats not possible\n"); //La lista se encuentra vacía.
        return;
    }

    while (pos != LNULL) { /*Se atraviesa toda la lista hasta llegar al final para calcular # de consolas por marca,
                            *y hacer el listing de todas en consola.
                            */
        tItemL item = getItem(pos, list); //El item guardado en la posición que estamos comprobando.

        printf("Console %s seller %s brand %s price %.2f", item.consoleId, item.seller,
            consoleBrand2String(item.consoleBrand), item.consolePrice);

        if (isEmptyStack(item.bidStack)) { //No se han creado pujas.
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


        if (item.consoleBrand == nintendo) { //Se aumenta el contador y el sumatorio.
            countNintendo++;
            sumNintendo += item.consolePrice;
        } else if (item.consoleBrand == sega) {
            countSega++;
            sumSega += item.consolePrice;
        }

        pos = next(pos, list);
    }

    avgNintendo = (countNintendo > 0) ? sumNintendo / (float)countNintendo : 0; //cálculo de promedio que evita div/0
    avgSega = (countSega > 0) ? sumSega / (float)countSega : 0;                 //cálculo de promedio que evita div/0

    printf("Brand     Consoles    Price  Average\n");
    printf("Nintendo  %8d %8.2f %8.2f\n", countNintendo, sumNintendo, avgNintendo);
    printf("Sega      %8d %8.2f %8.2f\n", countSega, sumSega, avgSega);

    if (topItemPos != NULL) {
        tItemL topItem = getItem(topItemPos, list);
        tItemS topBid = peek(topItem.bidStack);
        printf("Top bid: console %s seller %s brand %s price %.2f bidder %s top price %.2f increase %.2f%%\n",
            topItem.consoleId, topItem.seller, consoleBrand2String(topItem.consoleBrand), topItem.consolePrice,
            topBid.bidder, topBid.consolePrice, maxIncrease);
    } else {
        printf("Top bid not possible\n");
    }
}

/* Especificación:
 * Objetivo: Eliminar de la lista aquellas consolas que no tengan ninguna puja.
 * Entradas:
 *   - commandNumber: Número de comando.
 *   - list: Lista en la que se guardan los valores (tList).
 * PostCD:
 *   - Se eliminan de la lista aquellas consolas que no tengan pujas. En caso de no haber consolas, o que ninguna
 *      esté libre de pujas, se imprimirá un error; de lo contrario se imprimirá que consolas se eliminan.
 */
void processRemoveCommand(char *commandNumber, tList *list) {
    bool removed = false;     //Auxiliar que lleva registro de si se ha eliminado alguna consola.
    tPosL pos = first(*list); //Posición del elemento para el que se realizarán comprobaciones.
    tItemL item;              //El item que se quiere comprobar.
    tPosL nextPos;            //La posición siguiente al item que se quiere comprobar.

    printf("********************\n");
    printf("%s R\n", commandNumber);

    if (isEmptyList(*list)) {
        printf("+ Error: Remove not possible\n"); //list is empty
        return;
    }

    while (pos != LNULL) { /*Atravesamos toda la lista hasta llegar al final para calcular # de consolas por marca, y
                            *hacer el listing de todas en consola.
                            */
        item = getItem(pos, *list);
        nextPos = next(pos, *list);

        if (isEmptyStack(item.bidStack)) {
            removed = true;
            printf("Removing console %s seller %s brand %s price %.2f bids %d\n", item.consoleId, item.seller,
                consoleBrand2String(item.consoleBrand), item.consolePrice, item.bidCounter);

            handleDeleteConsole(pos, &item, list);
        } else { /*Al borrar un elemento de la lista, el nodo posterior pasa a ser el nodo eliminado. Si se invalidara
                  * el penúltimo nodo, next daría bad access. Además, si se actualizase a next tras eliminar,
                  * la siguiente consola no se eliminaría.
                  */
            pos = nextPos;
        }
    }

    if (!removed) {
        printf("* Error: Remove not possible\n");
    }
}

/* Especificación:
 * Objetivo: Procesar el comando 'I' para invalidar las consolas con pujas irregulares.
 * Entradas:
 *   - commandNumber: Número de comando.
 *   - list: Lista en la que se guardan los valores (tList).
 *
 * PostCD:
 *  - Se imprime el número total de consolas, la suma de precios y el precio promedio por marca.
 *  - Se imprime '+ Error:' si la lista está vacía.
 */
void processInvalidateBidsCommand(char *commandNumber, tList *list) {
    int totalBids = 0, numConsoles = 0; //Auxiliar de tipo contador.
    tPosL pos;                          //Posición del elemento para el que se realizarán comprobaciones.
    float averageBids;                  //Promedio de las pujas.
    float range;                        //Rango de las pujas.
    bool invalidated = false;           //Auxiliar que lleva registro de si se ha invalidado alguna puja.

    printf("********************\n");
    printf("%s I\n", commandNumber);

    if (isEmptyList(*list)) {
        printf("+ Error: InvalidateBids not possible\n");
        return;
    }

    pos = first(*list);

    //Se atraviesa la lista para recoger los promedios.
    while (pos != LNULL) {
        tItemL item = getItem(pos, *list);
        totalBids += item.bidCounter;
        numConsoles++;
        pos = next(pos, *list);
    }

    averageBids = (float)totalBids / (float)numConsoles;
    range = 2 * averageBids;
    invalidated = false;

    pos = first(*list);

    //Se atraviesa la lista y se eliminan las pujas si se supera el rango.
    while (pos != LNULL) {
        tItemL item = getItem(pos, *list);

        if ((float)item.bidCounter > range) {
            clearStack(&item.bidStack);

            printf("* InvalidateBids: console %s seller %s brand %s price %.2f bids %d average bids %.2f\n", item.consoleId,
                item.seller, consoleBrand2String(item.consoleBrand), item.consolePrice, item.bidCounter, averageBids);
            item.bidCounter = 0;
            updateItem(item,pos,list);
            invalidated = true;
        }

        pos = next(pos, *list);
    }

    if (!invalidated) {
        printf("+ Error: InvalidateBids not possible\n"); //Ninguna consola fue invalidada.
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
            processStatsCommand(commandNumber, *list);
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
    tList list; //Variable lista que se pasa a las funciones
    createEmptyList(&list); //Se inicializa la lista.

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