// pydict.h
#ifndef PYDICT_H
#define PYDICT_H

typedef struct {
    char key[50];
    int value;
} dict_entry;

typedef struct {
    dict_entry* items;
    int size;
    int capacity;
} pydict;

pydict* pydict_new();
void pydict_del(pydict* d);
void pydict_set(pydict* d, const char* key, int val);
int pydict_get(pydict* d, const char* key);
void pydict_print(pydict* d);

int pydict_len(pydict* d);
void pydict_put(pydict* d, const char* key, const char* value);
void pydict_dump(pydict* d);
int pydict_size(pydict* d);


#endif
