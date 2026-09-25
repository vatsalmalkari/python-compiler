# ** Python-Style Interpreter in C (with Data Structures) **

A lightweight Python-like interpreter written in C, supporting expressions, control flow, and object-oriented data structures (strings, lists, dictionaries, and maps). Built with a focus on security, memory safety, and compiler fundamentals.

Implemented using structs, pointers, and manual memory management no external libraries.

---

## ** Features **

- **Variables** – Single-letter variable names (a-z for integers, A-Z for strings, L-O for lists, D-G for dicts)
- **Expressions** – Arithmetic operators: `+`, `-`, `*`, `/`, `%` with proper operator precedence
- **Control Flow** – `if`, `else`, `while`, `for` loops with Python-style indentation
- **Functions** – Define and call functions with parameters and return values (planned)
- **I/O** – `print()` function for output
- **Type System** – Runtime type checking and inference

### ** Data Structures **

- **Strings** – Dynamic string allocation with automatic capacity growth
- **Lists** – Dynamic arrays with append, indexing, and length operations
- **Dictionaries** – Key-value storage with O(1) lookup
### ** Security **

- **Buffer Overflow Protection** – All string operations bounds-checked
- **Type Safety** – Type checking on variable assignment
- **Memory Safety** – Proper cleanup with reference counting
- **Division by Zero** – Returns 0 instead of crashing
- **Input Validation** – Validates string lengths, variable names, and block sizes

---

main.c (REPL Interface) │
│ Reads input, tracks line numbers │
└────────────┬────────────────────────────┘
│
┌────────────▼────────────────────────────┐
│ interpreter.c (Execution Engine) │
│ ├─ Tokenization (lexical analysis) │
│ ├─ Parsing (syntax analysis) │
│ ├─ Type Checking (semantic analysis) │
│ └─ Execution (tree-walking eval) │
└────────────┬────────────────────────────┘
│
┌────────────▼────────────────────────────┐
│ Data Layer (strings.c, list.c, etc.) │
│ Manages objects, memory, references │

Source Code -> Tokenizer (strtok) -> Parser (identify statement type) -> Type Inference (infer_type) -> Evaluator (execute_statements) -> Result

---
---

## **Installation & Compilation**

### **Prerequisites**

- GCC or Clang compiler
- Standard C library
- Make (optional)

### **Compile**

```bash
# Basic compilation
gcc -Wall -Wextra -g -I./include -o interpreter \
    main.c interpreter.c strings.c list.c pydict.c \
    pyint.c pyobject.c queue.c stack.c typesystem.c security.c

# With AddressSanitizer
gcc -fsanitize=address -g -I./include -o interpreter \
    main.c interpreter.c strings.c list.c pydict.c \
    pyint.c pyobject.c queue.c stack.c typesystem.c security.c

```

### **Run Interactive REPL**

```bash
./interpreter
>>> a = 5
>>> print(a)
5
>>> exit (or Ctrl+D)
```
### **Run Test Suite**

# Run tests
gcc -Wall -Wextra -g -I./include -o test_expr \
    tests/test_expressions.c interpreter.c strings.c list.c \
    pydict.c map.c pyint.c pyobject.c queue.c stack.c typesystem.c security.c
./test_expr

gcc -Wall -Wextra -g -I./include -o test_ctrl \
    tests/test_control.c interpreter.c strings.c list.c \
    pydict.c map.c pyint.c pyobject.c queue.c stack.c typesystem.c security.c
./test_ctrl
---

## **Quick Start**

### **Example 1: Basic Variables and Arithmetic**

```python
a = 5
b = 10
c = a + b * 2
print(c)  # Output: 25
```

### **Example 2: Control Flow**

```python
for i in range(5):
    if i % 2 == 0:
        print(i)
```

**Output:** 0, 2, 4

### ** Example 4: Lists **

```python
L = [1, 2, 3]
L.append(4)
print(L)  # Output: [1, 2, 3, 4]
```
---

## ** Data Types & Variables **

### ** Integer Variables (a-z) **

Single lowercase letter for integers.

```python
a = 5
b = 10
c = a + b
print(c)  # 15
```

**Behavior:**
- Range: 32-bit signed integers (-2,147,483,648 to 2,147,483,647)
- Uninitialized variables default to 0
- Division by zero returns 0 (safe)

### ** String Variables (A-Z) **

Single uppercase letter for strings.

```python
A = "hello"
B = "world"
C = A + B
print(C)
A.append(c) 
```

**Behavior:**
- Dynamic memory allocation
- Capacity doubles when needed
- Maximum string length: 199 characters
- Supports concatenation and character append

### ** List Variables (L-O) **

