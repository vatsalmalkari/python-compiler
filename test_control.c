#include <stdio.h>
#include "interpreter.h" 

int main() {
    init_str_vars(); init_list_vars(); init_dict_vars(); init_map_vars();

    printf("Control Flow Tests\n");

    char script[] =
        "A = 5\n"
        "B = 0\n"
        "while A > 0\n"
        "    B = B + A\n"
        "    A = A - 1\n"
        "print(B)\n"   
        "for C in range(3)\n"
        "    print(C)\n"
        "S = \"Hi\"\n"
        "print(S)\n";

    execute_statements(script);
    return 0;
}