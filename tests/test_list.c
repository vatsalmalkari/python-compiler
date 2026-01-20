#include <stdio.h>
#include "list.h"
#include "pyobject.h"
#include "pyint.h"

int main() {
    printf(" list Tests \n");

    pylist* L = pylist_new();

    printf("List before append: ");
    pylist_print(L);

    pylist_append(L, pyint_new(1));
    pylist_append(L, pyint_new(2));
    pylist_append(L, pyint_new(3));

    printf("List after appending 1 2 3: ");
    pylist_print(L);

    printf("Length: %d\n", pylist_len(L));

    pylist_set(L, 1, pyint_new(42));
    printf("After set index 1 to 42: ");
    pylist_print(L);

    PyObject* elem = pylist_get(L, 2);
    printf("Element at index 2: "); pyobject_print(elem); printf("\n");

    pylist_append(L, pyint_new(99));
    printf("After appending 99: ");
    pylist_print(L);

    pylist_del(L);
    return 0;
}
