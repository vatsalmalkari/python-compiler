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
PyObject* pystr_concat(PyObject* obj1, PyObject* obj2);
int pystr_find(PyObject* obj, const char* sub);
PyObject* pystr_replace(PyObject* obj, const char* old_sub, const char* new_sub);
char pystr_get_char(PyObject* obj, int index);
PyObject* pystr_slice(PyObject* obj, int start, int end);

#endif
