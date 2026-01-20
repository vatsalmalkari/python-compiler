#ifndef PYINT_H
#define PYINT_H
#include "pyobject.h"

PyObject* pyint_new(int value);
int pyint_value(PyObject* obj);

#endif
