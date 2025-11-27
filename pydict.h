#ifndef PYDICT_H
#define PYDICT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef  struct pydict pydict;
typedef  struct dnode dnode;


struct dnode {
    char *key;
    char *value;
    struct dnode *next;
};

struct pydict {
    struct dnode *head;
    struct dnode *tail;
    int count;
};

// Function declarations
struct pydict* pydict_new();
void pydict_put(struct pydict* self, const char* key, const char* value);
char* pydict_get(struct pydict* self, const char* key, const char* default_value);
int pydict_len(struct pydict* self);
void pydict_print(struct pydict* self);
void pydict_del(struct pydict* self);
struct dnode* pydict_find(struct pydict* self, const char* key);

#endif