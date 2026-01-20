#include <stdio.h>
#include <stdlib.h>
#include "include/strings.h"
#include <ctype.h>
#include "list.h"
#include "strings.h"
#include "pydict.h"
#include "map.h"

#define MAX_VARS 26
#define MAX_LINE 200
#define MAX_TOKENS 50
#define MAX_LIST_VARS 10
#define MAX_DICT_VARS 10
#define MAX_MAP_VARS 10
#define MAX_STR_VARS 26

typedef struct { char name; int value; } Variable;
Variable vars[MAX_VARS];
int var_count = 0;

Variable* find_var(char name){
    for(int i=0;i<var_count;i++) if(vars[i].name==name) return &vars[i];
    return NULL;
}
void set_var(char name,int value){
    Variable* v=find_var(name);
    if(v) v->value=value;
    else if(var_count<MAX_VARS){ vars[var_count].name=name; vars[var_count].value=value; var_count++; }
}
int get_value(char* token){
    if(isdigit(token[0])||(token[0]=='-'&&isdigit(token[1]))) return atoi(token);
    if(isalpha(token[0])){ Variable* v=find_var(token[0]); return v?v->value:0; }
    return 0;
}

pystr* str_vars[MAX_STR_VARS];
void init_str_vars(){ for(int i=0;i<MAX_STR_VARS;i++) str_vars[i]=NULL; }
pystr* find_str_var(char name){ return (name>='A'&&name<='Z')? str_vars[name-'A']:NULL; }
void set_str_var(char name,pystr* s){ if(name>='A'&&name<='Z'){ if(str_vars[name-'A']) pystr_del(str_vars[name-'A']); str_vars[name-'A']=s; } }

pylist* list_vars[MAX_LIST_VARS];
void init_list_vars(){ for(int i=0;i<MAX_LIST_VARS;i++) list_vars[i]=NULL; }
pylist* find_list_var(char name){ return (name>='L'&&name<='O')? list_vars[name-'L']:NULL; }
void set_list_var(char name,pylist* l){ if(name>='L'&&name<='O'){ if(list_vars[name-'L']) pylist_del(list_vars[name-'L']); list_vars[name-'L']=l; } }

pydict* dict_vars[MAX_DICT_VARS];
void init_dict_vars(){ for(int i=0;i<MAX_DICT_VARS;i++) dict_vars[i]=NULL; }
pydict* find_dict_var(char name){ return (name>='D'&&name<='G')? dict_vars[name-'D']:NULL; }
void set_dict_var(char name,pydict* d){ if(name>='D'&&name<='G'){ if(dict_vars[name-'D']) pydict_del(dict_vars[name-'D']); dict_vars[name-'D']=d; } }

struct Map* map_vars[MAX_MAP_VARS];
void init_map_vars(){ for(int i=0;i<MAX_MAP_VARS;i++) map_vars[i]=NULL; }
struct Map* find_map_var(char name){ return (name>='M'&&name<='P')? map_vars[name-'M']:NULL; }
void set_map_var(char name,struct Map* m){ if(name>='M'&&name<='P'){ if(map_vars[name-'M']) map_vars[name-'M']->del(map_vars[name-'M']); map_vars[name-'M']=m; } }

int evaluate_expression(char* expr){
    char* tokens[MAX_TOKENS]; int tcount=0;
    char buffer[MAX_LINE]; strncpy(buffer,expr,MAX_LINE); buffer[MAX_LINE-1]=0;
    char* tok = strtok(buffer," ");
    while(tok && tcount<MAX_TOKENS){ tokens[tcount++]=tok; tok=strtok(NULL," "); }
    if(tcount==0) return 0;

    int values[MAX_TOKENS]; char ops[MAX_TOKENS][4]; int vcount=0, opcount=0;
    values[vcount++]=get_value(tokens[0]);
    for(int i=1;i<tcount;i+=2){ strcpy(ops[opcount++],tokens[i]); values[vcount++]=get_value(tokens[i+1]); }

    for(int i=0;i<opcount;i++){
        if(strcmp(ops[i],"*")==0||strcmp(ops[i],"/")==0||strcmp(ops[i],"mod")==0){
            int left=values[i], right=values[i+1], res=0;
            if(strcmp(ops[i],"*")==0) res=left*right;
            else if(strcmp(ops[i],"/")==0) res=(right?left/right:0);
            else if(strcmp(ops[i],"mod")==0) res=(right?left%right:0);
            values[i]=res;
            for(int j=i+1;j<vcount-1;j++) values[j]=values[j+1];
            for(int j=i;j<opcount-1;j++) strcpy(ops[j],ops[j+1]);
            vcount--; opcount--; i--;
        }
    }
    int res=values[0];
    for(int i=0;i<opcount;i++){ if(strcmp(ops[i],"+")==0) res+=values[i+1]; else if(strcmp(ops[i],"-")==0) res-=values[i+1]; }
    return res;
}

