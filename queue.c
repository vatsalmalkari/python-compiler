#include "include/pyqueue.h"
#include <stdlib.h>


pyqueue* queue_new() {
    pyqueue* q = malloc(sizeof(pyqueue));
    q->list = pylist_new();
    return q;
}

void queue_del(pyqueue* q) {
    if (!q) return;
    pylist_del(q->list);
    free(q);
}

void queue_enqueue(pyqueue* q, PyObject* obj) {
    pylist_append(q->list, obj);
}

PyObject* queue_dequeue(pyqueue* q) {
    if (!q || pylist_len(q->list) == 0) return py_none();
    PyObject* obj = pylist_get(q->list, 0);

    for (int i = 1; i < pylist_len(q->list); i++)
        q->list->items[i - 1] = q->list->items[i];

    q->list->size--;
    return obj;
}

PyObject* queue_peek(pyqueue* q) {
    if (!q || pylist_len(q->list) == 0) return py_none();
    return pylist_get(q->list, 0);
}

int queue_len(pyqueue* q) {
    return pylist_len(q->list);
}
