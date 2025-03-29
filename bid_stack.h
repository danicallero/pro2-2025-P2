/*
* TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: 3.3
 * DATE: 25 / 03 / 25
 */

#ifndef BID_STACK_H
#define BID_STACK_H

#include "types.h"
#include <stdbool.h>

#define SMAX 25 //items máximo
#define SNULL -1 //pos nula

typedef struct {
    tUserId bidder;
    tConsolePrice consolePrice;
}tItemS;

typedef int tPosS;

typedef struct {
    tItemS data[SMAX]; // Array para almacenar los elementos
    tPosS top; // Última posición ocupada en la lista (-1 para lista vacía)
} tStack;

/*
 * Objetivo: Crea una pila vacía.
 * Entradas:
 *   - S: puntero a la pila a inicializar.
 * Precondiciones:
 *   - La pila debe estar declarada.
 * Postcondiciones:
 *   - La pila queda inicializada y se marca como vacía.
 */
void createEmptyStack(tStack *S);

/*
 * Objetivo: Inserta un elemento en la cima de la pila.
 * Entradas:
 *   - d: elemento a insertar (tItemS).
 *   - S: puntero a la pila.
 * Salidas:
 *   - True si se insertó el elemento correctamente, false en caso contrario.
 * Postcondiciones:
 *   - Se incrementa el tamaño de la pila.
 */
bool push(tItemS d, tStack *S);

/*
 * Objetivo: Elimina el elemento en la cima de la pila.
 * Entradas:
 *   - S: puntero a la pila.
 * Precondiciones:
 *   - La pila no está vacía.
 */
void pop(tStack *S);

/*
 * Objetivo: Devuelve el elemento en la cima de la pila.
 * Entradas:
 *   - S: Pila.
 * Salidas:
 *   - El tItemS de la cima
 * Precondiciones:
 *   - La pila no está vacía.
 */
tItemS peek(tStack S);

/*
 * Objetivo: Determina si una pila está vacía.
 * Entradas:
 *   - S: pila a comprobar.
 * Salidas:
 *   - Devuelve true si la pila está vacía, false en caso contrario.
 */
bool isEmptyStack(tStack S);

#endif
