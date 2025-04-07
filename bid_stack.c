/*
* TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: Yago Regueiro Piera LOGIN 2: y.regueiro.piera@udc.es
 * GROUP: 3.3
 * DATE: 25 / 03 / 25
 */

#include "bid_stack.h"
#include <stdbool.h>

/* Write your code here... */
void createEmptyStack(tStack *S){
    S->top = SNULL; //cuando vacía top = null
}

bool isEmptyStack(tStack S){
    return S.top == SNULL; //cuando vacía top = null
}

bool push(tItemS d, tStack *S){
    bool out = false; //output
    if(S->top >= SMAX-1) //está la lista llena?
        out = false;
    else {
        S->top++; //un valor más en la lista
        S->data[S->top] = d; //añadimos a la cima (LIFO)
        out = true;
    }
    return out;
}

void pop(tStack *S){
    S->top--; //top es uno menos
}

tItemS peek(tStack S){
    return S.data[S.top]; //devolvemos top
}
