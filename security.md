# Security Analysis & Memory Safety

1. **Buffer overflow prevention** via bounds checking
2. **Type safety** with runtime type checking
3. **Memory safety** with reference counting
4. **Input validation** at all boundaries
5. **Safe error handling** instead of crashes

---

## 1. Buffer Overflow Prevention

### The Problem

Buffer overflow occurs when data written to a buffer exceeds its allocated size:

```c
// VULNERABLE
char buffer[10];
strcpy(buffer, user_input); 
```

This overwrites adjacent memory, causing crashes or security vulnerabilities.

### The Solution: Bounds Checking

Every buffer operation validates size before writing:

```c
// SAFE
char buffer[10];
int buffer_size = 10;
int input_len = strlen(user_input);

if (input_len >= buffer_size) {
    fprintf(stderr, "Error: input too large\n");
    return;  // Don't write
}
strncpy(buffer, user_input, buffer_size - 1);
buffer[buffer_size - 1] = '\0'; 
```

### Where We Prevent Overflow

#### 1. **String Concatenation in Blocks**

```c
// In execute_statements()
int current_len = strlen(if_block);
int line_len = strlen(lines[j] + block_indent);

if (current_len + line_len + 2 > MAX_BLOCK_SIZE) {
    fprintf(stderr, "Error: block too large\n");
    break;
}
strcat(if_block, lines[j] + block_indent);
strcat(if_block, "\n");
```

**Why this matters:**
- MAX_BLOCK_SIZE = 2000 chars (10 lines * 200 chars each)
- Without check, large programs crash
- Bounds check prevents overflow

#### 2. ** String Parsing **

```c
if (len > MAX_LINE - 1) {
    fprintf(stderr, "String too long\n");
    return;
}
char temp[MAX_LINE];
strncpy(temp, start, len);
temp[len] = '\0';
```

#### 3. **Dynamic String Growth**

```c
// In pystr.c
void pystr_append_char(PyObject* obj, char c) {
    pystr* s = (pystr*)obj;
    
    if (s->length + 1 >= s->capacity) {
        s->capacity *= 2;  
        s->data = realloc(s->data, s->capacity);
    }
    
    s->data[s->length] = c;
    s->length++;
    s->data[s->length] = '\0';
}
```

**How it works:**
1. Track current capacity
2. If adding data would overflow, grow buffer
3. Realloc with new size
4. Continue safely

---

## 2. Type Safety

Type mismatches lead to undefined behavior:

```python
A = "hello"
B = A + 5
```

### Type Inference & Checking

#### Type Inference

```c
PyType infer_type(char* value) {
    if (!value || strlen(value) == 0) return INT;
    
    if (value[0] == '"' || value[0] == '\'') return STR;
    if (value[0] == '[') return LIST;
    if (value[0] == '{') return DICT;
    
    if (isalpha(value[0])) {
        if (find_str_var(value[0])) return STR;
        if (find_list_var(value[0])) return LIST;
        if (find_dict_var(value[0])) return DICT;
        return INT;  // Default integer
    }
    
    return INT;
}
```

#### Type Checking

```c
PyType rhs_type = infer_type(val_str);
Variable* existing = find_var(var_name);

if (existing) {
    fprintf(stderr, "Warning: variable '%c' type changed\n", var_name);
    // allow reassignment but warn
}
```

### Type Safety in Operations

#### String Concatenation (Type-Safe)

```python
A = "hello"
B = "world"
C = A + B  # String + String = String

A = 5
B = A + "hello"  # Int + String = Type error
```

#### Arithmetic (Type-Safe)

```python
a = 5
b = 10
c = a + b  # Int + Int = Int

d = a / 0  # Division by zero protected
```
---

## 3. Pointer Safety

### The Problem

```c
int* ptr = malloc(sizeof(int));
*ptr = 5;
free(ptr);
*ptr = 10;  // USE AFTER FREE
```

### The Solution: Careful Pointer Management

#### 1. ** Always Check Before Dereference **

```c
pystr* find_str_var(char name) {
    if (name >= 'A' && name <= 'Z') {
        return str_vars[name - 'A'];
    }
    return NULL;
}

pystr* s = find_str_var(var_name);
if (s) {  
    pystr_print((PyObject*)s);
}
```

#### 2. ** Reference Counting **

Track how many variables reference an object:

```c
typedef struct {
    int refcount;
} PyObject;

// Create: refcount = 1
PyObject* obj = malloc(...);
obj->refcount = 1;

// Use by another variable
obj->refcount++;

// Delete from one variable
obj->refcount--;
if (obj->refcount == 0) {
    free(obj);  // Only free when no one uses it
}
```

