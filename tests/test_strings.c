#include <stdio.h>
#include "strings.h"
#include "pyobject.h"
#include <stdlib.h>
#include <string.h>
#include "pystr.h"

int main() {
    printf(" Strings Tests \n");

   pystr* s = (pystr*)pystr_new_from("Hello");

    pystr_print((PyObject*)s);
    printf("Length: %d\n", pystr_len((PyObject*)s));
    pystr_append_char((PyObject*)s, '!');
    
    printf("After pystr_append_char '!': "); pystr_print((PyObject*)s);
    printf("Length: %d\n", pystr_len(s));

    pystr_append_char((PyObject*)s, '?');
    printf("After pystr_append '?': "); pystr_print(s);
    printf("Length: %d\n", pystr_len(s));

    pystr_appends(s, " World");
    printf("After pystr_appends ' World': "); pystr_print(s);
    printf("Length: %d\n", pystr_len(s));

    pystr_assign(s, "New string");
    printf("After pystr_assign 'New string': "); pystr_print(s);
    printf("Length: %d\n", pystr_len(s));

    pystr* empty = (pystr*)pystr_new_from("");
    printf("Empty string: "); pystr_print((PyObject*)empty);
    printf("Length: %d\n", pystr_len((PyObject*)empty));

    pystr_append_char((PyObject*)empty, 'A');
    pystr_append_char((PyObject*)empty, 'B');
    pystr_append_char((PyObject*)empty, 'C');
    printf("After multiple appends to empty string: "); pystr_print((PyObject*)empty);
    printf("Length: %d\n", pystr_len((PyObject*)empty));

    pystr_del((PyObject*)s);
    pystr_del((PyObject*)empty);
    printf("Strings deleted successfully.\n");

    return 0;
}
