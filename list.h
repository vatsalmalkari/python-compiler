// list.h
#ifndef LIST_H
#define LIST_H

typedef struct {
    int* items;
    int size;
    int capacity;
} pylist;

pylist* pylist_new();
void pylist_del(pylist* l);
void pylist_append(pylist* l, int val);
int pylist_len(pylist* l);
void pylist_print(pylist* l);

void pylist_set(pylist* l, int index, int val);
int pylist_get(pylist* l, int index);
int pylist_index(pylist* l, int val);


#endif
