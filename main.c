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

/**
 * @file main.c
 * @brief Gestión de plataforma de pujas de consolas.
 *
 * @note
 * Esta práctica está comentada en formato Doxygen (/** ... * /), pero se puede ignorar si no se utiliza un IDE con
 * soporte o no se tiene Doxygen instalado.
 */

/*
 * Como guía rápida en caso de no conocer la notación doxygen:
 * @brief:          Breve descripción de la función.
 * @param:          Parámetros: [in] (entrada), [out] (salida), [in,out] (entrada y salida).
 * @return/retval:  Valor devuelto (si existe).
 * @pre:            Precondiciones necesarias antes de ejecutar la función.
 * @post:           Cambios garantizados después de la ejecución.
 * @attention:      Avisos importantes sobre el uso de la función.
 * @remark/note:    Información adicional relevante.
 */

/**
 * @brief Número máximo de caracteres por línea en el archivo de entrada.
 */
#define MAX_BUFFER 255

/**
 * @brief Transforma un valor tConsoleBrand a una cadena de caracteres equivalente.
 *
 * @param[in] brand Valor enum de tipo tConsoleBrand.
 *
 * @retval string Cadena de caracteres equivalente al tConsoleBrand dado, o "unknown" si se ha pasado otro enum.
 */
char *consoleBrand2String(const tConsoleBrand brand) {
    switch (brand) {
        case nintendo: return "nintendo";
        case sega: return "sega";
        default: return "unknown";
    }
}

/**
 * @brief Transforma las mayúsculas de un string a minúsculas.
 *
 * @param[in,out] str Puntero a un string.
 *
 * @pre El puntero 'str' debe apuntar a un string válido.
 * @post El string se modifica y ya no contiene mayúsculas.
 *
 * @attention Esta función modifica 'str' diréctamente, NO devuelve un duplicado.
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

/**
 * @brief Transforma una cadena de caracteres en su valor equivalente del enum tConsoleBrand.
 *
 * @param[in,out] source Cadena que representa una marca.
 * @param[out] dest Puntero a una variable de tipo tConsoleBrand donde se almacenará el valor del enum.
 *
 * @retval true La marca existe dentro de los valores definidos en tConsoleBrand
 * @retval false No se encontró la marca entre los valores de tConsoleBrand.
 *
 * @post –  La cadena 'source' se convierte a minúsculas al llamar a la función.
 * @post –  Si la función devuelve true, 'dest' cambiará su valor y contendrá el valor correspondiente del enum tConsoleBrand.
 *
 * @attention Llamar a esta función transformará el string 'source' que se le pase a minúsculas.
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

/**
 * @brief Transforma de forma segura un valor de tipo string a float.
 *
 * @param[in] str Puntero al string que se quiere transformar.
 *
 * @retval float que contiene el valor representado por el string pasado, o
 * @retval float con valor -1.0f en caso de error.
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

/**
 * @brief Vacía los contenidos de una pila tStack.
 *
 * @param[in,out] stack Puntero a la pila tStack que se quiere vaciar.
 *
 * @pre El puntero debe apuntar a una pila inicializada.
 * @post La pila queda vaciada.
 */
void clearStack(tStack *stack) {
    while (!isEmptyStack(*stack)) { //Se atraviesa el stack mientras no esté vacío.
        pop(stack);
    }
}

/**
 * @brief Gestiona de forma unificada la eliminación de consolas cumpliendo con la precondición de vaciar la pila de
 * pujas previamente.
 *
 * @param[in] pos Posición tPosL del elemento que se quiere eliminar.
 * @param[in,out] item Puntero al tItemL que se encuentra en la tPosL 'pos'.
 * @param[in,out] list Puntero a la lista en la que se encuentra el item.
 *
 * @pre - 'list' debe apuntar a una tList inicializada.
 * @pre - La tPosL 'pos' debe ser una posición válida que contenga un elemento de la lista.
 * @pre - El puntero 'item' debe apuntar al tItemL que se encuentra en la posición 'pos'.
 *
 * @post - La pila de pujas del elemento 'item' queda vaciada.
 * @post - El elemento 'item' se elimina de la lista, y como consecuencia otros elementos pueden haber sido desplazados.
 */
