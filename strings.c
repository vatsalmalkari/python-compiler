#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "strings.h"

void pystr_append(void* self, char ch) {
    pystr* ps = (pystr*)self;
    if (ps->length + 2 > ps->alloc) {
        ps->alloc += 10;
        ps->data = realloc(ps->data, ps->alloc);
    }
    ps->data[ps->length] = ch;
    ps->length++;
    ps->data[ps->length] = '\0';
}

void pystr_appends(void* self, const char* s) {
    pystr* ps = (pystr*)self;
    for (int i = 0; s[i] != '\0'; i++) {
        pystr_append(ps, s[i]);
    }
}

void pystr_assign(void* self, const char* s) {
    pystr* ps = (pystr*)self;
    ps->length = 0;
    ps->data[0] = '\0';
    pystr_appends(ps, s);
}

void pystr_print(void* self) {
    pystr* ps = (pystr*)self;
    printf("\"%s\"\n", ps->data);
}

int pystr_len(void* self) {
    pystr* ps = (pystr*)self;
    return ps->length;
}

char* pystr_str(void* self) {
    pystr* ps = (pystr*)self;
    return ps->data;
}

// Constructor
pystr* pystr_new() {
    pystr* ps = malloc(sizeof(pystr));
    ps->length = 0;
    ps->alloc = 10;
    ps->data = malloc(ps->alloc);
    ps->data[0] = '\0';
    
    // Connect method pointers
    ps->append = pystr_append;
    ps->appends = pystr_appends;
    ps->assign = pystr_assign;
    ps->print = pystr_print;
    ps->len = pystr_len;
    ps->str = pystr_str;
    
    return ps;
}

pystr* pystr_new_from(const char* s) {
    pystr* ps = pystr_new();
    pystr_appends(ps, s);
    return ps;
}

void pystr_del(void* self) {
    pystr* ps = (pystr*)self;
    free(ps->data);
    free(ps);
}
