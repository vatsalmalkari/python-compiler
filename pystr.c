#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pystr.h"
#include "pyobject.h"

PyObject* pystr_new_from(const char* s) {
    pystr* str_obj = malloc(sizeof(pystr));
    if (!str_obj) return NULL;

    str_obj->base.type = PY_STRING;
    str_obj->base.refcount = 1;

    str_obj->length = strlen(s);
    str_obj->capacity = str_obj->length + 1;
    str_obj->data = malloc(str_obj->capacity);
    if (!str_obj->data) { free(str_obj); return NULL; }

    strcpy(str_obj->data, s);
    return (PyObject*)str_obj;
}

void pystr_del(PyObject* obj) {
    if (!obj) return;
    pystr* s = (pystr*)obj;
    free(s->data);
    free(s);
}

void pystr_append_char(PyObject* obj, char c) {
    pystr* s = (pystr*)obj;
    if (s->length + 1 >= s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity);
    }
    s->data[s->length++] = c;
    s->data[s->length] = '\0';
}

void pystr_appends(PyObject* obj, const char* extra) {
    pystr* s = (pystr*)obj;
    int extra_len = strlen(extra);
    while (s->length + extra_len >= s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity);
    }
    strcat(s->data, extra);
    s->length += extra_len;
}

void pystr_assign(PyObject* obj, const char* newval) {
    pystr* s = (pystr*)obj;
    int new_len = strlen(newval);
    if (new_len >= s->capacity) {
        s->capacity = new_len + 1;
        s->data = realloc(s->data, s->capacity);
    }
    strcpy(s->data, newval);
    s->length = new_len;
}

void pystr_print(PyObject* obj) {
    if (!obj) { printf("None\n"); return; }
    pystr* s = (pystr*)obj;
    printf("%s\n", s->data);
}

int pystr_len(PyObject* obj) {
    if (!obj) return 0;
    pystr* s = (pystr*)obj;
    return s->length;
}
