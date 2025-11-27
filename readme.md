Python Interpreter using C with Object-Oriented Features

A comprehensive C-based interpreter that implements Python-like object-oriented programming features including strings, lists, dictionaries, and encapsulated maps using only structs, pointers, and dynamic memory allocation.

Features

Core Language Features

Variables: Integer variables with arithmetic operations
Control Structures: if/else, while, for loops
Expressions: Arithmetic with +, -, *, /, mod
Input/Output: print() function for output
Object-Oriented Types

Strings (pystr)
Variables: A-Z
Methods:
.append(char) - Append single character
.appends(string) - Append entire string
.print() - Display string
.len() - Get length

Lists (pylist)
Variables: L-O
Methods:
.append(value) - Add element
.print() - Display list
.len() - Get length
.index(value) - Find element index
.get(index) - Retrieve element
.set(index, value) - Update element

Dictionaries (pydict)
Variables: D-G
Methods:
.put(key, value) - Add/update key-value pair
.get(key) - Retrieve value
.print() - Display dictionary
.len() - Get count of pairs

Encapsulated Maps (struct Map)
Variables: M-P
Methods:
.put(key, value) - Add/update key-value pair
.get(key, default) - Retrieve value with default
.size() - Get count of pairs
.dump() - Display map contents
.del() - Explicit destruction

Installation & Compilation

Prerequisites

GCC compiler
Make (optional)
Compilation Command

bash
gcc -o myprogram main.c list.c strings.c pydict.c map.c point.c

run
./myprogram

Variable Names:

A-Z: Strings (text)
L-O: Lists (collections)
D-G: Dictionaries (key-value pairs)
M-P: Maps (key-number pairs)

Common Operations:

= to assign values
print() to display
+ - * / for math
if/else for decisions
for/while for loops

"Variable not found": Make sure you used = [] for lists, = {} for dictionaries

"Method not found": Check your variable name is in the right range (A-Z, L-O, etc.)