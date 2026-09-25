#include "security.h"
#include <limits.h>
int check_division(int divisor) {
    if (divisor == 0) {
        return 0;  // Division by zero is not allowed
    }
    return 1;
}

int check_list_index(int index, int size) {
    if (index < 0 || index >= size) {
        return 0;  // Index out of bounds is not allowed
    }
    return 1;
}
int check_integer_overflow(int a, int b, char op) {

    if (op == '+') {
        if (b > 0 && a > INT_MAX - b) return 0;
        if (b < 0 && a < INT_MIN - b) return 0;
    }

    else if (op == '-') {
        if (b > 0 && a < INT_MIN + b) return 0;
        if (b < 0 && a > INT_MAX + b) return 0;
    }

    else if (op == '*') {
        if (a > 0 && b > 0 && a > INT_MAX / b) return 0;
        if (a > 0 && b < 0 && b < INT_MIN / a) return 0;
        if (a < 0 && b > 0 && a < INT_MIN / b) return 0;
        if (a < 0 && b < 0 && a < INT_MAX / b) return 0;
    }
    else if (op == '/') {
        if (b == 0) return 0;

        // INT_MIN / -1 cannot be represented by int
        if (a == INT_MIN && b == -1) return 0;
    }

    return 1;
}
int check_string_length(size_t length) {
    if (length > 1000) {  
        return 0; 
    }
    return 1;
}
