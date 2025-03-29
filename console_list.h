/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: Yago Regueiro Piera LOGIN 2: y.regueiro.piera@udc.es
 * GROUP: 3.3
 * DATE: 25 / 03 / 25
 */

#ifndef CONSOLE_LIST_H
#define CONSOLE_LIST_H

#include "types.h"
#include "bid_stack.h"
#include <stdbool.h>
#include <stdlib.h>

#define LNULL NULL //en dinámicas usaremos NULL porque trabajamos con punteros

typedef struct tItemL {
 tUserId seller;
 tConsoleId consoleId;
 tConsoleBrand consoleBrand;
 tConsolePrice consolePrice;
 tBidCounter bidCounter;
 tStack bidStack;
} tItemL;

typedef struct tNode *tPosL; //definimos tPosL como puntero a tNode

struct tNode{
  tItemL data;
  tPosL next;
};

typedef tPosL tList;
/*
 * tList es un alias de tPosL, que a su vez apunta a un struct tNode. Por ello tList será puntero al primer elemento de la lista.
 * Si la lista está vacía la inicializamos apuntando a LNULL, si tiene algún elemento, este habitará la primera pos (tList),
 * o se insertará a continuación enlazando el nuevo nodo en la tPosL next.
 * El último nodo apunta a LNULL.
 */

void createEmptyList(tList *L);
/*
 * Objetivo: Crea una lista vacía.
 * Entradas:
 *   - list: puntero a la lista a inicializar.
 * Precondiciones:
 *   - La lista debe estar declarada.
 * Postcondiciones:
 *   - La lista queda inicializada y se marca como vacía.
 */

bool isEmptyList(tList L);
/*
 * Objetivo: Determina si una lista está vacía.
 * Entradas:
 *   - L: lista a comprobar.
 * Salidas:
 *   - Devuelve true si la lista está vacía, false en caso contrario.
 */

bool insertItem(tItemL d, tList *L);
/*
 * Objetivo: Inserta un elemento en la lista manteniendo el orden
 * Entradas:
 *   - d: elemento a insertar (tItemL).
 *   - L: puntero a la lista.
 * Salidas:
 *   - True si se insertó el elemento correctamente, false en caso contrario.
 * Postcondiciones:
 *   - Se incrementa el tamaño de la lista.
 *   - Los elementos en posiciones posteriores pueden haberse desplazado.
 */

void deleteAtPosition(tPosL p, tList *L);
/*
 * Objetivo: Elimina un elemento que ocupa la posición indicada de la lista.
 * Entradas:
 *   - p: posición del elemento a eliminar (tPosL).
 *   - L: puntero a la lista.
 * Precondiciones:
*   - La posición p debe ser válida, y la consola en dicha posición tiene una pila de pujas vacía.
 * Postcondiciones:
 *   - Se reduce el tamaño de la lista.
 *   - Los elementos posteriores a la posición 'p' pueden haberse desplazado.
 */

void updateItem(tItemL d, tPosL p, tList *L);
/*
 * Objetivo: Modifica el contenido de un elemento situado en la posición indicada.
 * Entradas:
 *   - d: nuevo valor del elemento (tItemL).
 *   - p: posición del elemento a modificar (tPosL).
 *   - L: puntero a la lista.
 * Precondiciones:
 *   - La posición p debe ser válida.
 * Postcondiciones:
 *   - El contenido del elemento es actualizado, y el orden de la lista queda intacto.
 */

tItemL getItem(tPosL p, tList L);
/*
 * Objetivo: Obtiene el elemento de una lista en la posición indicada.
 * Entradas:
 *   - p: posición del elemento (tPosL).
 *   - L: lista de donde obtener el elemento.
 * Salidas:
 *   - El elemento en la posición p (tItemL)
 * Precondiciones:
 *   - La posición p debe ser válida.
 */

tPosL first(tList L);
/*
 * Objetivo: Devuelve la posición del primer elemento de la lista.
 * Entradas:
 *   - L: lista a evaluar.
 * Salidas:
 *   - Posición del primer elemento (tPosL).
 * Precondiciones:
 *   - La lista no debe estar vacía.
 * Nota de implementación:
 *   - Por la naturaleza de la implementación dinámica, la fc devuelve LNULL si la llamas con una lista vacía e inicializada.
 */

tPosL last(tList L);
/*
 * Objetivo: Devuelve la posición del último elemento de la lista.
 * Entradas:
 *   - L: lista a evaluar.
 * Salidas:
 *   - Posición del último elemento (tPosL)
 * Precondiciones:
 *   - La lista no debe estar vacía.
 * Nota de implementación:
 *   - Por la naturaleza de la implementación dinámica, la fc devuelve LNULL si la llamas con una lista vacía e inicializada.
 */

tPosL previous(tPosL p, tList L);
/*
 * Objetivo: Devuelve la posición anterior a una posición dada.
 * Entradas:
 *   - p: posición actual (tPosL).
 *   - L: lista en la que buscar la posición anterior.
 * Salidas:
 *   - Posición anterior (tPosL) o LNULL si 'p' es la primera.
 * Precondiciones:
 *   - La posición p debe ser válida.
 */

tPosL next(tPosL p, tList L);
/*
 * Objetivo: Devuelve la posición siguiente a una posición dada.
 * Entradas:
 *   - p: posición actual (tPosL).
 *   - L: lista en la que buscar la posición siguiente.
 * Salidas:
 *   - Posición siguiente (tPosL) o LNULL si 'p' es la última.
 * Precondiciones:
 *   - La posición p debe ser válida.
 */

tPosL findItem(tConsoleId id, tList L);
/*
 * Objetivo: Busca el primer elemento en la lista con un identificador.
 * Entradas:
 *   - id: identificador de la consola (tConsoleId).
 *   - L: lista en la que buscar.
 * Salidas:
 *   - Posición del primer elemento si se encuentra (tPosL), o LNULL si no existe.
 */
#endif