Four uppercase letters (L, M, N, O) for lists.

```python
L = [1, 2, 3]
L.append(4)
L.append(5)
print(L)
```

**Behavior**
- Dynamic array of integers
- `.append(value)` – Add element
- `.print()` – Display contents
- Maximum 10 lists can exist simultaneously

### ** Dictionary Variables (D-G) **

Four uppercase letters (D, E, F, G) for dictionaries.

```python
D = {} 
D = {"a": 1, "b": 2}
print(D)
```

### ** Map Variables (M-P) **
// needs to be implemented
Four uppercase letters (M, N, O, P) for maps.

```python
M = {}
M= {"key": 1, "key2": 2}
print(M)
```

### ** Variable Scope **

| Variable Type | Names | Max Count 
| Integer | a-z | 26 
| String | A-Z | 26 
| List | L-O | 4 
| Dictionary | D-G | 4 
| Map | M-P | 4

---

## **Syntax & Examples**

### **Operators & Precedence**

```python
a = 2 + 3 * 4    # 14 
b = 10 - 2 * 3   # 4
c = 20 / 4 + 5   # 10
d = 10 mod 3     # 1

if a > 5:        
    print(1)
if a == 5:       
    print(1)
if a != 5:       
    print(1)
if a >= 5:       
    print(1)
if a <= 5:       
    print(1)
```

### **Control Flow**

#### **If/Else**

```python
x = 10
if x > 5:
    print(1)
else:
    print(0)
```

**Indentation matters!** Use spaces or tabs consistently.

#### **While Loop**

```python
i = 0
while i < 5:
    print(i)
    i = i + 1
```

**Warning:** Infinite loops will hang. Use Ctrl+C to stop.

#### ** For Loop **

```python
for i in range(5):
    print(i)
```

**Output:** 0, 1, 2, 3, 4

**Note:** `range(n)` goes from 0 to n-1 (Python-style).

#### ** Nested Loops**

```python
for i in range(3):
    for j in range(2):
        print(i)  # Prints 0, 0, 1, 1, 2, 2
```

### ** Strings & String Operations **

```python
A = "hello"

B = "world"
C = A + B
print(C)  # helloworld

A.append(x)

print(A)
```

### ** Lists & List Operations**

```python
# List creation
L = [1, 2, 3]

# List append
L.append(4)
L.append(5)

# Print list
print(L)  # [1, 2, 3, 4, 5]

# Iterate over list
for x in L:
    print(x)
```

---

## **Security Features**

### **1. Buffer Overflow Protection**

```c
// Before
strcat(large_buffer, user_input);

// After
int current_len = strlen(large_buffer);
int input_len = strlen(user_input);
if (current_len + input_len + 1 > MAX_BLOCK_SIZE) {
    fprintf(stderr, "Error: block too large\n");
    return;
}
strcat(large_buffer, user_input); 
```

**Where it's used:**
- `execute_statements()` – Collecting if/while/for blocks
- String concatenation operations
- All dynamic buffer operations

### **2. Type Safety**

```c
// Type inference
PyType rhs_type = infer_type(val_str);
printf("Inferred type: %s\n", type_name(rhs_type));

// Type mismatch detection
if (existing_var && existing_var->type != inferred_type) {
    fprintf(stderr, "Warning: variable '%c' type changed\n", var_name);
}
```

**Catches:**
- String vs integer confusion
- List vs dictionary confusion
- Invalid type assignments

### **3. Division by Zero**

```c
int result = divisor ? numerator / divisor : 0;
int result = divisor ? numerator % divisor : 0;
```
### **4. Input Validation**
```c
// String length check
if (len > MAX_LINE - 1) {
    fprintf(stderr, "String too long\n");
    return;
}
// Variable name validation
if (!isalpha(var_name)) {
    fprintf(stderr, "Error: invalid variable name '%c'\n", var_name);
    return;
}
// Unclosed quote detection
char* end = strchr(start, quote);
if (!end) {
    fprintf(stderr, "Error: unclosed string\n");
    return;
}
```
### **5. Memory Safety**
**Problem:** Memory leaks and dangling pointers.
**Solution:** Reference counting and cleanup.

```c
pystr* s = pystr_new_from("hello");
pystr_print((PyObject*)s);
pystr_del((PyObject*)s);

```
---

#### **How Memory Management works:**

1. **Allocation** – `malloc()` creates objects
2. **Reference Counting** – Track how many variables use an object
3. **Cleanup** – `free()` when refcount reaches 0

#### ** Example: **

```c
pystr* s = pystr_new_from("hello");  // refcount = 1
pystr_append_char(s, '!');
pystr_del((PyObject*)s);  
```

