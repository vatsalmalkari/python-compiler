// strings.h
#ifndef STRINGS_H
#define STRINGS_H

typedef struct {
    char* data;
    int len;
} pystr;

pystr* pystr_new_from(char* s);
void pystr_del(pystr* s);
void pystr_print(pystr* s);

void pystr_append(void* self, char ch);
void pystr_appends(void* self, const char* s);
void pystr_assign(void* self, const char* s);
void pystr_append_char(pystr* s, char c);
int pystr_len(pystr* self);


#endif
