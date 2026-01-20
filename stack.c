#include "include/pystack.h"
#include <stdlib.h>

pystack* stack_new() {
    pystack* s = malloc(sizeof(pystack));
    s->list = pylist_new();
    return s;
}

void stack_del(pystack* s) {
    if (!s) return;
    pylist_del(s->list);
    free(s);
}

void stack_push(pystack* s, PyObject* obj) {
    pylist_append(s->list, obj);
}

PyObject* stack_pop(pystack* s) {
    if (!s || pylist_len(s->list) == 0) return py_none();
    PyObject* obj = pylist_get(s->list, pylist_len(s->list) - 1);
    s->list->size--;
    return obj;
}

PyObject* stack_peek(pystack* s) {
    if (!s || pylist_len(s->list) == 0) return py_none();
    return pylist_get(s->list, pylist_len(s->list) - 1);
}

int stack_len(pystack* s) {
    return pylist_len(s->list);
}
