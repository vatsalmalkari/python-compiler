#include "map.h"

struct MapEntry {
    char *key;
    int value;
    struct MapEntry *__prev;
    struct MapEntry *__next;
};

void __Map_put(struct Map *self, const char *key, int value) {
    struct MapEntry *current = self->__head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;  
            return;
        }
        current = current->__next;
    }
    
    struct MapEntry *new_entry = malloc(sizeof(struct MapEntry));
    new_entry->key = strdup(key);  
    new_entry->value = value;
    new_entry->__prev = self->__tail;
    new_entry->__next = NULL;
    
    if (self->__head == NULL) {
        self->__head = new_entry;
    } else {
        self->__tail->__next = new_entry;
    }
    self->__tail = new_entry;
    self->__count++;
}

int __Map_get(struct Map *self, const char *key, int default_value) {
    struct MapEntry *current = self->__head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->__next;
    }
    return default_value;
}

int __Map_size(struct Map *self) {
    return self->__count;
}

void __Map_dump(struct Map *self) {
    printf("{");
    struct MapEntry *current = self->__head;
    while (current != NULL) {
        printf("'%s': %d", current->key, current->value);
        if (current->__next != NULL) printf(", ");
        current = current->__next;
    }
    printf("}\n");
}

void __Map_del(struct Map *self) {
    struct MapEntry *current = self->__head;
    while (current != NULL) {
        struct MapEntry *next = current->__next;
        free(current->key);  
        free(current);       
        current = next;
    }
    free(self);  
}

struct Map *Map_new() {
    struct Map *m = malloc(sizeof(struct Map));
    
    m->__head = NULL;
    m->__tail = NULL;
    m->__count = 0;
    
    m->put = __Map_put;
    m->get = __Map_get;
    m->size = __Map_size;
    m->dump = __Map_dump;
    m->del = __Map_del;
    
    return m;
}