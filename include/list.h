#ifndef LIST_H
#define LIST_H

#include "pyobject.h"

typedef struct {
    PyObject** items;
    int size;
    int capacity;
} pylist;

pylist* pylist_new(void);
void pylist_del(pylist* l);

void pylist_append(pylist* l, PyObject* obj);
PyObject* pylist_get(pylist* l, int index);
void pylist_set(pylist* l, int index, PyObject* obj);

int pylist_len(pylist* l);
void pylist_print(pylist* l);

#endif
