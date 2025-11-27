#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void pylist_append(void* self, const char* text) {
    pylist* pl = (pylist*)self;
    
    // Create new node
    lnode* new_node = malloc(sizeof(lnode));
    new_node->text = malloc(strlen(text) + 1);
    strcpy(new_node->text, text);
    new_node->next = NULL;
    
    // Add to list
    if (pl->head == NULL) {
        // First element
        pl->head = new_node;
        pl->tail = new_node;
    } else {
        // Append to end
        pl->tail->next = new_node;
        pl->tail = new_node;
    }
    
    pl->count++;
}

void pylist_print(void* self) {
    pylist* pl = (pylist*)self;
    printf("[");
    
    lnode* current = pl->head;
    while (current != NULL) {
        printf("'%s'", current->text);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("] (length: %d)\n", pl->count);
}

int pylist_len(void* self) {
    pylist* pl = (pylist*)self;
    return pl->count;
}

int pylist_index(void* self, const char* text) {
    pylist* pl = (pylist*)self;
    lnode* current = pl->head;
    int index = 0;
    
    while (current != NULL) {
        if (strcmp(current->text, text) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }
    
    return -1; // Not found
}

char* pylist_get(void* self, int index) {
    pylist* pl = (pylist*)self;
    
    if (index < 0 || index >= pl->count) {
        return NULL; // Index out of bounds
    }
    
    lnode* current = pl->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    return current->text;
}

void pylist_set(void* self, int index, const char* text) {
    pylist* pl = (pylist*)self;
    
    if (index < 0 || index >= pl->count) {
        return; 
    }
    
    lnode* current = pl->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    
    free(current->text);
    current->text = malloc(strlen(text) + 1);
    strcpy(current->text, text);
}

pylist* pylist_new() {
    pylist* pl = malloc(sizeof(pylist));
    pl->head = NULL;
    pl->tail = NULL;
    pl->count = 0;
    
    // Connect method pointers
    pl->append = pylist_append;
    pl->print = pylist_print;
    pl->len = pylist_len;
    pl->index = pylist_index;
    pl->get = pylist_get;
    pl->set = pylist_set;
    
    return pl;
}

void pylist_del(void* self) {
    pylist* pl = (pylist*)self;
    
    // Free all nodes
    lnode* current = pl->head;
    while (current != NULL) {
        lnode* next = current->next;
        free(current->text);  
        free(current);        
        current = next;
    }
    
    free(pl);
}





