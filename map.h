#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MapEntry;
struct Map;

struct Map {
    struct MapEntry *__head;
    struct MapEntry *__tail;
    int __count;
    
    void (*put)(struct Map *self, const char *key, int value);
    int  (*get)(struct Map *self, const char *key, int default_value);
    int  (*size)(struct Map *self);
    void (*dump)(struct Map *self);
    void (*del)(struct Map *self);
};

struct Map *Map_new();

#endif