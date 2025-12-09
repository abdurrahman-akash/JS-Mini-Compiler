// Complete JavaScript Mini-Compiler Demo
// Showcasing all features including error handling

console.log("=== Variables and Data Types ===");
let name = "JavaScript";
let version = 2024;
let isAwesome = true;
console.log(name + " " + version);

console.log("\n=== Arrays ===");
let numbers = [10, 20, 30];
console.log("First: " + numbers[0]);
console.log("Length: " + numbers.length);

console.log("\n=== Objects ===");
let person = {
    name: "Alice",
    age: 30
};
console.log("Name: " + person.name);

console.log("\n=== Control Flow ===");
if (version > 2020) {
    console.log("Modern JS");
}

let i = 0;
while (i < 3) {
    console.log("Count: " + i);
    i = i + 1;
}

console.log("\n=== Functions ===");
function greet(name) {
    return "Hello, " + name;
}
console.log(greet("World"));

console.log("\n=== Recursion ===");
function factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}
console.log("5! = " + factorial(5));

console.log("\n=== Error Handling ===");
try {
    console.log("Attempting operation...");
    throw "Something failed";
} catch (error) {
    console.log("Caught: " + error);
} finally {
    console.log("Cleanup done");
}

function safeDivide(a, b) {
    try {
        if (b == 0) {
            throw "Division by zero";
        }
        return a / b;
    } catch (e) {
        console.log("Error: " + e);
        return 0;
    }
}

console.log("10 / 2 = " + safeDivide(10, 2));
console.log("10 / 0 = " + safeDivide(10, 0));

console.log("\n=== All Features Working! ===");
