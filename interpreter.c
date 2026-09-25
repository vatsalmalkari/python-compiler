#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"
#include "pydict.h"
#include "pystr.h"
#include <string.h> 
#include <ctype.h>
#include "include/interpreter.h" 
#include "include/pyint.h"
#include "include/pyobject.h"
#include "security.h"
#define MAX_VARS 26
#define MAX_LINE 200
#define MAX_TOKENS 50
#define MAX_LIST_VARS 10
#define MAX_DICT_VARS 10
#define MAX_MAP_VARS 10
#define MAX_STR_VARS 26
#define MAX_BLOCK_SIZE (MAX_LINE * 10)

typedef struct {
    char* code;
    int line_number;
    int column;
} SourceLocation;

typedef struct {
    char* name;
    char* params[10];
    char* body;
    int param_count;
    int line_defined;
} Function;

Function functions[26];  
int function_count = 0;

typedef struct {
    char* operation;
    char* risk;
    char* mitigation;
} SecurityCheck;

TypedVariable vars[MAX_VARS];
int var_count = 0;

SecurityCheck Checks[] = { {"pystr_append", "buffer overflow", "dynamic capacity doubling + bounds check"},
    {"evaluate_expression", "integer overflow", "return 0 on error"},
    {"list indexing", "out of bounds", "validate index range before access"},
};

TypedVariable* find_var(char name) {
    if (name >= 'a' && name <= 'z'){
        return &vars[name - 'a'];
    }
    else{
        return NULL;
    }
}
void set_var(char name, int value) {
    if (name >= 'a' && name <= 'z') {
        vars[name - 'a'].name = name;
        vars[name - 'a'].value = value;
    } else {
        fprintf(stderr, "Error: variable name must be a-z\n");
    }
}
int get_value(char* token){
    if(isdigit(token[0])||(token[0]=='-'&&isdigit(token[1]))){
        return atoi(token);
    }
    if(isalpha(token[0])){ 
        TypedVariable* v= find_var(token[0]); 
        if (!v) {
            fprintf(stderr, "Error: variable not found\n");
            return 0;
        }
        else{
            return v->value;
        }
    }
    return 0;
}

pystr* str_vars[MAX_STR_VARS];
void init_str_vars(){ 
    for(int i=0;i<MAX_STR_VARS;i++){
        str_vars[i]=NULL;
    }
}
pystr* find_str_var(char name){
    if(name>='A'&&name<='Z'){
        return str_vars[name-'A'];
    }
    return NULL;
}
void set_str_var(char name,pystr* s){
    if(name>='A'&&name<='Z'){
        if(str_vars[name-'A']) pystr_del((PyObject*)str_vars[name-'A']);
        str_vars[name-'A']=s;
    }
}

pylist* list_vars[MAX_LIST_VARS];
void init_list_vars(){ 
    for(int i=0;i<MAX_LIST_VARS;i++){
        list_vars[i]=NULL;
    }
}
pylist* find_list_var(char name){ 
    if(name>='L'&&name<='O'){
        return list_vars[name-'L'];
    }
    return NULL;
}

void set_list_var(char name, pylist* l) {
    if (name >= 'L' && name <= 'O') {
        if (list_vars[name - 'L'])
            pylist_del(list_vars[name - 'L']);

        list_vars[name - 'L'] = l;
    }
}

pydict* dict_vars[MAX_DICT_VARS];
void init_dict_vars(){
    for(int i=0;i<MAX_DICT_VARS;i++){
        dict_vars[i] = NULL;
    }
}
pydict* find_dict_var(char name){
    if(name >= 'D' && name <= 'G'){
        return dict_vars[name-'D'];
    }
    return NULL;
}
void set_dict_var(char name,pydict* d){
    if(name>='D'&&name<='G'){
        if(dict_vars[name-'D']) pydict_del(dict_vars[name-'D']);
        dict_vars[name-'D']=d;
    }
}

struct Map* map_vars[MAX_MAP_VARS];
void init_map_vars(){ 
    for(int i=0;i<MAX_MAP_VARS;i++){
        map_vars[i]=NULL;
    }
}

