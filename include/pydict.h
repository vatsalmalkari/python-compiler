#ifndef PYDICT_H
#define PYDICT_H

#include "pyobject.h"

typedef struct {
    char* key;
    PyObject* value;
} dict_entry;

typedef struct {
    dict_entry* items;
    int size;
    int capacity;
} pydict;

pydict* pydict_new(void);
void pydict_del(pydict* d);

void pydict_set(pydict* d, const char* key, PyObject* val);
PyObject* pydict_get(pydict* d, const char* key);
int pydict_len(pydict* d);
void pydict_print(pydict* d);
void pydict_dump(pydict* d);

#endif
    