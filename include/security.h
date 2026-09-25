#ifndef SECURITY_H
#define SECURITY_H
#include <stddef.h>
#include <stdint.h>
int check_division(int divisor);
int check_list_index(int index, int size);
int check_integer_overflow(int a, int b, char op);
int check_string_length(size_t length);

#endif // SECURITY_H
