#include "include/list.h"
#include <stdlib.h>
#include <stdio.h>

pylist* pylist_new() {
    pylist* l = malloc(sizeof(pylist));
    l->size = 0;
    l->capacity = 4;
    l->items = malloc(sizeof(PyObject*) * l->capacity);
    return l;
}

void pylist_del(pylist* l) {
    if (!l) return;

    for (int i = 0; i < l->size; i++) {
        py_decref(l->items[i]);
    }

    free(l->items);
    free(l);
}

void pylist_append(pylist* l, PyObject* obj) {
    if (!l || !obj) return;

    if (l->size >= l->capacity) {
        l->capacity *= 2;
        l->items = realloc(l->items, sizeof(PyObject*) * l->capacity);
    }

    py_incref(obj);
    l->items[l->size++] = obj;
}

PyObject* pylist_get(pylist* l, int index) {
    if (!l || index < 0 || index >= l->size)
        return py_none();
    return l->items[index];
}

void pylist_set(pylist* l, int index, PyObject* obj) {
    if (!l || index < 0 || index >= l->size || !obj) return;

    py_decref(l->items[index]);
    py_incref(obj);
    l->items[index] = obj;
}

int pylist_len(pylist* l) {
    return l ? l->size : 0;
}

void pylist_print(pylist* l) {
    if (!l) return;

    printf("[");
    for (int i = 0; i < l->size; i++) {
        if (i > 0) printf(", ");
        pyobject_print(l->items[i]);
    }
    printf("]\n");
}
