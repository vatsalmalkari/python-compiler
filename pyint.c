#include <stdlib.h>
#include "pyint.h"

PyObject* pyint_new(int value) {
    PyObject* obj = malloc(sizeof(PyObject));
    obj->type = PY_INT;
    obj->refcount = 1;

    int* v = malloc(sizeof(int));
    *v = value;
    obj->value = v;

    return obj;
}

int pyint_value(PyObject* obj) {
    return *(int*)obj->value;
}
