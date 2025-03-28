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

/* AVISO: Esta implementación sigue la exigencia de la asignatura de no realizar comprobaciones de precondición.
 * En un entorno real, validar los parámetros de entrada sería esencial para evitar comportamientos indefinidos,
 * prevenir posibles corrupciones y mejorar la robustez del código. Confiar en que el usuario siempre cumplirá las
 * precondiciones no es una buena decisión y hace que el TAD sea susceptible a errores de programación o ataques
 * de inyección.
 */

void createEmptyList(tList *L) { //puntero a un puntero permite modificar el primero.
    *L = LNULL;
}

//Función auxiliar para facilitar la creación de nodos

/* Objetivo: asignar un espacio en la memoria para cada nodo.
 * Entrada: puntero a una posición.
 * Salida: true si hay espacio en memoria y un espacio en ella queda asignado al nodo, false en caso contrario.
 * Postcondición: si la función devuelve true, *p apunta a un nuevo nodo válido en memoria;
 * si devuelve false, *p sigue siendo LNULL y no se ha reservado memoria.
 */

bool allocateNode(tPosL *p){
    *p = malloc(sizeof(struct tNode));
    return *p != LNULL;
}

bool isEmptyList(tList L) {
    return L == LNULL;
}

bool insertItem(tItemL d, tList *L) {
    tPosL newNode, prev = LNULL, current = *L;
    bool out = false;

    if (allocateNode(&newNode)) { //comprobamos al principio para no cambiar nada si hay error en memoria

        newNode->data = d;
        newNode->next = LNULL; //inicializamos como null siempre

        //caso 1: nuevo nodo es el menor: lista vacía (*L) o insertando al inicio
        if (isEmptyList(*L) || strcmp(d.consoleId, (*L)->data.consoleId) < 0) {
            newNode->next = *L;
            *L = newNode;
        } else {
            //caso 2: buscar la posición correcta para mantener el orden
            while (current != LNULL && strcmp(d.consoleId, current->data.consoleId) > 0) {
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
    tPosL prev = *L;
    if (!isEmptyList(*L) && p != LNULL) {     /* Lista vacía o posición inválida. Mejora eficiencia salir de la función
                                               * en lugar de meternos en búcles con un valor que corromperá la lista
                                               */
        if (*L == p) { //eliminamos el primer nodo
            *L = (*L)->next;
            free(p);
        } else {
            while (prev->next != LNULL && prev->next != p) { //mejor no usar previous(), así no se duplica para atravesar la lista y podemos asegurar que delete nunca elimina una p invalida ni libera un nodo que no existe.
                prev = prev->next;
            }

            if (prev->next == p) { //aseguramos que el puntero encontrado es el que hemos pasado antes de liberarlo
                prev->next = p->next; //enlazamos el anterior con el posterior

                free(p);
            }
        }
    }
}

tItemL getItem(tPosL p, tList L) { //p es el puntero al struct que contiene la info, tlist no es necesario
    return p->data;
}

void updateItem(tItemL d, tPosL p, tList *L) {
    p->data = d;
}

tPosL findItem(tConsoleId id, tList L) {//l sigue siendo puntero. Como una lista vacía tiene L==LNULL, la fc también acepta listas vacías, ya que devolverá null tras el primer intento
    tPosL p;
    for (p = L; p != LNULL && strcmp(p->data.consoleId, id) <= 0; p = p->next) { //atravesamos hasta encontrar un match, o consoleId es mayor que el pasado. en el peor de los casos la fc es O(n)
        if (strcmp(p->data.consoleId, id) == 0) return p;
    }
    return LNULL;
}

tPosL first(tList L) {
    return L;
}

tPosL last(tList L) {
    while (L->next != LNULL) { //atravesamos hasta el final
        L = L->next;
    }
    return L;
}

tPosL previous(tPosL p, tList L) {
    tPosL out = LNULL;
    if (p != L) { //si la p==L, es el primer nodo y no tiene previo
        tList prev = L;
        while (prev->next != LNULL && prev->next != p) { //atravesamos hasta encontrar un puntero cuyo siguiente sea la posición enviada, o el final.
            prev = prev->next;
        }
        out = prev->next == p ? prev : LNULL; //si el que hemos encontrado es el previo, lo devolvemos, si no devolvemos lnull.
    }
    return out;
}

tPosL next(tPosL p, tList L) {
    return p->next;
}
