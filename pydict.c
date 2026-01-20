#include "include/pydict.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

pydict* pydict_new() {
    pydict* d = malloc(sizeof(pydict));
    d->size = 0;
    d->capacity = 4;
    d->items = malloc(sizeof(dict_entry) * d->capacity);
    return d;
}

void pydict_del(pydict* d) {
    if (!d) return;
    for (int i = 0; i < d->size; i++)
        py_decref(d->items[i].value);
    free(d->items);
    free(d);
}

void pydict_set(pydict* d, const char* key, PyObject* val) {
    if (!d || !val) return;

    for (int i = 0; i < d->size; i++) {
        if (strcmp(d->items[i].key, key) == 0) {
            py_decref(d->items[i].value);
            py_incref(val);
            d->items[i].value = val;
            return;
        }
    }

    if (d->size >= d->capacity) {
        d->capacity *= 2;
        d->items = realloc(d->items, sizeof(dict_entry) * d->capacity);
    }

    d->items[d->size].key = strdup(key);
    py_incref(val);
    d->items[d->size].value = val;
    d->size++;
}

PyObject* pydict_get(pydict* d, const char* key) {
    if (!d) return py_none();
    for (int i = 0; i < d->size; i++) {
        if (strcmp(d->items[i].key, key) == 0)
            return d->items[i].value;
    }
    return py_none();
}

int pydict_len(pydict* d) {
    return d ? d->size : 0;
}

void pydict_print(pydict* d) {
    if (!d) return;
    printf("{");
    for (int i = 0; i < d->size; i++) {
        if (i > 0) printf(", ");
        printf("\"%s\": ", d->items[i].key);
        pyobject_print(d->items[i].value);
    }
    printf("}\n");
}

void pydict_dump(pydict* d) {
    if (!d) return;
    for (int i = 0; i < d->size; i++) {
        printf("%s: ", d->items[i].key);
        pyobject_print(d->items[i].value);
        printf("\n");
    }
}