int evaluate_condition(char* cond){
    char* ops[]={">=","<=","==","!=",">","<"}; int nops=6;
    char* found=NULL; int lenop=0;
    for(int i=0;i<nops;i++){ char* pos=strstr(cond,ops[i]); if(pos){ found=ops[i]; lenop=strlen(found); break; } }
    if(!found) return 0;
    char left[MAX_LINE], right[MAX_LINE]; strncpy(left,cond,MAX_LINE); left[MAX_LINE-1]=0;
    char* pos=strstr(left,found); *pos=0; strcpy(right,pos+lenop);
    int lval=evaluate_expression(left), rval=evaluate_expression(right);
    if(strcmp(found,"==")==0) return lval==rval;
    else if(strcmp(found,"!=")==0) return lval!=rval;
    else if(strcmp(found,">")==0) return lval>rval;
    else if(strcmp(found,"<")==0) return lval<rval;
    else if(strcmp(found,">=")==0) return lval>=rval;
    else if(strcmp(found,"<=")==0) return lval<=rval;
    return 0;
}

int count_leading_spaces(char* line){ int cnt=0; while(*line==' '||*line=='\t'){ cnt++; line++; } return cnt; }

void interpret(char* line) {
    char* trimmed = line;
    while(*trimmed == ' ' || *trimmed == '\t') trimmed++;
    if(strlen(trimmed) == 0) return;

    char* end_ptr = trimmed + strlen(trimmed) - 1;
    while(end_ptr > trimmed && isspace((unsigned char)*end_ptr)) {
        *end_ptr = '\0';
        end_ptr--;
    }

    char* eq = strchr(trimmed, '=');
   
    if (eq && *(eq+1) != '=' && *(eq-1) != '!' && *(eq-1) != '<' && *(eq-1) != '>') {
        char var_name = trimmed[0];
        char* val_str = eq + 1;
        while(*val_str == ' ') val_str++;
       
        if (val_str[0] == '\"' || val_str[0] == '\'') {
            char quote = val_str[0];
            char* start = val_str + 1;
            char* end = strchr(start, quote); 
            if (end) {
                int len = end - start;
                char temp[MAX_LINE];
                strncpy(temp, start, len);
                temp[len] = '\0';
              
                set_str_var(var_name, pystr_new_from(temp)); 
            }
        }
       
        else if (val_str[0] == '[') {
            pylist* l = pylist_new();
            char* start = val_str + 1;
            char* end = strchr(start, ']');
            if (end) {
                char content[MAX_LINE];
                strncpy(content, start, end - start);
                content[end - start] = '\0';
                
                char* token = strtok(content, ", ");
                while(token) {
                    pylist_append(l, atoi(token));
                    token = strtok(NULL, ", ");
                }
            }
            set_list_var(var_name, l); 
        }

        else {
            set_var(var_name, evaluate_expression(val_str));
        }
        return;
    }

    if(strchr(trimmed, '.')) {
        char obj = trimmed[0];
        char* dot = strchr(trimmed, '.') + 1;
        if(strncmp(dot, "append", 6) == 0) {
            char* arg = strchr(dot, '('); 
            if(arg) { 
                arg++; char* arg_end = strchr(arg, ')'); if(arg_end) *arg_end = 0; 

                pylist* l = find_list_var(obj); 
                if(l) { pylist_append(l, evaluate_expression(arg)); return; }

                pystr* s = find_str_var(obj);
                if(s) {
                    if((arg[0] == '\'' || arg[0] == '\"') && strlen(arg) >= 2) 
                        pystr_append_char(s, arg[1]);
                    else 
                        pystr_append_char(s, (char)evaluate_expression(arg));
                    return;
                }
            }
        }
        return;
    }

    if(strncmp(trimmed, "print(", 6) == 0) {
        char* content = trimmed + 6;
        char* p_end = strrchr(content, ')'); if(p_end) *p_end = 0;

        if(strlen(content) == 1 && isupper(content[0])) {
            pystr* s = find_str_var(content[0]); if(s) { pystr_print(s); return; }
            pylist* l = find_list_var(content[0]); if(l) { pylist_print(l); return; }
           
            printf("%d\n", get_value(content));
        } else {
            printf("%d\n", evaluate_expression(content));
        }
        return;
    }
}

