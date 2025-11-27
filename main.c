#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"
#include "point.h"
#include "strings.h"
#include "pydict.h"
#include "map.h"
#define MAX_VARS 26
#define MAX_LINE 100
#define MAX_TOKENS 50
#define MAX_STR_VARS 26
#define MAX_LIST_VARS 10
#define MAX_DICT_VARS 10
#define MAX_MAP_VARS 10
pylist *list_vars[MAX_LIST_VARS];
pydict* dict_vars[MAX_DICT_VARS];
pystr *str_vars[MAX_STR_VARS];
pystr *str_vars[MAX_STR_VARS];
struct Map* map_vars[MAX_MAP_VARS];


pystr *find_str_var(char name)
{
    if (name >= 'A' && name <= 'Z')
    {
        int index = name - 'A';
        if (index < MAX_STR_VARS && str_vars[index] != NULL)
        {
            return str_vars[index];
        }
    }
    return NULL;
}

void set_str_var(char name, pystr *value)
{
    if (name >= 'A' && name <= 'Z')
    {
        int index = name - 'A';
        if (index < MAX_STR_VARS)
        {
            if (str_vars[index] != NULL)
            {
                pystr_del(str_vars[index]);
            }
            str_vars[index] = value;
        }
    }
}

void init_str_vars()
{
    for (int i = 0; i < MAX_STR_VARS; i++)
    {
        str_vars[i] = NULL;
    }
}

void init_list_vars()
{
    for (int i = 0; i < MAX_LIST_VARS; i++)
    {
        list_vars[i] = NULL;
    }
}

pylist *find_list_var(char name)
{
    if (name >= 'L' && name <= 'O')
    {
        int index = name - 'L';
        if (index < MAX_LIST_VARS && list_vars[index] != NULL)
        {
            return list_vars[index];
        }
    }
    return NULL;
}

void set_list_var(char name, pylist *value)
{
    if (name >= 'L' && name <= 'O')
    {
        int index = name - 'L';
        if (index < MAX_LIST_VARS)
        {
            if (list_vars[index] != NULL)
            {
                pylist_del(list_vars[index]);
            }
            list_vars[index] = value;
        }
    }
}

void init_dict_vars() {
    for (int i = 0; i < MAX_DICT_VARS; i++) {
        dict_vars[i] = NULL;
    }
}

pydict* find_dict_var(char name) {
    if (name >= 'D' && name <= 'G') {
        int index = name - 'D';
        if (index < MAX_DICT_VARS && dict_vars[index] != NULL) {
            return dict_vars[index];
        }
    }
    return NULL;
}

void set_dict_var(char name, pydict* value) {
    if (name >= 'D' && name <= 'G') {
        int index = name - 'D';
        if (index < MAX_DICT_VARS) {
            if (dict_vars[index] != NULL) {
                pydict_del(dict_vars[index]);
            }
            dict_vars[index] = value;
        }
    }
}

void init_map_vars() {
    for (int i = 0; i < MAX_MAP_VARS; i++) {
        map_vars[i] = NULL;
    }
}

struct Map* find_map_var(char name) {
    if (name >= 'M' && name <= 'P') {  
        int index = name - 'M';
        if (index < MAX_MAP_VARS && map_vars[index] != NULL) {
            return map_vars[index];
        }
    }
    return NULL;
}

void set_map_var(char name, struct Map* value) {
    if (name >= 'M' && name <= 'P') {
        int index = name - 'M';
        if (index < MAX_MAP_VARS) {
            if (map_vars[index] != NULL) {
                map_vars[index]->del(map_vars[index]);
            }
            map_vars[index] = value;
        }
    }
}


typedef struct
{
    char name;
    int value;
} Variable;

Variable vars[MAX_VARS];
int var_count = 0;

Variable *find_var(char name);
void set_var(char name, int value);
int get_value(char *token);
void safe_strcpy(char *dest, const char *src, size_t dest_size);
int evaluate_expression(char *expr);
int evaluate_condition(char *cond);
void interpret(char *line);
void execute_statement(char *stmt);

Variable *find_var(char name)
{
    for (int i = 0; i < var_count; i++)
    {
        if (vars[i].name == name)
            return &vars[i];
    }
    return NULL;
}

void set_var(char name, int value)
{
    Variable *var = find_var(name);
    if (var)
    {
        var->value = value;
    }
    else if (var_count < MAX_VARS)
    {
        vars[var_count].name = name;
        vars[var_count].value = value;
        var_count++;
    }
}

