#include <stdlib.h>
#include <stdio.h>
#include "pyobject.h"

void py_incref(PyObject* obj) {
    if (obj) obj->refcount++;
}

void py_decref(PyObject* obj) {
    if (!obj) return;
    obj->refcount--;
    if (obj->refcount == 0) {
        free(obj->value); 
        free(obj);
    }
}

PyObject* py_none(void) {
    static PyObject none = { PY_NONE, 1, NULL };
    return &none;
}

void pyobject_print(PyObject* obj) {
    if (!obj) {
        printf("None");
        return;
    }

    switch(obj->type) {
        case PY_INT:
            printf("%d", *(int*)obj->value);
            break;
        case PY_NONE:
            printf("None");
            break;
        case PY_STRING:
            printf("\"%s\"", (char*)obj->value);
            break;
        default:
            printf("<object>");
    }
}
