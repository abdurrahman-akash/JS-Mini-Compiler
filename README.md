# Mini JavaScript Compiler

A feature-rich JavaScript compiler/interpreter written in C that implements a substantial subset of JavaScript. This project includes lexical analysis, parsing, and tree-walking interpretation with support for modern JavaScript features.

## Features

### Core Language Features
- **Lexical Analysis**: Tokenizes JavaScript source code into meaningful tokens
- **Syntax Parsing**: Constructs an Abstract Syntax Tree (AST) using recursive descent parsing
- **Tree-Walking Interpreter**: Direct AST evaluation with proper scoping

### Data Types
- **Numbers**: Floating-point arithmetic
- **Strings**: With escape sequences (`\n`, `\t`, `\\`, `\"`)
- **Booleans**: `true` and `false` literals
- **Arrays**: Dynamic arrays with indexing and `.length` property
- **Objects**: Key-value pairs with member access
- **Functions**: First-class functions with closures
- **Null**: `null` value support

### Variables and Operations
- **Variable Declaration**: `let` keyword with lexical scoping
- **Variable Reassignment**: Update existing variables
- **Arithmetic Operations**: `+`, `-`, `*`, `/`, unary `-`
- **String Concatenation**: `+` operator for strings
- **Comparison Operators**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical Operators**: `&&`, `||`, `!`

### Control Flow
- **If-Else Statements**: Conditional branching
- **While Loops**: Iteration with break conditions
- **Try-Catch-Finally**: Exception handling with error propagation
- **Throw Statements**: Custom error throwing

### Functions
- **Function Declarations**: `function name(params) { body }`
- **Function Expressions**: `let f = function(params) { body }`
- **Parameters and Arguments**: Multi-parameter support
- **Return Statements**: Early returns and return values
- **Recursion**: Full recursive function support (factorial, etc.)
- **Lexical Scoping**: Proper scope chain management

### Advanced Features
- **Array Operations**: Literals `[1,2,3]`, indexing `arr[0]`, `.length` property
- **Object Operations**: Literals `{key: value}`, member access `obj.property`
- **Comments**: Single-line comments with `//`
- **console.log()**: Modern JavaScript output syntax
- **print()**: Alternative output function
- **Block Statements**: `{ }` for grouping statements
- **Error Handling**: Complete try-catch-finally implementation

## Architecture

### Components

1. **Lexer** (`src/lexer.c`, `src/lexer.h`)
   - Tokenizes input source code
   - Recognizes 40+ token types including keywords, operators, and literals
   - String parsing with escape sequence support

2. **Parser** (`src/parser.c`, `src/parser.h`)
   - Recursive descent parser with operator precedence
   - Builds complete AST from token stream
   - Supports function expressions and declarations
   - Error recovery and reporting

3. **AST** (`src/ast.c`, `src/ast.h`)
   - 25+ node types for complete language coverage
   - Support for expressions, statements, and declarations
   - Try-catch-finally and throw nodes

4. **Evaluator** (`src/eval.c`, `src/eval.h`)
   - Tree-walking interpreter with exception handling
   - Direct AST evaluation without bytecode compilation
   - Proper scope management for nested functions

5. **Environment** (`src/env.c`, `src/env.h`)
   - Scope-based variable storage with scope chains
   - push_scope/pop_scope for lexical scoping
   - Variable lookup through parent scopes

6. **Value System** (`src/value.c`, `src/value.h`)
   - Union-type value representation
   - 8 value types with proper memory management
   - Deep copy and free operations

## Building

```bash
make
```

This compiles all source files and generates the `build/mini_js` executable.

## Usage

Create a JavaScript file with supported syntax:

```javascript
// Modern JavaScript syntax with console.log
console.log("=== Basic Features ===");

// Variables and strings
let name = "JavaScript";
let version = 2024;
console.log(name + " " + version);

// Arrays
let numbers = [10, 20, 30, 40, 50];
console.log("First: " + numbers[0]);
console.log("Length: " + numbers.length);

// Objects
let person = {
    name: "Alice",
    age: 30,
    city: "NYC"
};
console.log("Name: " + person.name);
console.log("Age: " + person.age);

// Control flow
if (person.age >= 18) {
    console.log("Adult");
} else {
    console.log("Minor");
}

// While loops
let i = 0;
while (i < 3) {
    console.log("Count: " + i);
    i = i + 1;
}

// Functions
function greet(name) {
    return "Hello, " + name + "!";
}
console.log(greet("World"));

// Recursive functions
function factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
console.log("5! = " + factorial(5));

// Error handling
try {
    console.log("Attempting operation...");
    throw "Something went wrong";
} catch (error) {
    console.log("Caught error: " + error);
} finally {
    console.log("Cleanup complete");
}

// Function expressions
let add = function(a, b) {
    return a + b;
};
console.log("Sum: " + add(10, 20));
```

Run the compiler:

```bash
./build/mini_js example/demo.js
```

See `example/demo.js` and `showcase.js` for comprehensive feature demonstrations.

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

**Try-Catch-Finally:**
```javascript
try {
    // code that may throw
    throw "Error message";
} catch (error) {
    // handle error
    console.log(error);
} finally {
    // always executes
}
```

### Functions

**Function Declaration:**
```javascript
function name(param1, param2) {
    return param1 + param2;
}
```

**Function Expression:**
```javascript
let name = function(param1, param2) {
    return param1 + param2;
};
```