int evaluate_expression(char* expr) {
    if (!expr || strlen(expr) == 0){
        return 0;
    }
    if (strlen(expr) >= MAX_LINE) {
        fprintf(stderr, "Expression too long\n");
        return 0;
    }
    
    char buffer[MAX_LINE];
    strncpy(buffer, expr, MAX_LINE - 1);
    buffer[MAX_LINE - 1] = '\0';

    // Tokenize
    char* tokens[MAX_TOKENS];
    int tcount = 0;
    char* tok = strtok(buffer, " ");
    while (tok && tcount < MAX_TOKENS) {
        tokens[tcount++] = tok;
        tok = strtok(NULL, " ");
    }

    if (tcount == 0) return 0;
    if (tcount % 2 == 0) {
        fprintf(stderr, "Syntax error: incomplete expression\n");
        return 0;
    }

    // Parse values
    int values[MAX_TOKENS];
    char ops[MAX_TOKENS][4];
    for (int i = 0; i < tcount; i++) {
        if (i % 2 == 0) values[i/2] = get_value(tokens[i]);
        else strncpy(ops[i/2], tokens[i], 3);
    }
    int vcount = (tcount + 1) / 2;
    int opcount = tcount / 2;

    // Handle *, /, %
    for (int i = 0; i < opcount; i++) {
        if (strcmp(ops[i], "*") == 0 || strcmp(ops[i], "/") == 0 || strcmp(ops[i], "mod") == 0) {
            int res = 0;
            if (strcmp(ops[i], "*") == 0){
                if (!check_integer_overflow(values[i], values[i + 1], '*')) {
                    fprintf(stderr, "Security error: integer overflow\n");
                    return 0;
                }
                res = values[i] * values[i + 1];
            }
            else if (strcmp(ops[i], "/") == 0) {
                if (!check_division(values[i + 1])) {
                    fprintf(stderr, "Security error: division by zero\n");
                    return 0;
                }
                res = values[i] / values[i + 1];
            }
            else res = values[i+1] ? values[i] % values[i+1] : 0;

            values[i] = res;
            for (int j = i + 1; j < vcount - 1; j++){
                values[j] = values[j+1];
            }
            for (int j = i; j < opcount - 1; j++) {
                strncpy(ops[j], ops[j+1], 3);
                ops[j][3] = '\0';
            }
            vcount--; opcount--; i--;
        }
    }
    // Handle +, -
    int res = values[0];
    for (int i = 0; i < opcount; i++) {
        if (strcmp(ops[i], "+") == 0) {
            if (!check_integer_overflow(values[i], values[i + 1], '+')) {
                fprintf(stderr, "Security error: integer overflow\n");
                return 0;
            }
            res = values[i] + values[i + 1];
        }
        else if (strcmp(ops[i], "-") == 0){
            if (!check_integer_overflow(values[i], values[i + 1], '-')) {
                fprintf(stderr, "Security error: integer overflow\n");
                return 0;
            }
            res = values[i] - values[i + 1];
        }
    }
    return res;
}

int evaluate_condition(char* cond){    
    int nops = 6;
    char* ops[] = {">=", "<=", "==", "!=", ">", "<"};
    char* found = NULL;
    int lenop = 0;

    for(int i = 0; i < nops; i++) {
        char* pos = strstr(cond, ops[i]);
        if(pos && (int)strlen(ops[i]) > lenop) {  
            found = ops[i];
            lenop = strlen(ops[i]);
        }
    }
        
    if(!found){
        return 0;
    }
    
    char left[MAX_LINE], right[MAX_LINE]; 
    strncpy(left, cond, MAX_LINE); 
    left[MAX_LINE-1] = 0;
    char* pos = strstr(left, found); 
    *pos = 0; 
    strncpy(right, pos+lenop, MAX_LINE - 1);
    right[MAX_LINE - 1] = '\0';
    
    int lval = evaluate_expression(left), rval = evaluate_expression(right);
 
    if(strcmp(found,"==")==0) return lval==rval;
    else if(strcmp(found,"!=")==0) return lval!=rval;
    else if(strcmp(found,">")==0) return lval>rval;
    else if(strcmp(found,"<")==0) return lval<rval;
    else if(strcmp(found,">=")==0) return lval>=rval;
    else if(strcmp(found,"<=")==0) return lval<=rval;
    return 0;
}

