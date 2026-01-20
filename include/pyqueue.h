#ifndef PYQUEUE_H
#define PYQUEUE_H

#include "pyobject.h"
#include "list.h"

typedef struct {
    pylist* list;
} pyqueue;

pyqueue* queue_new(void);
void queue_del(pyqueue* q);

void queue_enqueue(pyqueue* q, PyObject* obj);
PyObject* queue_dequeue(pyqueue* q);
PyObject* queue_peek(pyqueue* q);
int queue_len(pyqueue* q);

#endif
