Here is a **clean, polished, professional README** version of yours — more structured, clearer, and GitHub-ready, while preserving everything you built.

---

# **Python-Style Interpreter in C (with Data Structures)**

A lightweight Python-like interpreter written in C, supporting expressions, control flow, and ßobject-oriented data structures (strings, lists, dictionaries, and maps).
It is implemented using **structs, pointers, and manual memory management**—no external libraries.

---

##  **Features**

### **Core Language**

* **Variables** – Single-letter variable names
* **Expressions** – `+`, `-`, `*`, `/`, `%`
* **Control Flow** – `if`, `else`, `while`, `for`
* **I/O** – `print()`
* **Blocks** – Python-style indentation

---

##  **Object-Oriented Data Types**

### **Strings (`pystr`)**

Variable names: **A–Z**

Methods:

* `.append(char)` – Append one character
* `.appends(string)` – Append full string
* `.print()` – Display string
* `.len()` – Length of string
* Supports reassignment via `A = "hello"`

---

### **Lists (`pylist`)**

Variable names: **L–O**

Methods:

* `.append(value)` – Add element
* `.len()` – List length
* `.index(value)` – First index of value
* `.get(index)` – Retrieve element
* `.set(index, value)` – Modify element
* `.print()` – Display list

---

### **Dictionaries (`pydict`)**

Variable names: **D–G**

Methods:

* `.put(key, value)` – Insert/update
* `.get(key)` – Retrieve
* `.len()` – Number of pairs
* `.print()` – Display contents

---

### **Encapsulated Maps (`struct Map`)**

Variable names: **M–P**

Methods:

* `.put(key, value)` – Insert/update
* `.get(key, default)` – Retrieve with default
* `.size()` – Size
* `.dump()` – Print full map
* `.del()` – Explicit destruction

---

## **Installation & Compilation**

### **Prerequisites**

* GCC
* Make (optional)

### **Compile**

```bash
gcc -o interpreter main.c list.c strings.c pydict.c map.c
```

### **Run**

```bash
./interpreter
```

---

## **Variable Rules**

| Type         | Variable Names |
| ------------ | -------------- |
| Strings      | **A–Z**        |
| Lists        | **L–O**        |
| Dictionaries | **D–G**        |
| Maps         | **M–P**        |

---

## **Common Syntax**

* Assignment: `x = 5`, `A = "hello"`
* Math: `2 + 3 * 4`
* Lists: `L = []`
* Dicts: `D = {}`
* Loops:

  ```python
  for i in range(3):
      print(i)
  ```
* Conditionals:

  ```python
  if x > 2:
      print(1)
  else:
      print(0)
  ```

---

## **Common Errors**

* **"Variable not found"** → Make sure you initialized a list (`= []`) or dict (`= {}`)
* **"Method not found"** → Ensure variable is in the correct A–Z / L–O / D–G / M–P range
