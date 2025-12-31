#include <stdio.h>
#include "list.h"

int main() {
    printf("List Tests \n");

    pylist* L = pylist_new();
    pylist_append(L, 1);
    pylist_append(L, 2);
    pylist_append(L, 3);

    printf("List after append: ");
    pylist_print(L);

    printf("Length: %d\n", pylist_len(L));

    pylist_set(L, 1, 42);
    printf("After set index 1 to 42: ");
    pylist_print(L);

    printf("Element at 2: %d\n", pylist_get(L, 2));

    pylist_append(L, 99);
    printf("After appending 99: ");
    pylist_print(L);

    pylist_del(L);
    return 0;
}
