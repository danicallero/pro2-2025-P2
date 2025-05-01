/*
* TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
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