int get_value(char *token)
{
    if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
    {
        return atoi(token);
    }
    else if (isalpha(token[0]))
    {
        Variable *var = find_var(token[0]);
        return var ? var->value : 0;
    }
    return 0;
}

void safe_strcpy(char *dest, const char *src, size_t dest_size)
{
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
}

void tokenize_expression(char *expr, char *tokens[], int *token_count)
{
    char expr_copy[MAX_LINE];
    safe_strcpy(expr_copy, expr, MAX_LINE);

    *token_count = 0;
    char *current = expr_copy;

    while (*current != '\0' && *token_count < MAX_TOKENS)
    {
        while (*current == ' ')
            current++;
        if (*current == '\0')
            break;

        if (isdigit(*current) || (*current == '-' && isdigit(*(current + 1))))
        {
            char *start = current;
            while (isdigit(*current) || (*current == '-' && current == start))
                current++;
            int len = current - start;

            tokens[*token_count] = malloc(len + 1);
            strncpy(tokens[*token_count], start, len);
            tokens[*token_count][len] = '\0';
            (*token_count)++;
        }
        else if (isalpha(*current))
        {
            char *start = current;
            while (isalpha(*current))
                current++;
            int len = current - start;

            tokens[*token_count] = malloc(len + 1);
            strncpy(tokens[*token_count], start, len);
            tokens[*token_count][len] = '\0';
            (*token_count)++;
        }
        else if (strchr("+-*/", *current) || (strncmp(current, "mod", 3) == 0 && !isalnum(current[3])))
        {
            if (strncmp(current, "mod", 3) == 0 && !isalnum(current[3]))
            {
                tokens[*token_count] = malloc(4);
                strcpy(tokens[*token_count], "mod");
                (*token_count)++;
                current += 3;
            }
            else
            {
                tokens[*token_count] = malloc(2);
                tokens[*token_count][0] = *current;
                tokens[*token_count][1] = '\0';
                (*token_count)++;
                current++;
            }
        }
        else
        {
            current++;
        }
    }
}

int evaluate_expression(char *expr)
{
    char *tokens[MAX_TOKENS];
    int token_count = 0;

    tokenize_expression(expr, tokens, &token_count);

    if (token_count == 0)
        return 0;

    int values[MAX_TOKENS];
    char operators[MAX_TOKENS][4];
    int value_count = 0;
    int op_count = 0;

    values[value_count++] = get_value(tokens[0]);

    for (int i = 1; i < token_count; i += 2)
    {
        if (i + 1 < token_count)
        {
            strcpy(operators[op_count], tokens[i]);
            op_count++;
            values[value_count++] = get_value(tokens[i + 1]);
        }
    }

    for (int i = 0; i < op_count; i++)
    {
        if (strcmp(operators[i], "*") == 0 || strcmp(operators[i], "/") == 0 || strcmp(operators[i], "mod") == 0)
        {
            int left = values[i];
            int right = values[i + 1];
            int result;

            if (strcmp(operators[i], "*") == 0)
            {
                result = left * right;
            }
            else if (strcmp(operators[i], "/") == 0)
            {
                if (right == 0)
                    return 0;
                result = left / right;
            }
            else if (strcmp(operators[i], "mod") == 0)
            {
                if (right == 0)
                    return 0;
                result = left % right;
            }

            values[i] = result;
            for (int j = i + 1; j < value_count - 1; j++)
            {
                values[j] = values[j + 1];
            }
            for (int j = i; j < op_count - 1; j++)
            {
                strcpy(operators[j], operators[j + 1]);
            }
            value_count--;
            op_count--;
            i--;
        }
    }

    int result = values[0];
    for (int i = 0; i < op_count; i++)
    {
        if (strcmp(operators[i], "+") == 0)
        {
            result += values[i + 1];
        }
        else if (strcmp(operators[i], "-") == 0)
        {
            result -= values[i + 1];
        }
    }

    for (int i = 0; i < token_count; i++)
        free(tokens[i]);

    return result;
}

