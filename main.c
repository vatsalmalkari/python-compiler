#include <stdio.h>
#include "interpreter.h"
#include <string.h>

#define MAX_BLOCK (MAX_LINE * 20)

int main(){
    init_str_vars(); 
    init_list_vars(); 
    init_dict_vars(); 
    init_map_vars();

    char line[MAX_LINE];
    char block[MAX_BLOCK] = "";
    int in_block = 0, base_indent = 0;

    printf(">>> ");
    while(fgets(line, MAX_LINE, stdin)){
        // Measure indentation
        int indent = 0; 
        while(line[indent] == ' ' || line[indent] == '\t') indent++;

        // If we have a pending block and indent decreases, execute it
        if(strlen(block) > 0 && indent <= base_indent && in_block) {
            execute_statements(block); 
            block[0] = 0; 
            in_block = 0;
        }

        char* trimmed = line + indent;
        
        // Skip empty lines
        if(strlen(trimmed) == 0 || trimmed[0] == '\n') {
            printf(in_block ? "... " : ">>> ");
            continue;
        }

        // Detect block start
        if(!in_block && (strstr(trimmed, "if ") || strstr(trimmed, "while ") || strstr(trimmed, "for "))) {
            in_block = 1;
            base_indent = indent;
        }

        // Bounds check
        if(strlen(block) + strlen(line) + 1 > MAX_BLOCK) {
            fprintf(stderr, "Error: input too large\n");
            block[0] = 0;
            in_block = 0;
            printf(">>> ");
            continue;
        }

        strcat(block, line);
        
        // Execute if not in a block
        if(!in_block) {
            execute_statements(block); 
            block[0] = 0;
        }

        printf(in_block ? "... " : ">>> ");
    }
    
    // Execute any remaining block
    if(strlen(block) > 0) execute_statements(block);
    // Compile with AddressSanitizer
// gcc -fsanitize=address -g interpreter.c ...
// This catches memory bugs automatically
    return 0;
}