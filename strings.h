#ifndef STRING_H
#define STRING_H

typedef struct {
    int length;
    int alloc;
    char *data;
    void (*append)(void* self, char ch);
    void (*appends)(void* self, const char* s);
    void (*assign)(void* self, const char* s);
    void (*print)(void* self);
    int (*len)(void* self);
    char* (*str)(void* self);
} pystr;

pystr* pystr_new();
pystr* pystr_new_from(const char* s);
void pystr_del(void* self);

#endif