int evaluate_condition(char *cond)
{
    char cond_copy[MAX_LINE];
    safe_strcpy(cond_copy, cond, MAX_LINE);

    char *operators[] = {">=", "<=", "==", "!=", ">", "<"};
    int num_ops = 6;
    char *found_op = NULL;
    int op_len = 0;

    for (int i = 0; i < num_ops; i++)
    {
        char *op_pos = strstr(cond_copy, operators[i]);
        if (op_pos != NULL)
        {
            found_op = operators[i];
            op_len = strlen(found_op);
            break;
        }
    }

    if (found_op == NULL)
        return 0;

    char *op_pos = strstr(cond_copy, found_op);
    *op_pos = '\0';

    char *left_expr = cond_copy;
    char *right_expr = op_pos + op_len;

    char *left_trim = left_expr;
    while (*left_trim == ' ')
        left_trim++;
    int len = strlen(left_trim);
    while (len > 0 && left_trim[len - 1] == ' ')
    {
        left_trim[len - 1] = '\0';
        len--;
    }

    char *right_trim = right_expr;
    while (*right_trim == ' ')
        right_trim++;
    len = strlen(right_trim);
    while (len > 0 && right_trim[len - 1] == ' ')
    {
        right_trim[len - 1] = '\0';
        len--;
    }

    int left_val = evaluate_expression(left_trim);
    int right_val = evaluate_expression(right_trim);

    if (strcmp(found_op, "==") == 0)
    {
        return left_val == right_val;
    }
    else if (strcmp(found_op, "!=") == 0)
    {
        return left_val != right_val;
    }
    else if (strcmp(found_op, ">") == 0)
    {
        return left_val > right_val;
    }
    else if (strcmp(found_op, "<") == 0)
    {
        return left_val < right_val;
    }
    else if (strcmp(found_op, ">=") == 0)
    {
        return left_val >= right_val;
    }
    else if (strcmp(found_op, "<=") == 0)
    {
        return left_val <= right_val;
    }

    return 0;
}

void execute_statements(char *stmts)
{
    char stmts_copy[MAX_LINE];
    safe_strcpy(stmts_copy, stmts, MAX_LINE);

    char *current = stmts_copy;
    while (*current != '\0')
    {
        char *semicolon = strchr(current, ';');
        char *next_stmt;

        if (semicolon != NULL)
        {
            *semicolon = '\0';
            next_stmt = semicolon + 1;
        }
        else
        {
            next_stmt = current + strlen(current);
        }

        char *stmt = current;
        while (*stmt == ' ')
            stmt++;
        int len = strlen(stmt);
        while (len > 0 && stmt[len - 1] == ' ')
        {
            stmt[len - 1] = '\0';
            len--;
        }

        if (strlen(stmt) > 0)
        {
            interpret(stmt);
        }

        if (semicolon == NULL)
            break;
        current = next_stmt;
    }
}

void execute_statement(char *stmt)
{
    char stmt_copy[MAX_LINE];
    safe_strcpy(stmt_copy, stmt, MAX_LINE);

    char *trimmed = stmt_copy;
    while (*trimmed == ' ')
        trimmed++;
    int len = strlen(trimmed);
    while (len > 0 && trimmed[len - 1] == ' ')
    {
        trimmed[len - 1] = '\0';
        len--;
    }

    if (strlen(trimmed) == 0)
        return;

    interpret(trimmed);
}

