// strings.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"

pystr* pystr_new_from(char* s) {
    pystr* p = malloc(sizeof(pystr));
    p->len = strlen(s);
    p->data = malloc(p->len + 1);
    strcpy(p->data, s);
    return p;
}

void pystr_del(pystr* s) {
    if (!s) return;
    free(s->data);
    free(s);
}

void pystr_print(pystr* s) {
    if (!s) return;
    printf("%s\n", s->data);
}
void pystr_append(void* self, char ch) {
    pystr* s = (pystr*)self;
    size_t len = strlen(s->data);
    s->data = realloc(s->data, len + 2);
    s->data[len] = ch;
    s->data[len + 1] = '\0';
}

void pystr_appends(void* self, const char* str) {
    pystr* s = (pystr*)self;
    size_t len1 = strlen(s->data);
    size_t len2 = strlen(str);
    s->data = realloc(s->data, len1 + len2 + 1);
    strcpy(s->data + len1, str);
}

void pystr_append_char(pystr* s, char c) {
    if (!s) return;
    s->len++;
    s->data = realloc(s->data, s->len + 2);
    s->data[s->len] = c; 
    s->len++;
    s->data[s->len] = '\0';
}

void pystr_assign(void* self, const char* str) {
    pystr* s = (pystr*)self;
    free(s->data);
    s->data = strdup(str);
}

int pystr_len(pystr* s){ 
    return strlen(s->data); 
}
