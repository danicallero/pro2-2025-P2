/*
* TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: Yago Regueiro Piera LOGIN 2: y.regueiro.piera@udc.es
 * GROUP: 3.3
 * DATE: 25 / 03 / 25
 */

#ifndef BID_STACK_H
#define BID_STACK_H

#include "types.h"
#include <stdbool.h>

/**
 * @file bid_stack.h
 * @brief Gestion del TAD pila estática de pujas.
 *
 * @note
 * Este código contiene especificaciones tanto en formato tradicional como en formato Doxygen.
 * El contenido de ambas es equivalente, y se ha mantenido el formato clásico como extra
 * para facilitar la corrección manual o en IDEs sin soporte para Doxygen.
 */

/**
 *@brief Nº de ítems máximo en tStack.
 */
#define SMAX 25
/**
 *@brief Valor fijo para una posición del tStack nula.
 */
#define SNULL -1

/**
 * @brief Estructura que representa un ítem (elemento) de la pila tStack.
 * Contiene la información de una puja realizada por un usuario sobre una consola
 */
typedef struct {
    tUserId bidder;             /**< ID del usuario que puja.*/
    tConsolePrice consolePrice; /**< Precio de la consola. */
} tItemS;

/** @brief Tipo que representa una posición en la pila. */
typedef int tPosS;

/**
 * @brief Representa la estructura de datos tipo pila.
 *
 * La pila se implementa como un array estático de ítems con una posición superior
 * que indica la la posición del último elemento útil.
 */
typedef struct {
    tItemS data[SMAX]; /**< Array que almacena los elementos de la pila. */
    tPosS top;         /**< Última posición ocupada en la pila (SNULL si tStack está vacío). */
} tStack;

/*
 * Objetivo: Inicializa un stack vacío.
 * Entradas:
 *   - S: Puntero al stack a inicializar.
 * Precondiciones:
 *   - El stack debe estar declarado.
 * Postcondiciones:
 *   - El stack queda inicializado y marcado como vacío.
 */
/**
 * @brief Inicializa un stack vacío.
 * @param[out] S Puntero al stack a inicializar.
 * @pre El stack debe estar declarado.
 * @post El stack queda inicializado y marcado como vacío.
 */
void createEmptyStack(tStack *S);

/*
 * Objetivo: Inserta un elemento en la cima de la pila.
 * Entradas:
 *   - d: Item a insertar.
 *   - S: Puntero al stack.
 * Salidas:
 *   - True si la inserción fue exitosa, false de lo contrario.
 * Postcondiciones:
 *   - El tamaño del stack se incrementa por 1 al guardar ahora el nuevo item.
 */
/**
 * @brief Inserta un elemento en la cima de la pila.
 * @param[in] d Item a insertar.
 * @param[in,out] S Puntero al stack.
 * @return true si la inserción fue exitosa, false de lo contrario.
 * @post El tamaño del stack se incrementa por 1 al guardar ahora el nuevo item.
 */
bool push(tItemS d, tStack *S);

/*
 * Objetivo: Elimina el elemento de la cima del stack.
 * Entradas:
 *   - S: Puntero al stack.
 * Precondiciones:
 *   - El stack no puede estar vacío.
 */
/**
 * @brief Elimina el elemento de la cima del stack.
 * @param[in,out] S Puntero al stack.
 * @pre El stack no puede estar vacío.
 */
void pop(tStack *S);

/*
 * Objetivo: Devuelve el elemento de la cima de la pila.
 * Entradas:
 *   - S: Variable de tipo tStack.
 * Salidas:
 *   - El elemento tItemS de la cima del stack.
 * Precondiciones:
 *   - El stack no puede estar vacío.
 */
/**
 * @brief Devuelve el elemento de la cima de la pila.
 * @param[in] S Variable de tipo tStack.
 * @return El elemento tItemS de la cima del stack.
 * @pre El stack no puede estar vacío.
 */
tItemS peek(tStack S);

/*
 * Objetivo: Determina si una pila está vacía.
 * Entradas:
 *   - S: Variable de tipo tStack.
 * Salidas:
 *   - true si el stack está vacío, false de lo contrario.
 */
/**
 * @brief Comprueba si un stack está vacío.
 * @param[in] S Variable de tipo tStack.
 * @return true si el stack está vacío, false de lo contrario.
 */
bool isEmptyStack(tStack S);

#endif // BID_STACK_H