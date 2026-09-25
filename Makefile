CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
SANITIZE = -fsanitize=address

# Source files
SOURCES = interpreter.c strings.c list.c pydict.c map.c pyint.c pyobject.c queue.c stack.c typesystem.c security.c

# Main interpreter
interpreter: main.c $(SOURCES)
	$(CC) $(CFLAGS) -o interpreter $^

# With AddressSanitizer (catch memory bugs)
interpreter_sanitize: main.c $(SOURCES)
	$(CC) $(CFLAGS) $(SANITIZE) -o interpreter_sanitize $^

# Test suite
test_expr: tests/test_expressions.c $(SOURCES)
	$(CC) $(CFLAGS) -o test_expr $^

test_ctrl: tests/test_control.c $(SOURCES)
	$(CC) $(CFLAGS) -o test_ctrl $^

test_str: tests/test_strings.c $(SOURCES)
	$(CC) $(CFLAGS) -o test_str $^

test_list: tests/test_list.c $(SOURCES)
	$(CC) $(CFLAGS) -o test_list $^

test_map: tests/test_map_dict.c $(SOURCES)
	$(CC) $(CFLAGS) -o test_map $^

test_all: test_expr test_ctrl test_str test_list test_map
	@echo "Running expression tests..."
	./test_expr
	@echo "\nRunning control flow tests..."
	./test_ctrl
	@echo "\nRunning string tests..."
	./test_str
	@echo "\nRunning list tests..."
	./test_list
	@echo "\nRunning map/dict tests..."
	./test_map

run: interpreter
	./interpreter

sanitize: interpreter_sanitize
	./interpreter_sanitize

clean:
	rm -f interpreter interpreter_sanitize test_expr test_ctrl test_str test_list test_map
	rm -f *.o

.PHONY: run sanitize test_all clean