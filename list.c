// list.c
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

pylist* pylist_new() {
    pylist* l = malloc(sizeof(pylist));
    l->size = 0;
    l->capacity = 4;
    l->items = malloc(sizeof(int) * l->capacity);
    return l;
}

void pylist_del(pylist* l) {
    if (!l) return;
    free(l->items);
    free(l);
}

void pylist_append(pylist* l, int val) {
    if (!l) return;
    if (l->size >= l->capacity) {
        l->capacity = (l->capacity == 0) ? 4 : l->capacity * 2;
        int* temp = realloc(l->items, sizeof(int) * l->capacity);
        if (temp == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return; 
        }
        l->items = temp;
    }
    l->items[l->size] = val;
    l->size++;
}

int pylist_len(pylist* l) {
    return l->size;
}

void pylist_print(pylist* l) {
    if (!l) return;
    printf("[");
    for (int i = 0; i < l->size; i++) {
        printf("%d", l->items[i]);
        if (i < l->size-1) printf(", ");
    }
    printf("]\n");
}
void pylist_set(pylist* l, int index, int val) {
    if (index < 0 || index >= l->size) return;
    l->items[index] = val;
}
int pylist_get(pylist* l, int index) {
    if (index < 0 || index >= l->size) return 0;
    return l->items[index];
}
int pylist_index(pylist* l, int val) {
    for (int i = 0; i < l->size; i++) {
        if (l->items[i] == val) return i;
    }
    return -1;
}