void execute_statements(char* stmts) {
    if (!stmts || strlen(stmts) == 0) return;

    char* copy = strdup(stmts);
    char* lines[MAX_TOKENS];
    int nlines = 0;

    char* curr = copy;
    while (curr && *curr && nlines < MAX_TOKENS) {
        lines[nlines++] = curr;
        char* nl = strchr(curr, '\n');
        if (nl) {
            *nl = 0;
            curr = nl + 1;
        } else break;
    }

    for (int i = 0; i < nlines; i++) {
        int indent = count_leading_spaces(lines[i]);
        char* trimmed = lines[i] + indent;
        if (strlen(trimmed) == 0) continue;

        if (strncmp(trimmed, "if ", 3) == 0) {
            int cond_val = evaluate_condition(trimmed + 3);
            char if_block[MAX_LINE * 10] = "";
            int j = i + 1, block_indent = -1;
            
            while (j < nlines) {
                int ind = count_leading_spaces(lines[j]);
                if (strlen(lines[j] + ind) == 0) { j++; continue; }
                if (block_indent == -1) block_indent = ind;
                if (ind < block_indent) break;
                strcat(if_block, lines[j] + block_indent); strcat(if_block, "\n");
                j++;
            }
            if (cond_val) execute_statements(if_block);
            
            if (j < nlines) {
                char* next_trimmed = lines[j] + count_leading_spaces(lines[j]);
                if (strncmp(next_trimmed, "else", 4) == 0) {
                    char else_block[MAX_LINE * 10] = "";
                    int k = j + 1, else_indent = -1;
                    while (k < nlines) {
                        int ind = count_leading_spaces(lines[k]);
                        if (strlen(lines[k] + ind) == 0) { k++; continue; }
                        if (else_indent == -1) else_indent = ind;
                        if (ind < else_indent) break;
                        strcat(else_block, lines[k] + else_indent); strcat(else_block, "\n");
                        k++;
                    }
                    if (!cond_val) execute_statements(else_block);
                    j = k;
                }
            }
            i = j - 1;
        }
      
        else if (strncmp(trimmed, "while ", 6) == 0) {
            char* cond_str = trimmed + 6;
            char block[MAX_LINE * 10] = "";
            int j = i + 1, block_indent = -1;
            while (j < nlines) {
                int ind = count_leading_spaces(lines[j]);
                if (strlen(lines[j] + ind) == 0) { j++; continue; }
                if (block_indent == -1) block_indent = ind;
                if (ind < block_indent) break;
                strcat(block, lines[j] + block_indent); strcat(block, "\n");
                j++;
            }
            while (evaluate_condition(cond_str)) execute_statements(block);
            i = j - 1;
        }
       
        else if (strncmp(trimmed, "for ", 4) == 0) {
            char var_name = (trimmed + 4)[0];
            char* range_ptr = strstr(trimmed, "range(");
            if (range_ptr) {
                int limit = evaluate_expression(range_ptr + 6);
                char block[MAX_LINE * 10] = "";
                int j = i + 1, block_indent = -1;
                while (j < nlines) {
                    int ind = count_leading_spaces(lines[j]);
                    if (strlen(lines[j] + ind) == 0) { j++; continue; }
                    if (block_indent == -1) block_indent = ind;
                    if (ind < block_indent) break;
                    strcat(block, lines[j] + block_indent); strcat(block, "\n");
                    j++;
                }
                for (int val = 0; val < limit; val++) {
                    set_var(var_name, val);
                    execute_statements(block);
                }
                i = j - 1;
            }
        }
        else {
            interpret(lines[i]);
        }
    }
    free(copy); 
}