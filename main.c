#include <stdio.h>
#include <string.h>
#include "interpreter.h"

int main(){
    init_str_vars(); init_list_vars(); init_dict_vars(); init_map_vars();

    char line[MAX_LINE];
    char block[MAX_LINE*20]="";
    int in_block=0, base_indent=0;

    printf(">>> ");
    while(fgets(line,MAX_LINE,stdin)){
        int indent=0; while(line[indent]==' '||line[indent]=='\t') indent++;

        if(strlen(block)>0 && indent<=base_indent){
            execute_statements(block); block[0]=0; in_block=0;
        }

        char* trimmed=line+indent;
        if(strncmp(trimmed,"if",2)==0 || strncmp(trimmed,"while",5)==0 || strncmp(trimmed,"for",3)==0){
            in_block=1; base_indent=indent;
        }

        strcat(block,line);
        if(!in_block){ execute_statements(block); block[0]=0; }

        printf(in_block ? "... " : ">>> ");
    }
    if(strlen(block)>0) execute_statements(block);
    return 0;
}
