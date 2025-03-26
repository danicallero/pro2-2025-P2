/*
* TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Callero Costales LOGIN 1: daniel.callero@udc.es
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: 3.3
 * DATE: 25 / 03 / 25
 */

#include "bid_stack.h"
#include <stdbool.h>

/* Write your code here... */
void createEmptyStack(tStack *S){
    S->top = SNULL;
}

bool isEmptyStack(tStack S){
    return S.top == SNULL;
}

bool push(tItemS d, tStack *S){
    bool out = false;
    if(S->top == SMAX-1)
        out = false;
    else {
        S->top++;
        S->data[S->top] = d;
        out = true;
    }
    return out;
}

void pop(tStack *S){
    S->top--;
}

tItemS peek(tStack S){
    return S.data[S.top];
}
