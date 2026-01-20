#ifndef PYOBJECT_H
#define PYOBJECT_H

typedef enum {
    PY_INT,
    PY_STRING,
    PY_NONE,
    PY_STR
} PyType;

typedef struct PyObject {
    PyType type;
    int refcount;
    void* value;  
} PyObject;

void py_incref(PyObject* obj);
void py_decref(PyObject* obj);


PyObject* py_none(void);

void pyobject_print(PyObject* obj);

#define PY_INT  1
#define PY_NONE 2
#define PY_STR  3
#define PY_STRING 4

#endif
