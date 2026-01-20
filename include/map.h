#ifndef MAP_H
#define MAP_H

#include "pyobject.h"

struct MapEntry {
    char* key;
    PyObject* value;
    struct MapEntry* __prev;
    struct MapEntry* __next;
};

struct Map {
    struct MapEntry* __head;
    struct MapEntry* __tail;
    int __count;

    void (*put)(struct Map*, const char*, PyObject*);
    PyObject* (*get)(struct Map*, const char*, PyObject*);
    int (*size)(struct Map*);
    void (*dump)(struct Map*);
    void (*del)(struct Map*);
};

struct Map* Map_new(void);
int map_len(struct Map* m);

#endif