void interpret(char *line)
{
    char line_copy[MAX_LINE];
    safe_strcpy(line_copy, line, MAX_LINE);

    char *trimmed = line_copy;
    while (*trimmed == ' ')
        trimmed++;

    int len = strlen(trimmed);
    while (len > 0 && trimmed[len - 1] == ' ')
    {
        trimmed[len - 1] = '\0';
        len--;
    }

    if (strlen(trimmed) == 0)
        return;
    if (isalpha(trimmed[0]) && trimmed[0] >= 'A' && trimmed[0] <= 'Z')
    {
        pylist *list_var = find_list_var(trimmed[0]);
        char *equal_pos = strchr(trimmed, '=');
        if (equal_pos && !strstr(trimmed, "=="))
        {
            char var_name = trimmed[0];
            char *value_start = equal_pos + 1;

            while (*value_start == ' ')
                value_start++;

            if (strncmp(value_start, "[]", 2) == 0)
            {
                pylist *new_list = pylist_new();
                set_list_var(var_name, new_list);
                return;
            }
        }

        if (list_var != NULL)
        {
            char *append_pos = strstr(trimmed, ".append");
            if (append_pos)
            {
                char *paren_start = strchr(append_pos, '(');
                if (paren_start)
                {
                    char *paren_end = strchr(paren_start, ')');
                    if (paren_end)
                    {
                        char *quote_start = NULL;
                        char *quote_end = NULL;

                        for (char *p = paren_start; p < paren_end; p++)
                        {
                            if (*p == '"' || *p == '\'')
                            {
                                if (!quote_start)
                                {
                                    quote_start = p;
                                }
                                else
                                {
                                    quote_end = p;
                                    break;
                                }
                            }
                        }

                        if (quote_start && quote_end && quote_end > quote_start)
                        {
                            char *content_start = quote_start + 1;
                            int content_len = quote_end - content_start;
                            if (content_len > 0)
                            {
                                char *content = malloc(content_len + 1);
                                strncpy(content, content_start, content_len);
                                content[content_len] = '\0';

                                list_var->append(list_var, content);
                                free(content);
                                return;
                            }
                        }
                    }
                }
            }

            char *print_pos = strstr(trimmed, ".print");
            if (print_pos)
            {
                char *paren_start = strchr(print_pos, '(');
                if (paren_start && strncmp(paren_start, "()", 2) == 0)
                {
                    list_var->print(list_var);
                    return;
                }
            }

            char *len_pos = strstr(trimmed, ".len");
            if (len_pos)
            {
                char *paren_start = strchr(len_pos, '(');
                if (paren_start && strncmp(paren_start, "()", 2) == 0)
                {
                    printf("%d\n", list_var->len(list_var));
                    return;
                }
            }

            char *index_pos = strstr(trimmed, ".index");
            if (index_pos)
            {
                char *paren_start = strchr(index_pos, '(');
                if (paren_start)
                {
                    char *paren_end = strchr(paren_start, ')');
                    if (paren_end)
                    {
                        char *quote_start = NULL;
                        char *quote_end = NULL;

                        for (char *p = paren_start; p < paren_end; p++)
                        {
                            if (*p == '"' || *p == '\'')
                            {
                                if (!quote_start)
                                {
                                    quote_start = p;
                                }
                                else
                                {
                                    quote_end = p;
                                    break;
                                }
                            }
                        }

                        if (quote_start && quote_end && quote_end > quote_start)
                        {
                            char *content_start = quote_start + 1;
                            int content_len = quote_end - content_start;
                            if (content_len > 0)
                            {
                                char *content = malloc(content_len + 1);
                                strncpy(content, content_start, content_len);
                                content[content_len] = '\0';

                                int idx = list_var->index(list_var, content);
                                printf("%d\n", idx);
                                free(content);
                                return;
                            }
                        }
                    }
                }
            }
            char *get_pos = strstr(trimmed, ".get(");
            if (get_pos)
            {
                if (list_var == NULL)
                {
                    printf("Error: List variable '%c' not found\n", trimmed[0]);
                    return;
                }

                if (list_var->get == NULL)
                {
                    printf("Error: List get method not available\n");
                    return;
                }

                char *paren_start = get_pos + 5;
                char *paren_end = strchr(paren_start, ')');
                if (!paren_end)
                {
                    printf("Error: Missing closing parenthesis\n");
                    return;
                }

                char index_str[10];
                int index_len = paren_end - paren_start;
                if (index_len <= 0 || index_len >= 10)
                {
                    printf("Error: Invalid index format\n");
                    return;
                }

                strncpy(index_str, paren_start, index_len);
                index_str[index_len] = '\0';

                int index = atoi(index_str);

                char *result = list_var->get(list_var, index);
                if (result != NULL)
                {
                    printf("'%s'\n", result);
                }
                else
                {
                    printf("IndexError: index %d out of range\n", index);
                }
                return;
            }

            char *set_pos = strstr(trimmed, ".set(");
            if (set_pos)
            {
                if (list_var == NULL || list_var->set == NULL)
                {
                    printf("Error: Invalid list or set method\n");
                    return;
                }

                char *paren_end = strchr(set_pos + 5, ')');
                if (!paren_end)
                {
                    printf("Error: Missing closing parenthesis\n");
                    return;
                }

                int len = paren_end - (set_pos + 5);
                char content[len + 1];
                strncpy(content, set_pos + 5, len);
                content[len] = '\0';

                char *comma = strchr(content, ',');
                if (!comma)
                {
                    printf("Error: Missing comma in set method\n");
                    return;
                }

                *comma = '\0';
                int index = atoi(content);

                char *value = comma + 1;
                while (*value == ' ')
                    value++;

                if ((value[0] == '"' && value[strlen(value) - 1] == '"') ||
                    (value[0] == '\'' && value[strlen(value) - 1] == '\''))
                {
                    value[strlen(value) - 1] = '\0';
                    value++;
                }

                list_var->set(list_var, index, value);
                return;
            }
        }
    }
if (isalpha(trimmed[0]) && trimmed[0] >= 'D' && trimmed[0] <= 'G') {
    pydict* dict_var = find_dict_var(trimmed[0]);
    
    char* equal_pos = strchr(trimmed, '=');
    if (equal_pos && !strstr(trimmed, "==")) {
        char var_name = trimmed[0];
        char* value_start = equal_pos + 1;
        
        while (*value_start == ' ') value_start++;
        
        if (strncmp(value_start, "{}", 2) == 0) {
            pydict* new_dict = pydict_new();
            set_dict_var(var_name, new_dict);
            return;
        }
    }
    
    if (dict_var != NULL) {

        char* put_pos = strstr(trimmed, ".put(");
        if (put_pos) {
            char* paren_end = strchr(put_pos, ')');
            if (paren_end) {
                int len = paren_end - (put_pos + 5);
                char content[len + 1];
                strncpy(content, put_pos + 5, len);
                content[len] = '\0';
                
                char* comma = strchr(content, ',');
                if (comma) {
                    *comma = '\0';
                    char* key = content;
                    char* value = comma + 1;
                    
                    while (*key == ' ' || *key == '"' || *key == '\'') key++;
                    int key_len = strlen(key);
                    while (key_len > 0 && (key[key_len-1] == ' ' || key[key_len-1] == '"' || key[key_len-1] == '\'')) {
                        key[key_len-1] = '\0';
                        key_len--;
                    }
                    
                    while (*value == ' ' || *value == '"' || *value == '\'') value++;
                    int val_len = strlen(value);
                    while (val_len > 0 && (value[val_len-1] == ' ' || value[val_len-1] == '"' || value[val_len-1] == '\'')) {
                        value[val_len-1] = '\0';
                        val_len--;
                    }
                    
                    pydict_put(dict_var, key, value);
                    return;
                }
            }
        }
        
        char* get_pos = strstr(trimmed, ".get(");
        if (get_pos) {
            char* paren_end = strchr(get_pos, ')');
            if (paren_end) {
                int len = paren_end - (get_pos + 5);
                char key[len + 1];
                strncpy(key, get_pos + 5, len);
                key[len] = '\0';
                
                char* key_trim = key;
                while (*key_trim == ' ' || *key_trim == '"' || *key_trim == '\'') key_trim++;
                int key_len = strlen(key_trim);
                while (key_len > 0 && (key_trim[key_len-1] == ' ' || key_trim[key_len-1] == '"' || key_trim[key_len-1] == '\'')) {
                    key_trim[key_len-1] = '\0';
                    key_len--;
                }
                
                char* result = pydict_get(dict_var, key_trim, "None");
                printf("'%s'\n", result);
                return;
            }
        }
        
        if (strstr(trimmed, ".print()")) {
            pydict_print(dict_var);
            return;
        }
        
        if (strstr(trimmed, ".len()")) {
            printf("%d\n", pydict_len(dict_var));
            return;
        }
    }
}

if (isalpha(trimmed[0]) && trimmed[0] >= 'M' && trimmed[0] <= 'P') {
    struct Map* map_var = find_map_var(trimmed[0]);
    
    char* equal_pos = strchr(trimmed, '=');
    if (equal_pos && !strstr(trimmed, "==")) {
        char var_name = trimmed[0];
        char* value_start = equal_pos + 1;
        
        while (*value_start == ' ') value_start++;
        
        if (strncmp(value_start, "Map()", 5) == 0) {
            struct Map* new_map = Map_new();
            set_map_var(var_name, new_map);
            return;
        }
    }
    
    if (map_var != NULL) {
        char* put_pos = strstr(trimmed, ".put(");
        if (put_pos) {
            char* paren_end = strchr(put_pos, ')');
            if (paren_end) {
                int len = paren_end - (put_pos + 5);
                char content[len + 1];
                strncpy(content, put_pos + 5, len);
                content[len] = '\0';
                
                char* comma = strchr(content, ',');
                if (comma) {
                    *comma = '\0';
                    char* key = content;
                    char* value_str = comma + 1;
                    
                    while (*key == ' ' || *key == '"' || *key == '\'') key++;
                    int key_len = strlen(key);
                    while (key_len > 0 && (key[key_len-1] == ' ' || key[key_len-1] == '"' || key[key_len-1] == '\'')) {
                        key[key_len-1] = '\0';
                        key_len--;
                    }
                    
                    while (*value_str == ' ') value_str++;
                    int value = atoi(value_str);
                    
                    map_var->put(map_var, key, value);
                    return;
                }
            }
        }
        
        char* get_pos = strstr(trimmed, ".get(");
        if (get_pos) {
            char* paren_end = strchr(get_pos, ')');
            if (paren_end) {
                int len = paren_end - (get_pos + 5);
                char content[len + 1];
                strncpy(content, get_pos + 5, len);
                content[len] = '\0';
                
                char* comma = strchr(content, ',');
                int default_value = -1;
                char* key = content;
                
                if (comma) {
                    *comma = '\0';
                    key = content;
                    char* default_str = comma + 1;
                    while (*default_str == ' ') default_str++;
                    default_value = atoi(default_str);
                }
                
                char* key_trim = key;
                while (*key_trim == ' ' || *key_trim == '"' || *key_trim == '\'') key_trim++;
                int key_len = strlen(key_trim);
                while (key_len > 0 && (key_trim[key_len-1] == ' ' || key_trim[key_len-1] == '"' || key_trim[key_len-1] == '\'')) {
                    key_trim[key_len-1] = '\0';
                    key_len--;
                }
                
                int result = map_var->get(map_var, key_trim, default_value);
                printf("%d\n", result);
                return;
            }
        }
        
        if (strstr(trimmed, ".size()")) {
            printf("%d\n", map_var->size(map_var));
            return;
        }
        
        if (strstr(trimmed, ".dump()")) {
            map_var->dump(map_var);
            return;
        }
        
        if (strstr(trimmed, ".del()")) {
            map_var->del(map_var);
            set_map_var(trimmed[0], NULL);
            return;
        }
    }
}

    if (strncmp(trimmed, "while", 5) == 0 && (trimmed[5] == ' ' || trimmed[5] == '('))
    {
        char *condition_start = trimmed + 5;
        while (*condition_start == ' ')
            condition_start++;

        char *colon = strchr(condition_start, ':');
        if (colon == NULL)
            return;

        char condition[MAX_LINE];
        safe_strcpy(condition, condition_start, colon - condition_start);

        char clean_condition[MAX_LINE];
        int j = 0;
        for (int i = 0; condition[i] != '\0'; i++)
        {
            if (condition[i] != '(' && condition[i] != ')')
            {
                clean_condition[j++] = condition[i];
            }
        }
        clean_condition[j] = '\0';

        char *cond_trim = clean_condition;
        while (*cond_trim == ' ')
            cond_trim++;
        len = strlen(cond_trim);
        while (len > 0 && cond_trim[len - 1] == ' ')
        {
            cond_trim[len - 1] = '\0';
            len--;
        }

        char *loop_body = colon + 1;
        while (*loop_body == ' ')
            loop_body++;

        while (evaluate_condition(cond_trim))
        {
            execute_statements(loop_body);
        }
        return;
    }

    if (strncmp(trimmed, "for", 3) == 0 && (trimmed[3] == ' ' || trimmed[3] == '('))
    {
        char *content = trimmed + 3;
        while (*content == ' ')
            content++;

        char *colon = strchr(content, ':');
        if (colon == NULL)
            return;

        *colon = '\0';
        char *loop_header = content;
        char *loop_body = colon + 1;
        while (*loop_body == ' ')
            loop_body++;

        char var_name;
        int end_value;

        char *var_start = loop_header;
        while (*var_start == ' ')
            var_start++;
        var_name = *var_start;

        char *range_start = strstr(loop_header, "in range(");
        if (range_start == NULL)
            return;

        char *value_start = range_start + 9;
        char *value_end = strchr(value_start, ')');
        if (value_end == NULL)
            return;
        *value_end = '\0';

        end_value = evaluate_expression(value_start);

        for (int i = 0; i < end_value; i++)
        {
            set_var(var_name, i);
            execute_statements(loop_body);
        }
        return;
    }

    if (strncmp(trimmed, "if", 2) == 0 && (trimmed[2] == ' ' || trimmed[2] == '('))
    {
        char *condition_start = trimmed + 2;
        while (*condition_start == ' ')
            condition_start++;

        char *colon = strchr(condition_start, ':');
        if (colon == NULL)
            return;

        char condition[MAX_LINE];
        safe_strcpy(condition, condition_start, colon - condition_start);

        char clean_condition[MAX_LINE];
        int j = 0;
        for (int i = 0; condition[i] != '\0'; i++)
        {
            if (condition[i] != '(' && condition[i] != ')')
            {
                clean_condition[j++] = condition[i];
            }
        }
        clean_condition[j] = '\0';

        char *cond_trim = clean_condition;
        while (*cond_trim == ' ')
            cond_trim++;
        len = strlen(cond_trim);
        while (len > 0 && cond_trim[len - 1] == ' ')
        {
            cond_trim[len - 1] = '\0';
            len--;
        }

        char *then_part = colon + 1;
        char *else_pos = strstr(then_part, "else");
        char *else_part = NULL;

        if (else_pos != NULL)
        {
            *else_pos = '\0';
            else_part = else_pos + 4;
            if (*else_part == ':')
                else_part++;
        }

        char *then_trimmed = then_part;
        while (*then_trimmed == ' ')
            then_trimmed++;

        if (else_part != NULL)
        {
            char *else_trimmed = else_part;
            while (*else_trimmed == ' ')
                else_trimmed++;
            else_part = else_trimmed;
        }

        if (evaluate_condition(cond_trim))
        {
            execute_statements(then_trimmed);
        }
        else if (else_part != NULL)
        {
            execute_statements(else_part);
        }
        return;
    }

    if (isalpha(trimmed[0]) && trimmed[0] >= 'A' && trimmed[0] <= 'Z')
    {
        char *equal_pos = strchr(trimmed, '=');
        if (equal_pos && !strstr(trimmed, "=="))
        {
            char var_name = trimmed[0];
            char *value_start = equal_pos + 1;
            while (*value_start == ' ')
                value_start++;

            if (*value_start == '"' || *value_start == '\'')
            {
                char quote_char = *value_start;
                char *end_quote = strchr(value_start + 1, quote_char);
                if (end_quote)
                {
                    *end_quote = '\0';
                    char *string_content = value_start + 1;

                    pystr *new_str = pystr_new_from(string_content);
                    set_str_var(var_name, new_str);
                    return;
                }
            }
        }
    }

    if (isalpha(trimmed[0]) && trimmed[0] >= 'A' && trimmed[0] <= 'Z')
    {
        char var_name = trimmed[0];
        pystr *str_var = find_str_var(var_name);

        if (str_var != NULL)
        {
            char *append_pos = strstr(trimmed, ".append(");
            if (append_pos)
            {
                char *paren_start = append_pos + 8;
                char *paren_end = strchr(paren_start, ')');
                if (paren_end && (paren_end - paren_start == 3 || paren_end - paren_start == 4))
                {
                    *paren_end = '\0';
                    if ((paren_start[0] == '\'' && paren_start[2] == '\'') ||
                        (paren_start[0] == '"' && paren_start[2] == '"'))
                    {
                        char ch = paren_start[1];
                        str_var->append(str_var, ch);
                        return;
                    }
                }
            }

            char *appends_pos = strstr(trimmed, ".appends(");
            if (appends_pos)
            {
                char *paren_start = appends_pos + 9;
                char *paren_end = strchr(paren_start, ')');
                if (paren_end)
                {
                    *paren_end = '\0';
                    if ((paren_start[0] == '"' && paren_end[-1] == '"') ||
                        (paren_start[0] == '\'' && paren_end[-1] == '\''))
                    {
                        paren_start++;
                        paren_end[-1] = '\0';
                        str_var->appends(str_var, paren_start);
                        return;
                    }
                }
            }

            if (strstr(trimmed, ".print()"))
            {
                str_var->print(str_var);
                return;
            }

            if (strstr(trimmed, ".len()"))
            {
                int length = str_var->len(str_var);
                printf("%d\n", length);
                return;
            }
        }
    }



if (strncmp(trimmed, "print", 5) == 0) {
    char* after_print = trimmed + 5;
    
    while (*after_print == ' ') after_print++;
    
    if (*after_print == '(') {
        char* content = after_print + 1;
        char* closing_paren = strchr(content, ')');
        
        if (closing_paren) {
            *closing_paren = '\0';
            
            char* expr = content;
            while (*expr == ' ') expr++;
            int len = strlen(expr);
            while (len > 0 && expr[len-1] == ' ') {
                expr[len-1] = '\0';
                len--;
            }
            
            if (strlen(expr) == 1 && expr[0] >= 'A' && expr[0] <= 'Z') {
                pystr* str_var = find_str_var(expr[0]);
                if (str_var != NULL) {
                    str_var->print(str_var);
                    return;
                }
            }
            
            if (*expr == '"' || *expr == '\'') {
                char quote_char = *expr;
                expr++;
                char* end_quote = strchr(expr, quote_char);
                if (end_quote) {
                    *end_quote = '\0';
                    printf("%s\n", expr);
                }
            } else {
                int result = evaluate_expression(expr);
                printf("%d\n", result);
            }
            return;
        }
    }
}

    for (int i = 0; trimmed[i] != '\0'; i++)
    {
        if (trimmed[i] == '*' && trimmed[i + 1] == '=')
        {
            char var_name = trimmed[0];
            if (isalpha(var_name))
            {
                Variable *var = find_var(var_name);
                if (var)
                {
                    char *expr = trimmed + i + 2;
                    while (*expr == ' ')
                        expr++;
                    int value = evaluate_expression(expr);
                    set_var(var_name, var->value * value);
                    return;
                }
            }
        }
        else if (trimmed[i] == '/' && trimmed[i + 1] == '=')
        {
            char var_name = trimmed[0];
            if (isalpha(var_name))
            {
                Variable *var = find_var(var_name);
                if (var)
                {
                    char *expr = trimmed + i + 2;
                    while (*expr == ' ')
                        expr++;
                    int value = evaluate_expression(expr);
                    if (value != 0)
                        set_var(var_name, var->value / value);
                    return;
                }
            }
        }
        else if (strncmp(&trimmed[i], "mod", 3) == 0 && trimmed[i + 3] == '=')
        {
            char var_name = trimmed[0];
            if (isalpha(var_name))
            {
                Variable *var = find_var(var_name);
                if (var)
                {
                    char *expr = trimmed + i + 4;
                    while (*expr == ' ')
                        expr++;
                    int value = evaluate_expression(expr);
                    if (value != 0)
                        set_var(var_name, var->value % value);
                    return;
                }
            }
        }
        else if (trimmed[i] == '+' && trimmed[i + 1] == '=')
        {
            char var_name = trimmed[0];
            if (isalpha(var_name))
            {
                Variable *var = find_var(var_name);
                if (var)
                {
                    char *expr = trimmed + i + 2;
                    while (*expr == ' ')
                        expr++;
                    int value = evaluate_expression(expr);
                    set_var(var_name, var->value + value);
                    return;
                }
            }
        }
        else if (trimmed[i] == '-' && trimmed[i + 1] == '=')
        {
            char var_name = trimmed[0];
            if (isalpha(var_name))
            {
                Variable *var = find_var(var_name);
                if (var)
                {
                    char *expr = trimmed + i + 2;
                    while (*expr == ' ')
                        expr++;
                    int value = evaluate_expression(expr);
                    set_var(var_name, var->value - value);
                    return;
                }
            }
        }
    }

    char *equal_pos = strchr(trimmed, '=');
    if (equal_pos && isalpha(trimmed[0]) &&
        !strstr(trimmed, "==") &&
        !strstr(trimmed, "!=") &&
        !strstr(trimmed, ">=") &&
        !strstr(trimmed, "<="))
    {

        char var_name = trimmed[0];
        char *expr_start = equal_pos + 1;
        while (*expr_start == ' ')
            expr_start++;

        int value = evaluate_expression(expr_start);
        set_var(var_name, value);
        return;
    }

    if (isdigit(trimmed[0]) || (trimmed[0] == '-' && isdigit(trimmed[1])) ||
        (isalpha(trimmed[0]) && !strchr(trimmed, '=')))
    {
        int result = evaluate_expression(trimmed);
        printf("%d\n", result);
        return;
    }
}

int main()
{
    char line[MAX_LINE];
    printf("> ");
    init_str_vars();
    init_list_vars();
    init_dict_vars();
    init_map_vars();

    while (fgets(line, sizeof(line), stdin))
    {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "exit()") == 0)
            break;
        if (strlen(line) > 0)
            interpret(line);
        printf("> ");
    }
    for (int i = 0; i < MAX_STR_VARS; i++)
    {
        if (str_vars[i] != NULL)
        {
            pystr_del(str_vars[i]);
        }
    }
    for (int i = 0; i < MAX_LIST_VARS; i++)
    {
        if (list_vars[i] != NULL)
        {
            pylist_del(list_vars[i]);
        }
    }
    for (int i = 0; i < MAX_DICT_VARS; i++)
    {
        if (dict_vars[i] != NULL)
        {
            pydict_del(dict_vars[i]);
        }
    }
    for (int i = 0; i < MAX_MAP_VARS; i++) {
        if (map_vars[i] != NULL) {
            map_vars[i]->del(map_vars[i]);
        }
    }

    return 0;
}