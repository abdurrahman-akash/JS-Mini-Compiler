CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Iinclude

SRC=src/main.c src/lexer.c src/parser.c src/ast.c src/eval.c src/env.c src/value.c
OBJ=$(SRC:.c=.o)

all: mini_js

mini_js: $(OBJ)
	$(CC) $(OBJ) -o build/mini_js

clean:
	rm -f $(OBJ) build/mini_js