int count_leading_spaces(char* line){
    int cnt=0; 
    while(*line==' '|| *line=='\t'){ 
        cnt++; 
        line++; 
    }
    return cnt; 
    }

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
   
    if (eq && eq != trimmed && *(eq+1) != '=' && *(eq-1) != '!' && *(eq-1) != '<' && *(eq-1) != '>') {
        char var_name = trimmed[0];
        if (!isalpha(var_name)) {
            fprintf(stderr, "Error: invalid variable name '%c'\n", var_name);
            return;
        }
        char* val_str = eq + 1;
        while(*val_str == ' ') val_str++;
        TypedVariable* existing = find_var(var_name);
        if (!existing) {
            existing = &vars[var_count++];
            existing->name = var_name;
        }
       
        if (val_str[0] == '\"' || val_str[0] == '\'') {
            char quote = val_str[0];
            char* start = val_str + 1;
            char* end = strchr(start, quote); 
            if (end) {
                int len = end - start;
                if (!check_string_length(len)) {
                    fprintf(stderr, "Security error: string exceeds maximum length\n");
                    return;
                }
                char temp[MAX_LINE];
                strncpy(temp, start, len);
                temp[len] = '\0';
                pystr* str_obj = (pystr*)pystr_new_from(temp);
                if (str_obj) {
                    set_str_var(var_name, str_obj);
                }
            } 
            else {
                fprintf(stderr, "Error: unclosed string\n");
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
                    pylist_append(l, pyint_new(atoi(token)));
                    token = strtok(NULL, ", ");
                }
            }
            set_list_var(var_name, l); 
        }
        else if (val_str[0] == '{') {
            pydict* d = pydict_new();
            char* start = val_str + 1;
            char* end = strchr(start, '}');
            
            if (end) {
                char content[MAX_LINE];
                strncpy(content, start, end - start);
                content[end - start] = '\0';
                
                if (strlen(content) > 0) {
                    char* pair = strtok(content, ",");
                    while (pair) {
                        while (*pair == ' ' || *pair == '\t') pair++;
                        
                        char* colon = strchr(pair, ':');
                        if (colon) {
                            char key[MAX_LINE];
                            char* key_start = pair;
                            while (*key_start == ' ' || *key_start == '\t') key_start++;
                            
                            int key_len = colon - key_start;
                            strncpy(key, key_start, key_len);
                            key[key_len] = '\0';
                            
                            if (key[0] == '"' || key[0] == '\'') {
                                memmove(key, key + 1, strlen(key));
                                key[strlen(key) - 1] = '\0';
                            }
                            
                            char* val_start = colon + 1;
                            while (*val_start == ' ' || *val_start == '\t') val_start++;
                            
                            int val = atoi(val_start);
                            pydict_set(d, key, pyint_new(val));
                        }
                        
                        pair = strtok(NULL, ",");
                    }
                }
            }
            set_dict_var(var_name, d);
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
                if(l) {pylist_append(l, pyint_new(evaluate_expression(arg))); return;}
                pystr* s = find_str_var(obj);
                if(s) {
                    if((arg[0] == '\'' || arg[0] == '\"') && strlen(arg) >= 2) 
                        pystr_append_char((PyObject*)s, arg[1]);
                    else 
                        pystr_append_char((PyObject*)s, (char)evaluate_expression(arg));
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
            pystr* s = find_str_var(content[0]); 
            if(s){ 
                pystr_print((PyObject*)s);
                return; 
            }
            pylist* l = find_list_var(content[0]); 
            if(l){ 
                pylist_print(l); 
                return; 
            }         
            pydict* d = find_dict_var(content[0]);
            if(d){
                pydict_print(d);
                return;
            }
           
            printf("%d\n", get_value(content));
        } else {
            printf("%d\n", evaluate_expression(content));
        }
        return;
    }
    // String concatenation
    else if (strchr(trimmed, '=') && strchr(trimmed, '+')) {
        char var_name = trimmed[0];
        if(var_name < 'A' || var_name > 'Z'){
            fprintf(stderr, "Error: invalid variable name\n");
            return;
        }
        char* val_str = strchr(trimmed, '=') + 1;
        while (*val_str == ' '){
            val_str++;
        }
        char* plus_pos = strchr(val_str, '+');
        if (plus_pos) {
            // Find first variable before +
            char* left_start = val_str;
            while (*left_start == ' '){
                left_start++;
            }
            
            // Find first variable after +
            char* right_start = plus_pos + 1;
            while (*right_start == ' '){
                right_start++;
            }
            pystr* s1 = find_str_var(left_start[0]);
            pystr* s2 = find_str_var(right_start[0]);
            
            if (s1 && s2) {
                PyObject* result = pystr_concat((PyObject*)s1, (PyObject*)s2);
                set_str_var(var_name, (pystr*)result);
                return;
            }
        }
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
        if (strlen(trimmed) == 0){
            continue;
        }

        if (strncmp(trimmed, "if ", 3) == 0) {
            char* cond_start = trimmed + 3;
            char cond[MAX_LINE];
            strncpy(cond, cond_start, MAX_LINE - 1);
            cond[MAX_LINE - 1] = '\0';
            
            int len = strlen(cond);
            while (len > 0 && (cond[len-1] == ':' || cond[len-1] == ' ' || cond[len-1] == '\t')) {
                cond[--len] = '\0';
            }
            
            int cond_val = evaluate_condition(cond);
            char if_block[MAX_LINE * 10] = "";
            int j = i + 1, block_indent = -1;
            
            while (j < nlines) {
                int ind = count_leading_spaces(lines[j]);
                if (strlen(lines[j] + ind) == 0) { 
                    j++; 
                    continue; 
                }
                if (block_indent == -1){
                    block_indent = ind;
                }
                if (ind < block_indent){
                    break;
                }
                int current_len = strlen(if_block);
                int line_len = strlen(lines[j] + block_indent);
                if (current_len + line_len + 2 > MAX_BLOCK_SIZE) {
                    fprintf(stderr, "Error: block too large\n");
                    break;
                }
                strcat(if_block, lines[j] + block_indent);
                strcat(if_block, "\n");
                j++;
            }

            if (cond_val){
                execute_statements(if_block);
            }
            
            if (j < nlines) {
                char* next_trimmed = lines[j] + count_leading_spaces(lines[j]);
                if (strncmp(next_trimmed, "else", 4) == 0) {
                    char else_block[MAX_LINE * 10] = "";
                    int k = j + 1, else_indent = -1;
                    while (k < nlines) {
                        int ind = count_leading_spaces(lines[k]);
                        if (strlen(lines[k] + ind) == 0){
                            k++; continue; 
                        }
                        if (else_indent == -1){
                            else_indent = ind;
                        }
                        if (ind < else_indent){
                            break;
                        }
                        
                        int current_len = strlen(else_block);
                        int line_len = strlen(lines[k] + else_indent);
                        if (current_len + line_len + 2 > MAX_BLOCK_SIZE) {
                            fprintf(stderr, "Error: else block too large\n");
                            break;
                        }
                        strcat(else_block, lines[k] + else_indent);
                        strcat(else_block, "\n");
                        k++;
                    }
                    if (!cond_val){
                        execute_statements(else_block);
                    }
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
                
                int current_len = strlen(block);
                int line_len = strlen(lines[j] + block_indent);
                if (current_len + line_len + 2 > MAX_BLOCK_SIZE) {
                    fprintf(stderr, "Error: while block too large\n");
                    break;
                }
                strcat(block, lines[j] + block_indent);
                strcat(block, "\n");
                j++;
            }
            
            while (evaluate_condition(cond_str)) {
                execute_statements(block); 
            }
            
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
                    
                    int current_len = strlen(block);
                    int line_len = strlen(lines[j] + block_indent);
                    if (current_len + line_len + 2 > MAX_BLOCK_SIZE) {
                        fprintf(stderr, "Error: for block too large\n");
                        break;
                    }
                    strcat(block, lines[j] + block_indent);
                    strcat(block, "\n");
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

void test_security(){
    int result = evaluate_expression("1 / 0");
    printf("1 / 0 = %d (should be 0)\n", result);
    
    int long_result = evaluate_expression("10 + 20 * 3 - 5 / 2");
    printf("10 + 20 * 3 - 5 / 2 = %d\n", long_result);
    
    pystr* test_str = (pystr*)pystr_new_from("hello");
    pystr_print((PyObject*)test_str);
    pystr_del((PyObject*)test_str);
}