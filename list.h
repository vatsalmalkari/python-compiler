#ifndef LIST_H
#define LIST_H

// List node structure
typedef struct lnode {
    char *text;              
    struct lnode *next;      
} lnode;

typedef struct {
    lnode *head;
    lnode *tail;
    int count;
    void (*append)(void* self, const char* text);
    void (*print)(void* self);
    int (*len)(void* self);
    int (*index)(void* self, const char* text);
    char* (*get)(void* self, int index);
    void (*set)(void* self, int index, const char* text);
} pylist;

pylist* pylist_new();
void pylist_del(void* self);
void demo_lists();

#endif