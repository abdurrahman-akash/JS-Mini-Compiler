# Mini JavaScript Compiler

A minimal JavaScript compiler/interpreter written in C that demonstrates the fundamentals of language implementation. This project implements a subset of JavaScript syntax and evaluates programs using a tree-walking interpreter approach.

## Features

- **Lexical Analysis**: Tokenizes JavaScript source code into meaningful tokens
- **Syntax Parsing**: Constructs an Abstract Syntax Tree (AST) from tokens
- **Variable Declaration**: Support for `let` keyword variable declarations
- **Variable Reassignment**: Update existing variable values
- **Arithmetic Operations**: Addition, subtraction, multiplication, and division
- **Comparison Operators**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical Operators**: `&&`, `||`, `!` (and, or, not)
- **Boolean Values**: `true` and `false` literals
- **Control Flow**: If-else statements and while loops
- **Block Statements**: Group multiple statements with `{ }`
- **Comments**: Single-line comments with `//`
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
// Variables and arithmetic
let x = 10;
let y = 20;
let z = x + y * 2;
print(z);  // Output: 50

// Control flow
let age = 20;
if (age >= 18) {
    print(1);  // Output: 1
}

// While loops
let count = 0;
while (count < 3) {
    print(count);  // Output: 0, 1, 2
    count = count + 1;
}

// Logical operators
if (x > 5 && y > 15) {
    print(100);  // Output: 100
}
```

Run the compiler:

```bash
./build/mini_js test.js
```

Output:
```
50
1
0
1
2
100
```

## Supported Syntax

### Variable Declaration and Assignment
```javascript
let variableName = expression;
variableName = newValue;  // Reassignment
```

### Arithmetic Expressions
```javascript
let result = 10 + 20 * 2;  // Respects precedence
let calc = (10 + 20) * 2;  // Supports parentheses
```

### Operators
**Arithmetic:**
- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Unary minus: `-x`

**Comparison:**
- Equal: `==`
- Not equal: `!=`
- Less than: `<`
- Greater than: `>`
- Less than or equal: `<=`
- Greater than or equal: `>=`

**Logical:**
- And: `&&`
- Or: `||`
- Not: `!`

### Control Flow
**If Statement:**
```javascript
if (condition) {
    // statements
}
```

**If-Else Statement:**
```javascript
if (condition) {
    // then branch
} else {
    // else branch
}
```

**While Loop:**
```javascript
while (condition) {
    // loop body
}
```

### Boolean Values
```javascript
let t = true;
let f = false;
```

### Block Statements
```javascript
{
    let x = 10;
    print(x);
}
```

### Comments
```javascript
// Single-line comments are supported
```

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
- Limited data types (only numbers and booleans)
- No functions or closures
- No for loops (only while loops)
- No objects or arrays
- No strings
- No standard library beyond `print()`
- Simple global variable scope only
- No break/continue statements
- No switch statements
- No ternary operator

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
print(sum);  // Output: 15
```

### If-Else Statement
```javascript
let score = 85;
if (score >= 90) {
    print(1);  // A grade
} else {
    print(0);  // Not A grade
}
// Output: 0
```

### While Loop - Factorial
```javascript
let n = 5;
let factorial = 1;
let i = 1;
while (i <= n) {
    factorial = factorial * i;
    i = i + 1;
}
print(factorial);  // Output: 120
```

### Logical Operators
```javascript
let x = 10;
let y = 20;
if (x > 5 && y < 25) {
    print(1);  // Both conditions true
}
if (x > 15 || y > 15) {
    print(2);  // At least one true
}
// Output: 1, 2
```

### Nested Control Flow
```javascript
let i = 0;
while (i < 3) {
    if (i == 1) {
        print(100);
    } else {
        print(200);
    }
    i = i + 1;
}
// Output: 200, 100, 200
```

## License

This is an educational project for learning compiler construction principles.

## Author

Created as a learning project to explore compiler design and implementation.
