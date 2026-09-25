#include "typesystem.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "include/typesystem.h"
#include "include/interpreter.h"

// find variable by name


PyType infer_type(char* value) {
    if (!value || strlen(value) == 0) return PY_NONE;
    
    // Check if it's a string (quoted)
    if ((value[0] == '"' || value[0] == '\'')) return PY_STR;
    
    // Check if it's a list (starts with [)
    if (value[0] == '[') return PY_LIST;
    
    // Check if it's a dict (starts with {)
    if (value[0] == '{') return PY_DICT;
    
    // Check if it's an integer (digits or negative)
    if (isdigit(value[0]) || (value[0] == '-' && isdigit(value[1]))) {
        return PY_INT;
    }
    // Check if it's a variable name
    if (isalpha(value[0])) {
        // Look up variable and return its type
        TypedVariable* v = find_var(value[0]);
        if (v) {
            return v->type;
        }
        return PY_NONE;
    }
    return PY_NONE;
}

int is_type_compatible(PyType expected, PyType actual) {
    if (expected == actual) return 1;
    if (expected == PY_NONE) return 1;  // NONE accepts anything
    return 0;
}

const char* type_name(PyType type) {
    switch (type) {
        case PY_INT: return "int";
        case PY_STR: return "str";
        case PY_LIST: return "list";
        case PY_DICT: return "dict";
        case PY_NONE: return "None";
        default: return "unknown";
    }
}

void print_type_error(char name, PyType expected, PyType actual, int line) {
printf("Type error: Variable '%c' expected type '%s' but got '%s' at line %d\n", 
     name, type_name(expected), type_name(actual), line);
}