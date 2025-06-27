#include <stdio.h>
#include <string.h>

#include "bid_stack.h"

void printTopBid(const tStack s) {
    if (isEmptyStack(s)) {
        printf("Stack: empty\n");
    } else {
        tItemS top = peek(s);
        printf("Stack: top -> %s (%.2f)\n", top.bidder, top.consolePrice);
    }
}

int main() {
    tStack s;
    tItemS bid;

    createEmptyStack(&s);
    printTopBid(s);

    // Push several bids
    const char *names[] = {"luis", "marta", "erik"};
    const float prices[] = {12, 25, 33};
    printf("Populating stack...\n");
    for (int i = 0; i < 3; i++) {
        strcpy(bid.bidder, names[i]);
        bid.consolePrice = prices[i];
        push(bid, &s);
        printTopBid(s);
    }

    // Pop one
    pop(&s);
    printf("\nPopping: %s\n", bid.bidder);
    printTopBid(s);

    // Push again
    strcpy(bid.bidder, "sofia");
    bid.consolePrice = 41;
    push(bid, &s);
    printf("Pushing: %s\n", bid.bidder);
    printTopBid(s);

    // Clear stack completely
    while (!isEmptyStack(s)) {
        pop(&s);
        printTopBid(s);
    }

    // Push and pop one
    strcpy(bid.bidder, "alex");
    bid.consolePrice = 55;
    printf("Pushing: %s\n", bid.bidder);
    push(bid, &s);
    printTopBid(s);
    printf("Popping: %s\n", bid.bidder);
    pop(&s);
    printTopBid(s);

    return 0;
}