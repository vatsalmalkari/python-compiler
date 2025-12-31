#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
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

typedef struct {
    char name;
    int value;
} Variable;

extern Variable vars[MAX_VARS];
extern int var_count;

Variable* find_var(char name);
void set_var(char name, int value);
int get_value(char* token);

void init_str_vars();
pystr* find_str_var(char name);
void set_str_var(char name, pystr* s);

void init_list_vars();
pylist* find_list_var(char name);
void set_list_var(char name, pylist* l);

void init_dict_vars();
pydict* find_dict_var(char name);
void set_dict_var(char name, pydict* d);

void init_map_vars();
struct Map* find_map_var(char name);
void set_map_var(char name, struct Map* m);

int evaluate_expression(char* expr);
int evaluate_condition(char* cond);

int count_leading_spaces(char* line);

void interpret(char* line);
void execute_statements(char* stmts);

#endif 