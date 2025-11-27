#include "pydict.h"

struct pydict* pydict_new() {
    struct pydict *p = malloc(sizeof(struct pydict));
    p->head = NULL;
    p->tail = NULL;
    p->count = 0;
    return p;
}

struct dnode* pydict_find(struct pydict* self, const char* key) {
    struct dnode *cur = self->head;
    while (cur != NULL) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void pydict_put(struct pydict* self, const char* key, const char* value) {
    struct dnode *existing = pydict_find(self, key);
    if (existing != NULL) {

        free(existing->value);
        existing->value = malloc(strlen(value) + 1);
        strcpy(existing->value, value);
        return;
    }

    // Create new node
    struct dnode *node = malloc(sizeof(struct dnode));
    node->key = malloc(strlen(key) + 1);
    node->value = malloc(strlen(value) + 1);
    strcpy(node->key, key);
    strcpy(node->value, value);
    node->next = NULL;

    // Append to list
    if (self->head == NULL) {
        self->head = node;
        self->tail = node;
    } else {
        self->tail->next = node;
        self->tail = node;
    }

    self->count++;
}

char* pydict_get(struct pydict* self, const char* key, const char* default_value) {
    struct dnode *found = pydict_find(self, key);
    if (found != NULL) {
        return found->value;
    }
    return (char*)default_value;
}

int pydict_len(struct pydict* self) {
    return self->count;
}

void pydict_print(struct pydict* self) {
    printf("{");
    struct dnode *cur = self->head;
    while (cur != NULL) {
        printf("'%s': '%s'", cur->key, cur->value);
        if (cur->next != NULL) printf(", ");
        cur = cur->next;
    }
    printf("}\n");
}

void pydict_del(struct pydict* self) {
    struct dnode *cur = self->head;
    while (cur != NULL) {
        struct dnode *next = cur->next;
        free(cur->key);
        free(cur->value);
        free(cur);
        cur = next;
    }
    free(self);
}