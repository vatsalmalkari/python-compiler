#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pyobject.h"
#include "pystr.h"

PyObject* pystr_new_from(const char* s) {
    pystr* s_obj = malloc(sizeof(pystr));
    s_obj->base.type = PY_STR;
    s_obj->base.refcount = 1;

    s_obj->length = strlen(s);
    s_obj->capacity = s_obj->length + 1;
    s_obj->data = malloc(s_obj->capacity);
    strcpy(s_obj->data, s);

    return (PyObject*)s_obj;
}

void pystr_print(PyObject* obj) {
    if (!obj) { printf("None\n"); return; }
    pystr* s = (pystr*)obj;
    printf("%s\n", s->data);
}

int pystr_len(PyObject* obj) {
    if (!obj) return 0;
    return ((pystr*)obj)->length;
}

void pystr_append_char(PyObject* obj, char c) {
    pystr* s = (pystr*)obj;
    if (s->length + 1 >= s->capacity) {
        s->capacity *= 2;
        s->data = realloc(s->data, s->capacity);
    }
    s->data[s->length] = c;
    s->length++;
    s->data[s->length] = '\0';
}

void pystr_appends(PyObject* obj, const char* extra) {
    pystr* s = (pystr*)obj;
    int extra_len = strlen(extra);
    while (s->length + extra_len >= s->capacity)
        s->capacity *= 2;
    s->data = realloc(s->data, s->capacity);
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

void pystr_del(PyObject* obj) {
    if (!obj) return;
    pystr* s = (pystr*)obj;
    free(s->data);
    free(s);
}

PyObject* pystr_concat(PyObject* obj1, PyObject* obj2) {
    if (!obj1) obj1 = pystr_new_from("");
    if (!obj2) obj2 = pystr_new_from("");
    pystr* s1 = (pystr*)obj1;
    pystr* s2 = (pystr*)obj2;
    int new_len = s1->length + s2->length;
    pystr* result = malloc(sizeof(pystr));
    result->base.type = PY_STR;
    result->base.refcount = 1;
    result->length = new_len;
    result->capacity = new_len + 1;
    result->data = malloc(result->capacity);
    strcpy(result->data, s1->data);
    strcat(result->data, s2->data);
    return (PyObject*)result;
};

char pystr_get_char(PyObject* obj, int index) {
    if (!obj) return '\0';
    pystr* s = (pystr*)obj;
    if (index < 0 || index >= s->length) {
        fprintf(stderr, "Error: index out of bounds\n");
        return '\0';
    }
    return s->data[index];
}

PyObject* pystr_slice(PyObject* obj, int start, int end) {
    if (!obj) return pystr_new_from("");
    pystr* s = (pystr*)obj;
    if (start < 0) start = 0;
    if (end > s->length) end = s->length;
    if (start >= end) return pystr_new_from("");
    int slice_len = end - start;
    pystr* result = malloc(sizeof(pystr));
    result->base.type = PY_STR;
    result->base.refcount = 1;
    result->length = slice_len;
    result->capacity = slice_len + 1;
    result->data = malloc(result->capacity);
    strncpy(result->data, s->data + start, slice_len);
    result->data[slice_len] = '\0';
    return (PyObject*)result;
};

int pystr_find(PyObject* obj, const char* sub) {
    if (!obj || !sub) return -1;
    pystr* s = (pystr*)obj;
    char* found = strstr(s->data, sub);
    return found ? (found - s->data) : -1;
}

PyObject* pystr_replace(PyObject* obj, const char* old_sub, const char* new_sub) {
    if (!obj) return pystr_new_from("");
    pystr* s = (pystr*)obj;
    char* found = strstr(s->data, old_sub);
    if (!found) return (PyObject*)s;
    int old_len = strlen(old_sub);
    int new_len = strlen(new_sub);
    int new_capacity = s->length - old_len + new_len + 1;
    char* new_data = malloc(new_capacity);
    strncpy(new_data, s->data, found - s->data);
    strcpy(new_data + (found - s->data), new_sub);
    strcpy(new_data + (found - s->data) + new_len, found + old_len);
    pystr* result = malloc(sizeof(pystr));
    result->base.type = PY_STR;
    result->base.refcount = 1;
    result->length = strlen(new_data);
    result->capacity = new_capacity;
    result->data = new_data;
    return (PyObject*)result;
};
