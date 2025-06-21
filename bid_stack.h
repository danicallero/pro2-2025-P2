/**
 * @file bid_stack.h
 * @brief Gestion del TAD pila estática de pujas.
 * @date 25/03/2025
 *
 * @note Proyecto compartido con fines educativos. Se desaconseja la entrega propia o con fines de plagio.
 */

#ifndef BID_STACK_H
#define BID_STACK_H

#include "types.h"
#include <stdbool.h>

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


/**
 * @brief Inicializa un stack vacío.
 * @param[out] S Puntero al stack a inicializar.
 * @pre El stack debe estar declarado.
 * @post El stack queda inicializado y marcado como vacío.
 */
void createEmptyStack(tStack *S);

/**
 * @brief Inserta un elemento en la cima de la pila.
 * @param[in] d Item a insertar.
 * @param[in,out] S Puntero al stack.
 * @return true si la inserción fue exitosa, false de lo contrario.
 * @post El tamaño del stack se incrementa por 1 al guardar ahora el nuevo item.
 */
bool push(tItemS d, tStack *S);

/**
 * @brief Elimina el elemento de la cima del stack.
 * @param[in,out] S Puntero al stack.
 * @pre El stack no puede estar vacío.
 */
void pop(tStack *S);

/**
 * @brief Devuelve el elemento de la cima de la pila.
 * @param[in] S Variable de tipo tStack.
 * @return El elemento tItemS de la cima del stack.
 * @pre El stack no puede estar vacío.
 */
tItemS peek(tStack S);

/**
 * @brief Comprueba si un stack está vacío.
 * @param[in] S Variable de tipo tStack.
 * @return true si el stack está vacío, false de lo contrario.
 */
bool isEmptyStack(tStack S);

#endif // BID_STACK_H