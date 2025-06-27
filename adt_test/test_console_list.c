#include <stdio.h>
#include <string.h>

#include "console_list.h"

void printList(const tList list) {
    printf("(");
    for (tPosL pos = first(list); pos != LNULL; pos = next(pos, list)) {
        tItemL item = getItem(pos, list);
        printf(" %s price %.2f", item.consoleId, item.consolePrice);
        if (next(pos, list) != LNULL) {
            printf(",");
        }
    }
    printf(" )\n");
}

int main() {
    tList list;
    tItemL item;
    tPosL pos;

    // Inicializar lista e ítem base
    createEmptyList(&list);
    item.consoleBrand = sega;
    strcpy(item.seller, "user1");
    item.bidCounter = 0;
    printList(list);

    // Datos de consolas en orden desordenado
    struct {
        const char *id;
        float price;
    } consoles[] = {
        {"c4", 40}, {"c1", 10}, {"c5", 50},
        {"c3", 30}, {"c2", 20}, {"c6", 60}
    };

    // Inserción de consolas
    int n = sizeof(consoles) / sizeof(consoles[0]);
    for (int i = 0; i < n; ++i) {
        strcpy(item.consoleId, consoles[i].id);
        item.consolePrice = consoles[i].price;
        insertItem(item, &list);
        printList(list);
    }

    // Búsquedas
    pos = findItem("unknown", list);
    if (pos == LNULL) printf("unknown Not found!\n");

    char *idsToFind[] = {"c1", "c5", "c3"};
    for (int i = 0; i < 3; ++i) {
        pos = findItem(idsToFind[i], list);
        if (pos != LNULL) {
            item = getItem(pos, list);
            printf("%s %.2f\n", item.consoleId, item.consolePrice);
        }
    }

    // Actualización
    pos = findItem("c3", list);
    if (pos != LNULL) {
        item = getItem(pos, list);
        item.consolePrice = 100;
        updateItem(item, pos, &list);
        item = getItem(pos, list);
        printf("%s %.2f\n", item.consoleId, item.consolePrice);
        printList(list);
    }

    // Eliminaciones
    deleteAtPosition(next(first(list), list), &list);         // remove c2
    printList(list);

    deleteAtPosition(previous(last(list), list), &list);      // remove c4
    printList(list);

    deleteAtPosition(first(list), &list);                     // remove c1
    printList(list);

    deleteAtPosition(last(list), &list);                      // remove c5
    printList(list);

    deleteAtPosition(first(list), &list);                     // remove c3
    printList(list);

    // Reinserción final
    insertItem(item, &list);  // c6
    printList(list);

    return 0;
}