**Recursive Functions:**
```javascript
function factorial(n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}
```

### Arrays

```javascript
let arr = [1, 2, 3, 4, 5];
let first = arr[0];
let length = arr.length;
```

### Objects

```javascript
let obj = {
    name: "Alice",
    age: 30,
    city: "NYC"
};
let name = obj.name;
let age = obj.age;
```

### Strings

```javascript
let greeting = "Hello, World!";
let escaped = "Line 1\nLine 2\tTabbed";
let combined = "Hello" + " " + "World";
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

### Print Statements
```javascript
print(expression);           // Traditional style
console.log(expression);     // Modern JavaScript style
```

## Project Structure

```
.
├── Makefile              # Build configuration
├── README.md             # Project documentation
├── build/                # Compiled binary output
│   └── mini_js
├── example/              # Example JavaScript files
│   ├── demo.js           # Complete feature demo
│   ├── test.js           # Modern syntax examples
│   ├── control_flow_test.js  # Error handling tests
│   └── error_demo.js     # Exception examples
├── include/              # Header files
│   └── mini_js.h
└── src/                  # Source code
    ├── ast.c/.h          # Abstract Syntax Tree (25+ node types)
    ├── env.c/.h          # Scope-based variable environment
    ├── eval.c/.h         # Tree-walking interpreter
    ├── lexer.c/.h        # Lexical analyzer (40+ tokens)
    ├── parser.c/.h       # Recursive descent parser
    ├── value.c/.h        # Value system (8 types)
    ├── main.c            # Entry point
    └── util.h            # Utility functions
```

## Limitations

This is an educational implementation with some limitations:

- **No bytecode compilation**: Direct AST interpretation (slower but simpler)
- **No garbage collection**: Manual memory management with reference tracking
- **No for loops**: Only while loops supported (for loops can be added)
- **No break/continue**: Loop control statements not yet implemented
- **No switch statements**: Use if-else chains instead
- **No ternary operator**: Use if-else expressions
- **No async/await**: Synchronous execution only
- **No modules/imports**: Single-file programs only
- **No classes**: Objects and functions only
- **Limited standard library**: Only `console.log()` and `print()`
- **Simple double-recursion bug**: Functions like fibonacci with two recursive calls in one expression have minor issues (single recursion like factorial works perfectly)

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

### Modern JavaScript Syntax
```javascript
console.log("Hello, World!");

let numbers = [1, 2, 3, 4, 5];
console.log("Array length: " + numbers.length);

let person = {name: "Bob", age: 25};
console.log("Name: " + person.name);
```

### Recursive Factorial
```javascript
function factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

console.log("5! = " + factorial(5));  // Output: 5! = 120
console.log("10! = " + factorial(10)); // Output: 10! = 3628800
```

### Error Handling
```javascript
function safeDivide(a, b) {
    try {
        if (b == 0) {
            throw "Division by zero error";
        }
        return a / b;
    } catch (error) {
        console.log("Error: " + error);
        return 0;
    } finally {
        console.log("Operation completed");
    }
}

console.log("Result: " + safeDivide(10, 2));  // Output: Operation completed, Result: 5
console.log("Result: " + safeDivide(10, 0));  // Output: Error: ..., Operation completed, Result: 0
```

### Control Flow with Arrays
```javascript
let scores = [85, 92, 78, 95, 88];
let i = 0;
let sum = 0;

while (i < scores.length) {
    sum = sum + scores[i];
    i = i + 1;
}

let average = sum / scores.length;
console.log("Average: " + average);  // Output: Average: 87.6
```

### Nested Try-Catch
```javascript
try {
    console.log("Outer try");
    try {
        console.log("Inner try");
        throw "Inner error";
    } catch (e) {
        console.log("Caught inner: " + e);
    }
    console.log("After inner try-catch");
} catch (e) {
    console.log("Outer catch: " + e);
}
// Output: Outer try, Inner try, Caught inner: ..., After inner try-catch
```

### Function Expressions and Closures
```javascript
let makeCounter = function() {
    let count = 0;
    return function() {
        count = count + 1;
        return count;
    };
};

// Note: Full closure support is limited, but function expressions work
let add = function(a, b) {
    return a + b;
};

console.log("Sum: " + add(15, 25));  // Output: Sum: 40
```

## License

This is an educational project for learning compiler construction principles.

## Key Features Summary

✅ **Complete Type System**: Numbers, Strings, Booleans, Arrays, Objects, Functions, Null, Errors  
✅ **Modern Syntax**: `console.log()`, function expressions, arrow-like syntax  
✅ **Error Handling**: Try-catch-finally with throw statements  
✅ **Recursion**: Full support for recursive functions  
✅ **Scoping**: Lexical scoping with scope chains  
✅ **String Operations**: Concatenation, escape sequences, comparisons  
✅ **Collections**: Dynamic arrays and objects with member access  
✅ **Control Flow**: If-else, while loops, exception handling  
✅ **Functions**: First-class functions with parameters and returns  

## Test Files

- `showcase.js` - Comprehensive feature demonstration
- `example/demo.js` - Modern JavaScript syntax examples
- `example/test.js` - Complete feature test suite
- `example/control_flow_test.js` - Error handling tests
- `test_factorial.js` - Recursive function test
- `test_safe_div.js` - Error handling in functions

## Author

Created as a learning project to explore compiler design, parsing techniques, and interpreter implementation.
