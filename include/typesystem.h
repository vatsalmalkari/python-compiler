// typesystem.h
#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H
#include "pystr.h"
#include "list.h"
#include "pydict.h"
#include "pyobject.h"

#define MAX_VARS 26
#define MAX_LINE 200
#define MAX_TOKENS 50
#define MAX_LIST_VARS 10
#define MAX_DICT_VARS 10
#define MAX_MAP_VARS 10
#define MAX_STR_VARS 26

typedef struct {
    char name;
    PyType type;
    int value;
    pystr* str_value;
    pylist* list_value;
    pydict* dict_value;
} TypedVariable;

PyType infer_type(char* value);
int is_type_compatible(PyType expected, PyType actual);
const char* type_name(PyType type);
void print_type_error(char name, PyType expected, PyType actual, int line);

TypedVariable* find_typed_var(char name);

#endif