### ** Lifecycle **
Create: malloc() allocates memory
Use: Access and modify
Reference: Other variables point to it
Cleanup: Free when done

### **Common Pitfalls**

| Problem | Solution |
|---------|----------|
| Forgetting to free | Always pair malloc/free |
| Double free | Check for NULL before free |
| Use after free | Don't access after free |
| Memory leak | Track all allocations |

### **Best Practices**

```c
pystr* s = pystr_new_from("hello");
if (s) {
    pystr_print((PyObject*)s);
    pystr_del((PyObject*)s);
}

pystr* s = pystr_new_from("hello");
pystr_print((PyObject*)s);
// No free

pystr* s = NULL;
pystr_del((PyObject*)s); 
pystr_del((PyObject*)s);  // Double free!
```
---

## ** Testing **

### ** Test Files in tests folder **

1. **test_expressions.c** – Math, operators, conditions
2. **test_controls.c** – Loops, if/else, strings, lists
3. **test_security.c** – Edge cases, buffer protection, type safety

### ** Running Tests **

```bash
# Compile and run expression tests
gcc -o test_expr test_expressions.c interpreter.c strings.c list.c pydict.c pyint.c pyobject.c
./test_expr

# Run control flow tests
gcc -o test_ctrl test_controls.c interpreter.c strings.c list.c pydict.c pyint.c pyobject.c
./test_ctrl

# Run security tests
gcc -o test_sec test_security.c interpreter.c strings.c list.c pydict.c pyint.c pyobject.c
./test_sec
```

### ** Interactive Testing **

```bash
./interpreter
>>> a = 5
>>> b = 10
>>> print(a + b)
15
>>> for i in range(3):
...     print(i)
... 
0
1
2
>>> A = "hello"
>>> B = "world"
>>> C = A + B
>>> print(C)
helloworld
```

---

## **Troubleshooting**

### **Common Issues**

| Issue | Cause | Solution |
|-------|-------|----------|
| "Variable not found" | Variable not initialized | Create variable: `a = 0` |
| "Method not found" | Wrong variable type | Check variable naming: strings are A-Z |
| "String too long" | String exceeds 200 chars | Use shorter strings |
| Infinite loop | Loop condition always true | Press Ctrl+C |
| No output | Missing print statement | Add `print()` call |
| Type changed warning | Reassigning different type | This is allowed, but warned |

### **Compilation Errors**

```bash
# Error: undefined reference to 'pystr_new_from'
# Solution: Include pystr.c in compilation
gcc -o interpreter main.c interpreter.c strings.c list.c ...

# Error: redefinition of 'Variable'
# Solution: Remove duplicate declarations in interpreter.c

# Error: segmentation fault
# Solution: Use AddressSanitizer to find memory bug
gcc -fsanitize=address -o interpreter main.c ...
./interpreter
```

### **Runtime Errors**

```bash
# Division by zero
a = 10 / 0  # Returns 0, doesn't crash 

# Unclosed string
A = "hello  # Error message shown 

# Buffer overflow
# Blocked by bounds checking
```

---

## **Advanced Topics**

### **How Operator Precedence Works**

```c
// Expression: 2 + 3 * 4 - 1
// Tokenized: [2, +, 3, *, 4, -, 1]
// 
// Step 1: Handle * (higher precedence)
//   3 * 4 = 12
//   Now: [2, +, 12, -, 1]
//
// Step 2: Handle + and - (left to right)
//   2 + 12 = 14
//   14 - 1 = 13
// Result: 13
```

### **How Indentation Parsing Works**

```python
if a > 5:           # indent=0, recognized as if statement
    print(1)        # indent=4, part of if block
    print(2)        # indent=4, part of if block
print(3)            # indent=0, outside if block

# Parser tracks:
# - base_indent = 0 (for if statement)
# - block_indent = 4 (for first indented line)
# - Stops collecting block when indent drops to 0
```

### **How Type Inference Works**

```c
PyType infer_type(char* value) {
    if (value[0] == '"') return STR;      // "hello"
    if (value[0] == '[') return LIST;     // [1,2,3]
    if (isdigit(value[0])) return INT;    // 5
    if (isalpha(value[0])) {              // Variable name
        if (find_str_var(value[0])) return STR;
        if (find_list_var(value[0])) return LIST;
        return INT;
    }
    return INT;  // Default
}
```

## ** Takeaways **

** Direct execution from parse tree
**Manual memory management**
**Security-focused** – Buffer overflow, type safety, input validation
**Compiler fundamentals** – Tokenization, parsing, evaluation
**Production-ready** – Proper error handling and bounds checking

---