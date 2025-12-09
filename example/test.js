// Modern JavaScript syntax test with console.log

console.log("=== Testing console.log ===");

// Variables
let name = "JavaScript";
let version = 2024;
console.log("Language: " + name);
console.log(version);

// Arrays
let numbers = [10, 20, 30, 40, 50];
console.log("First number: " + numbers[0]);
console.log("Array length: " + numbers.length);

// Objects
let person = {
    name: "Alice",
    age: 30,
    city: "NYC"
};
console.log("Name: " + person.name);
console.log("Age: " + person.age);

// Functions
function add(a, b) {
    return a + b;
}

let sum = add(15, 25);
console.log("Sum: " + sum);

// Recursion
function countdown(n) {
    if (n <= 0) {
        console.log("Done!");
        return 0;
    }
    console.log(n);
    return countdown(n - 1);
}

countdown(5);

// Factorial
function factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

console.log("5! = " + factorial(5));

console.log("=== All tests passed! ===");
