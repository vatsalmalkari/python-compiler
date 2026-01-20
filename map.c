#include "include/map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void __Map_put(struct Map* self, const char* key, PyObject* value) {
    struct MapEntry* current = self->__head;
    while (current) {
        if (strcmp(current->key, key) == 0) {
            py_decref(current->value);
            py_incref(value);
            current->value = value;
            return;
        }
        current = current->__next;
    }

    struct MapEntry* new_entry = malloc(sizeof(struct MapEntry));
    new_entry->key = strdup(key);
    py_incref(value);
    new_entry->value = value;
    new_entry->__prev = self->__tail;
    new_entry->__next = NULL;

    if (!self->__head)
        self->__head = new_entry;
    else
        self->__tail->__next = new_entry;

    self->__tail = new_entry;
    self->__count++;
}

PyObject* __Map_get(struct Map* self, const char* key, PyObject* default_value) {
    struct MapEntry* current = self->__head;
    while (current) {
        if (strcmp(current->key, key) == 0)
            return current->value;
        current = current->__next;
    }
    return default_value;
}

int __Map_size(struct Map* self) { return self->__count; }

void __Map_dump(struct Map* self) {
    printf("{");
    struct MapEntry* current = self->__head;
    while (current) {
        printf("\"%s\": ", current->key);
        pyobject_print(current->value);
        if (current->__next) printf(", ");
        current = current->__next;
    }
    printf("}\n");
}

void __Map_del(struct Map* self) {
    struct MapEntry* current = self->__head;
    while (current) {
        struct MapEntry* next = current->__next;
        free(current->key);
        py_decref(current->value);
        free(current);
        current = next;
    }
    free(self);
}

int map_len(struct Map* m) { return m ? m->size(m) : 0; }

struct Map* Map_new() {
    struct Map* m = malloc(sizeof(struct Map));
    m->__head = m->__tail = NULL;
    m->__count = 0;

    m->put = __Map_put;
    m->get = __Map_get;
    m->size = __Map_size;
    m->dump = __Map_dump;
    m->del = __Map_del;

    return m;
}
