#ifndef PYOBJECT_H
#define PYOBJECT_H

typedef struct PyObject {
    int type;
    int refcount;
} PyObject;

#define PY_INT  1
#define PY_NONE 2
#define PY_STR  3

#endif
