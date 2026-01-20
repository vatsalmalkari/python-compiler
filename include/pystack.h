#ifndef PYSTACK_H
#define PYSTACK_H

#include "pyobject.h"
#include "list.h"

typedef struct {
    pylist* list;  
} pystack;

pystack* stack_new(void);
void stack_del(pystack* s);

void stack_push(pystack* s, PyObject* obj);
PyObject* stack_pop(pystack* s);
PyObject* stack_peek(pystack* s);
int stack_len(pystack* s);

#endif
