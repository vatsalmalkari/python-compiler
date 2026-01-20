#ifndef PYSTR_H
#define PYSTR_H

#include "pyobject.h"

typedef struct pystr {
    PyObject base;
    char* data;
    int length;
    int capacity;
} pystr;

PyObject* pystr_new_from(const char* s);
void pystr_print(PyObject* obj);
int pystr_len(PyObject* obj);
void pystr_append_char(PyObject* obj, char c);
void pystr_appends(PyObject* obj, const char* extra);
void pystr_assign(PyObject* obj, const char* newval);
void pystr_del(PyObject* obj);

#endif