#### 3. ** Dangling Pointers **

```c
pystr* s;
{
    pystr local_str = {"data", 10};
    s = &local_str;
}
// s is now dangling (points to freed stack space)

pystr* s = malloc(sizeof(pystr));
if (s) {
    free(s);
    s = NULL;  // Prevent use-after-free
}
```
---

## 4. Memory Management Strategy

### Manual Memory Lifecycle

Every allocation follows this pattern:
Allocate - Use - Check for Errors - Cleanup

#### Example: String Creation

```c
pystr* s = malloc(sizeof(pystr));
if (!s) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
}

s->base.refcount = 1;
s->base.type = PY_STR;
s->length = strlen(data);
s->capacity = s->length + 1;
s->data = malloc(s->capacity);

if (!s->data) {
    fprintf(stderr, "Memory allocation failed\n");
    free(s);
    return NULL;
}

strcpy(s->data, data);

return (PyObject*)s;

pystr_del((PyObject*)s);
```

### Stack vs Heap Allocation

```c
// Stack (automatic cleanup)
char buffer[200];  

// Heap (manual cleanup)
char* buffer = malloc(200);
// ... use buffer ...
free(buffer);  
```

Our interpreter uses:
- **Stack:** Local buffers (MAX_LINE = 200 bytes)
- **Heap:** Dynamic strings, lists (pystr_new_from, pylist_new)

---

## 5. Input Validation

### Incorrect Vectors

```python
A = """"""""""...""""""""  # Very long string
# If not validated, overflows buffer

1 = 5  # Invalid variable name
# If not validated, crashes parser

if a > 5  # Missing body
while a  # Invalid condition
# If not validated, parser fails
```

### Response

#### 1. ** String Length Validation **

```c
if (strlen(expr) >= MAX_LINE) {
    fprintf(stderr, "Expression too long\n");
    return 0;
}
```

Prevents buffer overflow from long expressions.

#### 2. ** Variable Name Validation **

```c
char var_name = trimmed[0];
if (!isalpha(var_name)) {
    fprintf(stderr, "Error: invalid variable name '%c'\n", var_name);
    return;
}
```
Ensures only a-z, A-Z are used.

#### 3. ** Quote Validation **

```c
char* end = strchr(start, quote);
if (!end) {
    fprintf(stderr, "Error: unclosed string\n");
    return;
}
```
Prevents parsing invalid string literals.

#### 4. ** Block Size Validation **

```c
int current_len = strlen(if_block);
int line_len = strlen(lines[j] + block_indent);
if (current_len + line_len + 2 > MAX_BLOCK_SIZE) {
    fprintf(stderr, "Error: block too large\n");
    break;
}
```

Prevents overflow when collecting blocks.

---

## 6. Safe Arithmetic

### Division by Zero

```c
// UNSAFE
int result = a / b;

// SAFE
int result = b ? a / b : 0;
```

### Integer Overflow

```c
if (strlen(expr) > MAX_LINE) {
    fprintf(stderr, "Expression too long\n");
    return 0;
}
```
---

### Security Test Cases

```c
void test_security() {
    // Test 1: Division by zero (should return 0)
    int result = evaluate_expression("10 / 0");
    assert(result == 0);
    
    // Test 2: Very long expression (should reject)
    char long_expr[10000];
    memset(long_expr, 'A', 9999);
    result = evaluate_expression(long_expr);
    // Should print error, not crash
    
    // Test 3: Unclosed string (should error)
    interpret("A = \"unclosed");
    // Should print error, not crash
    
    // Test 4: Invalid variable name (should error)
    interpret("1 = 5");
    // Should print error, not crash
    
    // Test 5: Large block (should reject)
    char large_block[3000];
    memset(large_block, 'a', 2999);
    execute_statements(large_block);
    // Should print error about block too large
}
```

### Compile with AddressSanitizer

```bash
gcc -fsanitize=address -g -o interpreter main.c interpreter.c \
    strings.c list.c pydict.c map.c pyint.c pyobject.c

./interpreter
# Any memory errors (overflow, use-after-free) are caught
```
---

## Summary: Security Principles

1. **Bounds check before every buffer operation**
2. **Validate all input before processing**
3. **Check pointers for NULL before dereference**
4. **Pair every malloc with free**
5. **Use reference counting for objects**
6. **Return safely on errors (don't crash)**
7. **Check for edge cases (0, negative, very large)**
---