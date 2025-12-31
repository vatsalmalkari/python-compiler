#include <stdio.h>
#include "strings.h"

int main() {
    printf("String Tests\n");

    pystr* s = pystr_new_from("Hello");
    printf("Initial string: %s\n", s->data);
    printf("Length: %d\n", pystr_len(s));

    pystr_append(s, '!');
    printf("After pystr_append: %s\n", s->data);
    printf("Length: %d\n", pystr_len(s));

    pystr_appends(s, " World");
    printf("After pystr_appends: %s\n", s->data);
    printf("Length: %d\n", pystr_len(s));

    pystr_assign(s, "New string");
    printf("After pystr_assign: %s\n", s->data);
    printf("Length: %d\n", pystr_len(s));

    pystr* empty = pystr_new_from("");
    printf("Empty string length: %d\n", pystr_len(empty));

    pystr_del(s);
    return 0;
}
