// Demonstration of control flow features

// Example 1: Basic if statement
let age = 20;
if (age >= 18) {
    print(1);  // Prints: 1 (adult)
}

// Example 2: If-else statement
let score = 75;
if (score >= 90) {
    print(90);
} else {
    print(75);  // Prints: 75
}

// Example 3: Comparison operators
let a = 10;
let b = 20;
if (a < b) print(1);   // Prints: 1
if (a != b) print(1);  // Prints: 1
if (b > a) print(1);   // Prints: 1

// Example 4: Logical operators
let isStudent = true;
let hasID = true;
if (isStudent && hasID) {
    print(100);  // Prints: 100
}

let isMember = false;
let isGuest = true;
if (isMember || isGuest) {
    print(200);  // Prints: 200
}

// Example 5: While loop
let count = 0;
while (count < 5) {
    print(count);  // Prints: 0, 1, 2, 3, 4
    count = count + 1;
}

// Example 6: Factorial calculation
let n = 5;
let factorial = 1;
let i = 1;
while (i <= n) {
    factorial = factorial * i;
    i = i + 1;
}
print(factorial);  // Prints: 120

// Example 7: Nested control flow
let x = 0;
while (x < 3) {
    if (x == 1) {
        print(999);  // Prints once when x is 1
    }
    x = x + 1;
}
