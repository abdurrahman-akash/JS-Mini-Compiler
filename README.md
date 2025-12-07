# Mini JavaScript Compiler

A minimal JavaScript compiler/interpreter written in C that demonstrates the fundamentals of language implementation. This project implements a subset of JavaScript syntax and evaluates programs using a tree-walking interpreter approach.

## Features

- **Lexical Analysis**: Tokenizes JavaScript source code into meaningful tokens
- **Syntax Parsing**: Constructs an Abstract Syntax Tree (AST) from tokens
- **Variable Declaration**: Support for `let` keyword variable declarations
- **Arithmetic Operations**: Addition, subtraction, multiplication, and division
- **Variable References**: Read and write variable values
- **Print Function**: Built-in `print()` function for output
- **Expression Evaluation**: Respects operator precedence and parentheses

## Architecture

### Components

1. **Lexer** (`src/lexer.c`, `src/lexer.h`)
   - Tokenizes input source code
   - Recognizes keywords, identifiers, numbers, and operators

2. **Parser** (`src/parser.c`, `src/parser.h`)
   - Recursive descent parser
   - Builds AST from token stream
   - Implements operator precedence

3. **AST** (`src/ast.c`, `src/ast.h`)
   - Abstract Syntax Tree node definitions
   - Node types: numbers, variables, binary operations, assignments, print statements

4. **Evaluator** (`src/eval.c`, `src/eval.h`)
   - Tree-walking interpreter
   - Direct AST evaluation without bytecode compilation

5. **Environment** (`src/env.c`, `src/env.h`)
   - Variable storage and lookup
   - Simple symbol table implementation

## Building

```bash
make
```

This compiles all source files and generates the `build/mini_js` executable.

## Usage

Create a JavaScript file with supported syntax:

```javascript
let x = 10;
let y = 20;
let z = x + y * 2;
print(z);
print(x - 5);
```

Run the compiler:

```bash
./build/mini_js test.js
```

Output:
```
50
5
```

## Supported Syntax

### Variable Declaration
```javascript
let variableName = expression;
```

### Arithmetic Expressions
```javascript
let result = 10 + 20 * 2;  // Respects precedence
let calc = (10 + 20) * 2;  // Supports parentheses
```

### Operators
- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Unary minus: `-x`

### Print Statement
```javascript
print(expression);
```

## Project Structure

```
.
├── Makefile              # Build configuration
├── build/                # Compiled binary output
│   └── mini_js
├── example/              # Example JavaScript files
│   └── test.js
├── include/              # Header files
│   └── mini_js.h
└── src/                  # Source code
    ├── ast.c/.h          # Abstract Syntax Tree
    ├── env.c/.h          # Variable environment
    ├── eval.c/.h         # Expression evaluator
    ├── lexer.c/.h        # Lexical analyzer
    ├── parser.c/.h       # Syntax parser
    ├── main.c            # Entry point
    └── util.h            # Utility functions
```

## Limitations

This is a minimal educational implementation and has several limitations:

- No bytecode compilation or virtual machine
- No garbage collection (manual memory management)
- Limited data types (only numbers)
- No functions or closures
- No control flow (if/else, loops)
- No objects or arrays
- No standard library beyond `print()`
- No string support
- Simple global variable scope only

## Requirements

- GCC compiler with C99 support
- Make build system
- Linux/Unix environment

## Development

Clean build artifacts:
```bash
make clean
```

Rebuild from scratch:
```bash
make clean && make
```

## Example Programs

### Basic Arithmetic
```javascript
let a = 5;
let b = 10;
let sum = a + b;
print(sum);
```

### Complex Expressions
```javascript
let x = 100;
let y = 50;
let result = (x + y) / 2 - 10 * 3;
print(result);
```

## License

This is an educational project for learning compiler construction principles.

## Author

Created as a learning project to explore compiler design and implementation.
