/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * GROUP: 3.3
 * DATE: 25 / 03 / 25
 */

#ifndef CONSOLE_LIST_H
#define CONSOLE_LIST_H

#include "types.h"
#include "bid_stack.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * @file console_list.h
 * @brief Gestión del TAD Lista dinámica ordenada de consolas.
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
 *@brief Valor fijo para una posición de la tList nula.
 */
#define LNULL NULL

/**
 * @brief Estructura que representa un ítem (elemento) de la lista tList.
 * Contiene toda la información característica de una consola.
 */
typedef struct tItemL {
 tUserId seller; /**< ID del usuario que vende la consola.*/
 tConsoleId consoleId; /**< ID de la consola.*/
 tConsoleBrand consoleBrand; /**< Marca de la consola.*/
 tConsolePrice consolePrice; /**< Precio original de la consola*/
 tBidCounter bidCounter; /**< Número de pujas que se han realizado sobre la consola*/
 tStack bidStack; /**< Stack que guarda las pujas realizadas sobre la consola*/
} tItemL;

/**
 * @brief tPosL se define como un puntero a un nodo tNode.
 */
typedef struct tNode *tPosL; //Se define tPosL como puntero a tNode.

/**
 * @brief Estructura que representa un nodo de la lista dinámica.
 * Contiene un tItemL y un enlace al nodo siguiente 'next'.
 */
struct tNode{
  tItemL data; /**< Struct que guarda la información de una consola.*/
  tPosL next; /**< Enlace al siguiente nodo. Un valor next = LNULL determina el final de la lista.*/
};

/**
 * @brief tList se define como un tipo equivalente a tNode, por ello, tList será el primer nodo de la lista enlazada.
 */
typedef tPosL tList;

/**
 * @brief Crea una lista vacía.
 *
 * @param[out] L Puntero a la lista a inicializar.
 *
 * @pre La lista debe estar declarada.
 * @post La lista queda inicializada y se marca como vacía.
 */
void createEmptyList(tList *L);

/**
 * @brief Comprueba si una lista está vacía.
 *
 * @param[in] L Lista a comprobar.
 *
 * @return true si la lista está vacía, false en caso contrario.
 */
bool isEmptyList(tList L);

/**
 * @brief Inserta un elemento en la lista manteniendo el orden.
 *
 * @param[in] data_d Elemento a insertar.
 * @param[in,out] L Puntero a la lista.
 *
 * @return true si se insertó correctamente, false en caso contrario.
 *
 * @post La lista aumenta de tamaño y el elemento se coloca en su posición ordenada. Los elementos posteriores pueden
 * haberse desplazado.
 */
bool insertItem(tItemL data_d, tList *L);

/**
 * @brief Elimina un elemento de la posición indicada en la lista.
 *
 * @param[in] p Posición del elemento a eliminar.
 * @param[in,out] L Puntero a la lista.
 *
 * @pre La posición 'p' debe ser válida y el elemento debe tener una pila de pujas vacía.
 * @post La lista disminuye de tamaño y los elementos posteriores a 'p' pueden haberse desplazado.
 */
void deleteAtPosition(tPosL p, tList *L);

/**
 * @brief Modifica el contenido de un elemento en la posición indicada.
 *
 * @param[in] d Nuevo valor del elemento.
 * @param[in] p Posición del elemento a modificar.
 * @param[in,out] L Puntero a la lista.
 *
 * @pre La posición 'p' debe ser válida.
 * @post El contenido del elemento se actualiza, el orden de la lista se mantiene.
 */
void updateItem(tItemL d, tPosL p, tList *L);

/**
 * @brief Obtiene el elemento de una lista en la posición dada.
 *
 * @param[in] p Posición del elemento.
 * @param[in] L Lista desde la que se obtiene el elemento.
 *
 * @return El elemento en la posición p.
 *
 * @pre La posición 'p' debe ser válida.
 */
tItemL getItem(tPosL p, tList L);

/**
 * @brief Devuelve la posición del primer elemento de la lista.
 *
 * @param[in] L Lista a evaluar.
 *
 * @return Posición del primer elemento.
 *
 * @pre La lista no debe estar vacía.
 * @remark Por la naturaleza de la implementación dinámica, devuelve LNULL si la lista está vacía e inicializada.
 */
tPosL first(tList L);

/**
 * @brief Devuelve la posición del último elemento de la lista.
 *
 * @param[in] L Lista a evaluar.
 *
 * @return Posición del último elemento.
 *
 * @pre La lista no debe estar vacía.
 * @remark Por la naturaleza de la implementación dinámica, devuelve LNULL si la lista está vacía e inicializada.
 */
tPosL last(tList L);

/**
 * @brief Devuelve la posición del nodo anterior al proporcionado.
 *
 * @param[in] p Posición actual.
 * @param[in] L Lista donde buscar.
 *
 * @return Posición anterior o LNULL si 'p' es el primero.
 *
 * @pre La posición 'p' debe ser válida.
 */
tPosL previous(tPosL p, tList L);

/**
 * @brief Devuelve la posición del nodo siguiente al proporcionado.
 *
 * @param[in] p Posición actual.
 * @param[in] L Lista donde buscar.
 *
 * @return Posición siguiente o LNULL si 'p' es el último.
 *
 * @pre La posición 'p' debe ser válida.
 */
tPosL next(tPosL p, tList L);

/**
 * @brief Busca un elemento con el identificador dado en la lista.
 *
 * @param[in] id Identificador de la consola.
 * @param[in] L Lista donde buscar.
 *
 * @return Posición del primer elemento con ese ID o LNULL si no se encuentra.
 */
tPosL findItem(tConsoleId id, tList L);
#endif