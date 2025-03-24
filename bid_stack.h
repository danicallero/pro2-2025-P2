/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: ***************************** LOGIN 1: **********
 * AUTHOR 2: ***************************** LOGIN 2: **********
 * GROUP: *.*
 * DATE: ** / ** / **
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

void createEmptyStack(tStack *S);
bool push(tItemS d, tStack *S);
void pop(tStack *S);
tItemS peek(tStack S);
bool isEmptyStack(tStack S);

#endif
