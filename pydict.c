// pydict.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pydict.h"

pydict* pydict_new() {
    pydict* d = malloc(sizeof(pydict));
    d->size = 0;
    d->capacity = 4;
    d->items = malloc(sizeof(dict_entry)*d->capacity);
    return d;
}

void pydict_del(pydict* d) {
    if(!d) return;
    free(d->items);
    free(d);
}

void pydict_set(pydict* d, const char* key, int val) {
    for(int i=0;i<d->size;i++){
        if(strcmp(d->items[i].key,key)==0){ d->items[i].value=val; return; }
    }
    if(d->size>=d->capacity){
        d->capacity*=2;
        d->items=realloc(d->items,sizeof(dict_entry)*d->capacity);
    }
    strcpy(d->items[d->size].key,key);
    d->items[d->size].value=val;
    d->size++;
}

int pydict_get(pydict* d, const char* key) {
    for(int i=0;i<d->size;i++){
        if(strcmp(d->items[i].key,key)==0) return d->items[i].value;
    }
    return 0;
}

void pydict_print(pydict* d) {
    printf("{");
    for(int i=0;i<d->size;i++){
        printf("\"%s\": %d", d->items[i].key, d->items[i].value);
        if(i<d->size-1) printf(", ");
    }
    printf("}\n");
}
int pydict_len(pydict* d) {
    return d->size;
}   
void pydict_put(pydict* d, const char* key, const char* value) {
    for(int i=0;i<d->size;i++){
        if(strcmp(d->items[i].key,key)==0){
            d->items[i].value = atoi(value);
            return;
        }
    }
    if(d->size>=d->capacity){
        d->capacity*=2;
        d->items=realloc(d->items,sizeof(dict_entry)*d->capacity);
    }
    strcpy(d->items[d->size].key,key);
    d->items[d->size].value=atoi(value);
    d->size++;
}
void pydict_dump(pydict* d) {
    for(int i=0;i<d->size;i++){
        printf("%s: %d\n", d->items[i].key, d->items[i].value);
    }
}
int pydict_size(pydict* d) {
    return d->size;
}