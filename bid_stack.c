/**
 * @file bid_stack.c
 * @brief Gestion del TAD pila estática de pujas.
 * @date 25/03/2025
 * @author Daniel Callero Costales hola@danicallero.es
 *
 * @note Proyecto compartido con fines educativos. Se desaconseja la entrega propia o con fines de plagio.
 */

#include "bid_stack.h"
#include <stdbool.h>


void createEmptyStack(tStack *S){
    S->top = SNULL; //La pila se marca como vacía utilizando un valor fijo LNULL.
}

bool isEmptyStack(tStack S){
    return S.top == SNULL;  //Una pila está vacía cuando su tamaño es el valor fijo LNULL.
}

bool push(tItemS d, tStack *S){
    bool out = false;       //Auxiliar donde se recoge el output que devolverá return.

    if(S->top >= SMAX-1)    //Solo se inserta si la lista no está llena.
        out = false;
    else {
        S->top++;           //Se amplía el número de datos útiles registrados.
        S->data[S->top] = d;//Se añade el elemento a la cima (LIFO).
        out = true;
    }
    return out;
}

void pop(tStack *S){
    S->top--; //Se reduce por 1 el número de valores útiles recogidos en la pila.
}

tItemS peek(tStack S){
    return S.data[S.top]; //Se devuelve el valor que ocupa la posición de la cima en la pila.
}
