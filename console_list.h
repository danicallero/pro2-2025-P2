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

#define LNULL NULL //Valor fijo para una posición nula.

typedef struct tItemL {
 tUserId seller;
 tConsoleId consoleId;
 tConsoleBrand consoleBrand;
 tConsolePrice consolePrice;
 tBidCounter bidCounter;
 tStack bidStack;
} tItemL;

typedef struct tNode *tPosL; //Se define tPosL como puntero a tNode.

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

/*
 * Objetivo: Crea una lista vacía.
 * Entradas:
 *   - list: puntero a la lista a inicializar.
 * Precondiciones:
 *   - La lista debe estar declarada.
 * Postcondiciones:
 *   - La lista queda inicializada y se marca como vacía.
 */
void createEmptyList(tList *L);

/*
 * Objetivo: Determina si una lista está vacía.
 * Entradas:
 *   - L: lista a comprobar.
 * Salidas:
 *   - Devuelve true si la lista está vacía, false en caso contrario.
 */
bool isEmptyList(tList L);

/*
 * Objetivo: Inserta un elemento en la lista manteniendo el orden
 * Entradas:
 *   - d: Elemento a insertar (tItemL).
 *   - L: Puntero a la lista.
 * Salidas:
 *   - True si se insertó el elemento correctamente, false en caso contrario.
 * Postcondiciones:
 *   - Se incrementa el tamaño de la lista y se guarda el nuevo elemento en la posición correspondiente.
 *   - Los elementos en posiciones posteriores pueden haberse desplazado.
 */
bool insertItem(tItemL data_d, tList *L);

/*
 * Objetivo: Elimina un elemento que ocupa la posición indicada de la lista.
 * Entradas:
 *   - p: Posición del elemento a eliminar (tPosL).
 *   - L: Puntero a la lista.
 * Precondiciones:
*   - La posición 'p' debe ser válida, y la consola en dicha posición debe tener una pila de pujas vacía.
 * Postcondiciones:
 *   - Se reduce el tamaño de la lista al eliminar de la misma el elemento.
 *   - Los elementos posteriores a la posición 'p' pueden haberse desplazado.
 */
void deleteAtPosition(tPosL p, tList *L);

/*
 * Objetivo: Modifica el contenido de un elemento situado en la posición indicada.
 * Entradas:
 *   - d: Nuevo valor del elemento (tItemL).
 *   - p: Posición del elemento a modificar (tPosL).
 *   - L: Puntero a la lista.
 * Precondiciones:
 *   - La posición 'p' debe ser válida.
 * Postcondiciones:
 *   - El contenido del elemento es actualizado, y el orden de la lista queda intacto.
 */
void updateItem(tItemL d, tPosL p, tList *L);

/*
 * Objetivo: Obtiene el elemento de una lista en la posición indicada.
 * Entradas:
 *   - p: Posición del elemento (tPosL).
 *   - L: Lista de donde obtener el elemento.
 * Salidas:
 *   - El elemento en la posición p (tItemL)
 * Precondiciones:
 *   - La posición 'p' debe ser válida.
 */
tItemL getItem(tPosL p, tList L);

/*
 * Objetivo: Devuelve la posición del primer elemento de la lista.
 * Entradas:
 *   - L: Lista a evaluar.
 * Salidas:
 *   - Posición del primer elemento (tPosL).
 * Precondiciones:
 *   - La lista no debe estar vacía.
 * Nota de implementación:
 *   - Por la naturaleza de la implementación dinámica, la función devuelve LNULL si la llamas con una lista vacía e inicializada.
 */
tPosL first(tList L);

/*
 * Objetivo: Devuelve la posición del último elemento de la lista.
 * Entradas:
 *   - L: Lista a evaluar.
 * Salidas:
 *   - Posición del último elemento (tPosL)
 * Precondiciones:
 *   - La lista no debe estar vacía.
 * Nota de implementación:
 *   - Por la naturaleza de la implementación dinámica, la función devuelve LNULL si la llamas con una lista vacía e inicializada.
 */
tPosL last(tList L);

/*
 * Objetivo: Devuelve la posición del nodo anterior a uno dado.
 * Entradas:
 *   - p: Posición actual (tPosL).
 *   - L: Lista en la que buscar la posición anterior.
 * Salidas:
 *   - Posición anterior (tPosL) o LNULL si 'p' es el primer nodo de la lista.
 * Precondiciones:
 *   - La posición 'p' debe ser válida.
 */
tPosL previous(tPosL p, tList L);

/*
 * Objetivo: Devuelve la posición del nodo siguiente a uno dado.
 * Entradas:
 *   - p: Posición actual (tPosL).
 *   - L: Lista en la que buscar la posición siguiente.
 * Salidas:
 *   - Posición siguiente (tPosL) o LNULL si 'p' es la última.
 * Precondiciones:
 *   - La posición 'p' debe ser válida.
 */
tPosL next(tPosL p, tList L);

/*
 * Objetivo: Busca el nodo que contiene la primera ocurrencia de una consola con el identificador proporcionado.
 * Entradas:
 *   - id: Identificador de la consola (tConsoleId).
 *   - L: Lista en la que buscar.
 * Salidas:
 *   - Posición del primer elemento si se encuentra (tPosL), o LNULL si no se ha encontrado ninguna ocurrencia del item en la lista.
 */
tPosL findItem(tConsoleId id, tList L);
#endif