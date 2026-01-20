#include <stdio.h>
#include "interpreter.h"

int main() {
    printf("Expression Tests\n");

    set_var('A', 10);
    set_var('B', 5);
    printf("A + B = %d\n", evaluate_expression("A + B"));
    printf("A - B = %d\n", evaluate_expression("A - B"));
    printf("A * B = %d\n", evaluate_expression("A * B"));
    printf("A / B = %d\n", evaluate_expression("A / B"));
    printf("A mod B = %d\n", evaluate_expression("A mod B"));

    printf("Condition A > B: %d\n", evaluate_condition("A > B"));
    printf("Condition A == 10: %d\n", evaluate_condition("A == 10"));
    printf("Condition B != 5: %d\n", evaluate_condition("B != 5"));

    return 0;
}
