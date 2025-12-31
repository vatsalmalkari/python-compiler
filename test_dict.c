#include <stdio.h>
#include "pydict.h"

int main() {
    printf("Dict Tests\n");

    pydict* D = pydict_new();
    pydict_set(D, "a", 10);
    pydict_set(D, "b", 20);
    printf("Dict dump: ");
    pydict_dump(D);

    printf("Get 'a': %d\n", pydict_get(D, "a"));
    printf("Get 'c' (default 0): %d\n", pydict_get(D, "c"));

    pydict_set(D, "a", 42);
    printf("After updating 'a': ");
    pydict_dump(D);

    printf("Dict size: %d\n", pydict_len(D));

    pydict_del(D);
    return 0;
}