void handleDeleteConsole(tPosL pos, tItemL *item, tList *list) {
    clearStack(&item->bidStack);
    updateItem(*item, pos, list); //Actualizamos el item con la pila borrada.
    deleteAtPosition(pos, list);
}

/**
 * @brief Copia de forma segura una cadena de caracteres de un string de origen a otro de destino, asegurando que no se
 * produzca overflow y avisando de posibles errores.
 *
 * @param[out] dest Puntero al de destino, que se sobreescribirá con los contenidos de 'src'.
 * @param[in] src Puntero al string de origen. No se modifica.
 * @param[in] size Valor de tipo size_t. Tamaño máximo del string incluyendo la terminación '\0'.
 * @param[in] label Nombre descriptivo del string que se imprimirá en el mensaje de error.
 *
 * @retval true La cadena de caracteres se copió exitosamente.
 * @retval false La cadena de caracteres no se copió exitosamente, se imprime en consola un mensaje explicativo del error.
 */
bool safeStrCpy(char *dest, const char *src, const size_t size, const char *label) {
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

/**
 * @brief Procesa el comando 'N' para añadir una nueva consola en una lista tList.
 *
 * @param[in] commandNumber Cadena de caracteres que contiene el número de comando. Este valor solo ayuda a mantener
 * registro de la petición, no tiene otro uso.
 * @param[in] consoleId_p Cadena que contiene el identificador de la consola a insertar.
 * @param[in] sellerId_p Cadena que contiene el identificador del vendedor.
 * @param[in] consoleBrand_p Cadena que representa la marca de la consola.
 * @param[in] consolePrice_p Cadena que representa el precio de la consola.
 * @param[in,out] list Puntero a la lista donde se guardará el item.
 *
 * @post - Se imprime el resultado y se modifica la lista si el comando está bien formulado.
 * @post - Se imprime '+ Error:' seguido de un aviso en caso de fallo.
 *
 * @remark No permite la inserción de duplicados, cadenas de caracteres con strings de longitud inválida, y evita errores
 * básicos.
 */
void processNewCommand(char *commandNumber, char *consoleId_p, char *sellerId_p, char *consoleBrand_p,
    const char *consolePrice_p, tList *list) {
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

    if (!string2ConsoleBrand(consoleBrand_p, &brand)) { //¡¡¡Sé que source se pasa a minúsculas, es a propósito!!!
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

/**
 * @brief Procesa el comando 'D' para eliminar una consola de la lista.
 *
 * @param[in] commandNumber Cadena con el número de comando.
 * @param[in] consoleId_p Identificador de la consola a eliminar.
 * @param[in,out] list Lista en la que se encuentra la consola (tList).
 *
 * @post Se imprime el resultado y se modifica la lista si el comando es válido.
 * @post Imprime "+ Error: Delete not possible" si el elemento no se encuentra.
 */

void processDeleteCommand(char *commandNumber, char *consoleId_p, tList *list) {
    tPosL pos;   //Posición del item sobre el que se puja.
    tItemL item; //Item que se elimina.

    printf("********************\n");
    printf("%s D: console %s\n", commandNumber, consoleId_p);

    pos = findItem(consoleId_p, *list); //También actúa como isEmptyList si devuelve LNULL.
    if (pos == LNULL) {
        printf("+ Error: Delete not possible\n"); //No se ha encontrado item o la lista se encuentra vacía.
        return;
    }
    item = getItem(pos,*list); //Se duplica antes de borrar para poder hacer el print.

    printf("* Delete: console %s seller %s brand %s price %.2f bids %d\n", item.consoleId, item.seller,
        consoleBrand2String(item.consoleBrand), item.consolePrice, item.bidCounter);

    handleDeleteConsole(pos, &item, list);

}

/**
 * @brief Procesa el comando 'B' para realizar una puja sobre una consola existente.
 *
 * @param[in] commandNumber Cadena con el número de comando.
 * @param[in] consoleId_p Identificador de la consola.
 * @param[in] bidderId_p Identificador del pujador.
 * @param[in] consolePrice_p Cadena con el importe de la puja.
 * @param[in,out] list Lista en la que se guardan los valores (tList).
 *
 * @post Se imprime el resultado y se actualiza el precio y contador de pujas si la puja es válida.
 * @note Imprime "+ Error: Bid not possible" si la consola no existe, el vendedor es el pujador,
 * el pujador actual ya era el último pujador, la puja no supera la anterior, o hay overflow.
 */
void processBidCommand(char *commandNumber, char *consoleId_p, char *bidderId_p, char *consolePrice_p, tList *list) {
    tPosL pos;          //Posición del item sobre el que se puja.
    tItemL item;        //Item sobre el que se puja.
    tItemS stackItem;   //Elemento de la pila de pujas que guarda los metadatos de la misma.
    float highestBid;   //Valor numérico de la puja más alta (si no hay pujas será el precio original).
    char *highestBidStr;//String del mayor pujador (si no hay pujas será el vendedor).
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

/**
 * @brief Procesa el comando 'A' para adjudicar una consola al mayor postor.
 *
 * @param[in] commandNumber Cadena con el número de comando.
 * @param[in] consoleId_p Identificador de la consola a adjudicar.
 * @param[in,out] list Lista en la que se encuentra la consola (tList).
 *
 * @post Imprime "* Award: console CC bidder UU brand BB price PP" con los datos del ganador.
 * @post Elimina la consola de la lista tras vaciar su pila de pujas.
 * @post Imprime "+ Error: Award not possible" si no existe la consola o no hay pujas.
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

/**
 * @brief Procesa el comando 'S' para mostrar estadísticas y listado de las consolas registradas.
 *
 * @param[in] commandNumber Cadena con el número de comando.
 * @param[in] list Lista en la que se guardan los valores (tList).
 *
 * @post Imprime por cada consola:
 *       - "Console CC seller UU brand BB price PP bids II top bidder UU"
 *       - O ". No bids" si no hay pujas.
 * @post Imprime un resumen por marca con número de consolas, suma de precios y precio medio.
 * @post Imprime la "Top bid" con mayor incremento porcentual.
 * @post Imprime "+ Error: Stats not possible" si la lista está vacía.
 */
void processStatsCommand(char *commandNumber, tList list) {
    int countNintendo = 0, countSega = 0;       //Contador del número de consolas de la marca.
    float sumNintendo = 0.0f, sumSega = 0.0f;   //Sumador de la suma de precios de todas las consolas de la marca.
    float avgNintendo, avgSega;                 //Precio medio de las consolas de la marca.

    tPosL pos;                //Posición del elemento para el que se realizarán comprobaciones e impresiones en consola.
    tPosL topItemPos = NULL;  //Auxiliar que guarda la posición del item con la mejor puja.
    float maxIncrease = 0.0f; //Auxiliar que guarda el precio del item con la mejor puja.

    float increase;     //Precio de la puja más alta. (para cambiar en cada iteración)
    float topBid;       //Mayor incremento en precio. (para cambiar en cada iteración)

    tItemL topItem;     //Auxiliar donde se guardará la consola con mayor puja.
    tItemS topStack;    //Auxiliar donde se guardará la mayor puja.

    printf("********************\n");
    printf("%s S\n", commandNumber);

    if (isEmptyList(list)) {
        printf("+ Error: Stats not possible\n"); //La lista se encuentra vacía.
        return;
    }

    pos = first(list);

    while (pos != LNULL) { /*Se atraviesa toda la lista hasta llegar al final para calcular # de consolas por marca,
                            *y hacer el listing de todas en consola.
                            */
        tItemL item = getItem(pos, list); //El item guardado en la posición que estamos comprobando.

        printf("Console %s seller %s brand %s price %.2f", item.consoleId, item.seller,
            consoleBrand2String(item.consoleBrand), item.consolePrice);

        if (isEmptyStack(item.bidStack)) { //No se han creado pujas.
            printf(". No bids\n");
        } else {
            topBid = peek(item.bidStack).consolePrice;
            increase = ((topBid - item.consolePrice) / item.consolePrice) * 100.0f;

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

    printf("\nBrand     Consoles    Price  Average\n");
    printf("Nintendo  %8d %8.2f %8.2f\n", countNintendo, sumNintendo, avgNintendo);
    printf("Sega      %8d %8.2f %8.2f\n", countSega, sumSega, avgSega);

    if (topItemPos != NULL) {
        topItem = getItem(topItemPos, list);
        topStack = peek(topItem.bidStack);
        printf("Top bid: console %s seller %s brand %s price %.2f bidder %s top price %.2f increase %.2f%%\n",
            topItem.consoleId, topItem.seller, consoleBrand2String(topItem.consoleBrand), topItem.consolePrice,
            topStack.bidder, topStack.consolePrice, maxIncrease);
    } else {
        printf("Top bid not possible\n");
    }
}

/**
 * @brief Elimina de la lista aquellas consolas que no tengan ninguna puja.
 *
 * @param[in] commandNumber Cadena con el número de comando.
 * @param[in,out] list Lista en la que se guardan los valores (tList).
 *
 * @post Por cada consola sin pujas, imprime
 *       "Removing console CC seller UU brand BB price PP bids II" y la elimina de la lista.
 * @post Imprime "+ Error: Remove not possible" si no hay consolas o ninguna sin pujas.
 */
void processRemoveCommand(char *commandNumber, tList *list) {
    bool removed = false;     //Auxiliar que lleva registro de si se ha eliminado alguna consola.
    tPosL pos;                //Posición del elemento para el que se realizarán comprobaciones.
    tItemL item;              //El item que se quiere comprobar.
    tPosL nextPos;            //La posición siguiente al item que se quiere comprobar.

    printf("********************\n");
    printf("%s R\n", commandNumber);

    if (isEmptyList(*list)) {
        printf("+ Error: Remove not possible\n");
        return;
    }

    pos = first(*list);     //Llamamos a first después de comprobar que la lista no esté vacía, por la preCD.

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
        printf("+ Error: Remove not possible\n");
    }
}

/**
 * @brief Procesa el comando 'I' para invalidar las consolas con pujas irregulares.
 *
 * @param[in] commandNumber Cadena con el número de comando.
 * @param[in,out] list Lista en la que se guardan los valores (tList).
 *
 * @post Calcula el número medio de pujas y define un rango = 2 × media.
 * @post Para cada consola cuyo bidCounter > rango, vacía su pila y:
 *       imprime "* InvalidateBids: console CC seller UU brand BB price PP bids II average bids AA".
 * @post Imprime "+ Error: InvalidateBids not possible" si la lista está vacía o no cumple condición.
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

/**
 * @brief Procesa los comandos recibidos y llama a las funciones correspondientes según el tipo de comando.
 *
 * @param[in] commandNumber Cadena con el número de comando recibido.
 * @param[in] command Carácter que indica el tipo de comando ('N','D','B','A','R','S','I').
 * @param[in] param1 Primer parámetro del comando (si aplica).
 * @param[in] param2 Segundo parámetro del comando (si aplica).
 * @param[in] param3 Tercer parámetro del comando (si aplica).
 * @param[in] param4 Cuarto parámetro del comando (si aplica).
 * @param[in,out] list Puntero a la lista donde se aplican las operaciones.
 *
 * @pre list debe estar inicializada.
 * @post Llama a la función processXCommand correspondiente.
 * @post Si el comando no es reconocido, no realiza ninguna acción.
 *
 * @note Función proporcionada por la universidad, modificada ligeramente.
 */
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

/**
 * @brief Lee los comandos desde un archivo y los procesa uno por uno.
 *
 * @param[in] filename Nombre del archivo que contiene los comandos.
 * @param[in,out] list Puntero a la lista donde se almacenará la información.
 *
 * @pre El archivo debe existir y ser accesible.
 * @post Se procesan todos los comandos hasta finalizar el archivo.
 * @post La lista puede quedar modificada por la ejecución de las operaciones.
 * @post Si el archivo no se encuentra, se muestra un mensaje de error.
 *
 * @note Función proporcionada por la universidad, modificada ligeramente.
 */

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