#include <stdio.h>
#include "map.h"
#include "pydict.h"
#include "pyint.h"
#include "pystr.h"
#include "pyobject.h"
#include <stdlib.h>
#include <string.h>
#include "strings.h"

int main() {
    printf(" MAP Tests \n");

    struct Map* M = Map_new();

    M->put(M, "x", pyint_new(100));
    M->put(M, "y", pyint_new(200));
    printf("After inserts: ");
    M->dump(M);

    PyObject* x_val = M->get(M, "x", py_none());
    PyObject* z_val = M->get(M, "z", py_none()); 
    printf("Get x: "); pyobject_print(x_val); printf("\n");
    printf("Get z (default None): "); pyobject_print(z_val); printf("\n");

    M->put(M, "x", pyint_new(500));
    printf("After updating x: ");
    M->dump(M);

    printf("Map size: %d\n", map_len(M));

    M->del(M);

    printf("\n PYDICT Tests \n");

    pydict* D = pydict_new();

    pydict_set(D, "a", pyint_new(10));
    pydict_set(D, "b", pyint_new(20));
    printf("After inserts: ");
    pydict_dump(D);

    PyObject* a_val = pydict_get(D, "a");
    PyObject* c_val = pydict_get(D, "c");
    printf("Get 'a': "); pyobject_print(a_val); printf("\n");
    printf("Get 'c' (default None): "); pyobject_print(c_val); printf("\n");

    pydict_set(D, "a", pyint_new(42));
    printf("After updating 'a': ");
    pydict_dump(D);

    printf("Dict size: %d\n", pydict_len(D));

    pydict_del(D);

    return 0;
}
