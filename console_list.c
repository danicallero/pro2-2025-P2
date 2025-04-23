/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: Yago Regueiro Piera LOGIN 2: y.regueiro.piera@udc.es
 * GROUP: 3.3
 * DATE: 25 / 03 / 25
 */

#include "console_list.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void createEmptyList(tList *L) {
    *L = LNULL; //Se marca como vacía utilizando un valor fijo LNULL.
}

//Función auxiliar para aislar la reserva de memoria para nodos.
/* Objetivo: asignar un espacio en la memoria para un nodo.
 * Entradas:
 *   - *p: puntero a una posición.
 * Salida:
 *    - Devuelve true si hay espacio en memoria y un espacio en ella queda asignado al nodo, false en caso contrario.
 * Postcondiciónes:
 *    - El puntero '*p' apunta a un nuevo nodo con espacio reservado en memoria si la función ha devuelto true.
 */
bool allocateNode(tPosL *p){
    *p = malloc(sizeof(struct tNode));  //Se reserva un espacio en memoria del tamaño del nodo.
    return *p != LNULL;
}

bool isEmptyList(const tList L) {
    return L == LNULL;  //Las listas vacías se marcan con L=LNULL en createEmptyList.
}

bool insertItem(const tItemL data_d, tList *L) {
    tPosL newNode;      //El nodo que se inserta en la lista.
    tPosL prev = LNULL; //Auxiliar que mantendrá registro del nodo anterior a current.
    tPosL current = *L; //Auxiliar donde se guarda el nodo que se comprueba en la iteración actual.
    bool out = false;   //Auxiliar donde se recoge el output que devolverá return.

    if (allocateNode(&newNode)) {//insertItem se ejecuta solo si se ha asignado espacio en memoria correctamente.

        newNode->data = data_d;
        newNode->next = LNULL;  //Se inicializa como nulo.

        //Caso 1: nuevo nodo es el menor: lista vacía (*L) o insertando al inicio.
        if (isEmptyList(*L) || strcmp(data_d.consoleId, (*L)->data.consoleId) < 0) {
            newNode->next = *L;
            *L = newNode;
        } else {
            //Caso 2: buscar la posición correcta para mantener el orden.
            while (current != LNULL && strcmp(data_d.consoleId, current->data.consoleId) > 0) {
                prev = current;
                current = current->next;
            }
            if (prev != LNULL) {
                prev->next = newNode;
            }
            newNode->next = current;
        }
        out = true;
    }
    return out;
}

void deleteAtPosition(tPosL p, tList *L) {
    if (!isEmptyList(*L) && p != LNULL) {
        if (p == *L) {
            // Caso 1: Eliminando el primer nodo.
            *L = (*L)->next;
            free(p);
        } else if (p->next == LNULL) {
            // Caso 2: Eliminando el último nodo.
            tPosL pos = *L;
            while (pos->next != LNULL && pos->next != p) {
                pos = pos->next;
            }
            if (pos->next == p) {
                pos->next = LNULL;
                free(p);
            }
        } else {
            // Caso 3: Eliminando un nodo intermedio.
            tPosL q = p->next;
            p->data = q->data;
            p->next = q->next;
            free(q);
        }
    }
}

tItemL getItem(tPosL p, tList L) {  /*'p' es el puntero al struct que contiene la info, tlist no es necesario en la
                                     *implementación dinámica, pero añadirlo asegura uniformidad entre implementaciones.
                                     */
    return p->data;
}

void updateItem(tItemL d, tPosL p, tList *L) {  /*Al igual que en getItem, tList no es necesario, pero añadirlo garantiza
                                                 *uniformidad entre implementaciones.
                                                 */
    p->data = d;
}

tPosL findItem(tConsoleId id, tList L) {
    tPosL p; //Auxiliar donde se guarda el nodo que se comprueba en la iteración actual.

    for (p = L; p != LNULL && strcmp(p->data.consoleId, id) <= 0; p = p->next) { /*Se atraviesa hasta encontrar una coincidencia,
                                                                                  *o consoleId es mayor que el del parámetro pasado.
                                                                                  *En el peor de los casos la función es O(n).
                                                                                  */
        if (strcmp(p->data.consoleId, id) == 0) return p;
    }
    return LNULL;
}

tPosL first(tList L) {
    return L;
}

tPosL last(tList L) {
    while (L->next != LNULL) { //Se atraviesa hasta el final.
        L = L->next;
    }
    return L;
}

tPosL previous(tPosL p, tList L) {
    tPosL out = LNULL; //Auxiliar donde se recoge el output que devolverá return. LNULL por defecto.
    tList prev; //Auxiliar que mantendrá registro del nodo anterior a current.

    if (p != L) { //Si 'p' es igual que 'L', 'p' es el nodo del primer elemento y no tiene previo ('L' es el primer nodo de la lista por definición).
        prev = L; //Auxiliar que mantendrá registro del nodo anterior a current.
        while (prev->next != LNULL && prev->next != p) { //Se atraviesa hasta encontrar un nodo cuyo siguiente sea el enviado, o el final.
            prev = prev->next;
        }
        out = prev->next == p ? prev : LNULL; //Si siguiente al que se ha encontrado es 'p', 'prev' es el que se busca.
    }
    return out;
}

tPosL next(tPosL p, tList L) {  /*Al igual que en getItem, tList no es necesario, pero añadirlo garantiza
                                 *uniformidad entre implementaciones.
                                 */
    return p->next;
}
