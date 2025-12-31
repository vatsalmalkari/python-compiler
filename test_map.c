#include <stdio.h>
#include "map.h"

int main() {
    printf("Map Tests \n");

    struct Map* M = Map_new();
    M->put(M, "x", 100);
    M->put(M, "y", 200);
    M->dump(M);

    printf("Get x: %d\n", M->get(M, "x", 0));
    printf("Get z (default 0): %d\n", M->get(M, "z", 0));

    M->put(M, "x", 500);
    printf("After updating x: ");
    M->dump(M);

    printf("Map size: %d\n", map_len(M));

    M->del(M);
    return 0;
}
