// Error Handling Test Suite

console.log("=== Basic Try-Catch ===");

try {
    console.log("In try block");
    throw "Something went wrong!";
    console.log("This should not execute");
} catch (e) {
    console.log("Caught error: " + e);
}

console.log("After try-catch");

console.log("\n=== Try-Catch with Variable ===");

let result = 0;
try {
    result = 10;
    throw "Error occurred";
    result = 20;
} catch (error) {
    console.log("Error: " + error);
    result = 5;
}
console.log("Result: " + result);

console.log("\n=== Try-Finally (no error) ===");

try {
    console.log("Try block executed");
    let x = 100;
    console.log("x = " + x);
} finally {
    console.log("Finally block always runs");
}

console.log("\n=== Try-Catch-Finally ===");

try {
    console.log("Starting operation...");
    throw "Operation failed";
} catch (err) {
    console.log("Handled: " + err);
} finally {
    console.log("Cleanup completed");
}

console.log("\n=== Nested Try-Catch ===");

try {
    console.log("Outer try");
    try {
        console.log("Inner try");
        throw "Inner error";
    } catch (e) {
        console.log("Inner catch: " + e);
    }
    console.log("After inner try-catch");
} catch (e) {
    console.log("Outer catch: " + e);
}

console.log("\n=== Function with Error Handling ===");

function divide(a, b) {
    try {
        if (b == 0) {
            throw "Division by zero";
        }
        return a / b;
    } catch (e) {
        console.log("Error in divide: " + e);
        return 0;
    }
}

console.log("10 / 2 = " + divide(10, 2));
console.log("10 / 0 = " + divide(10, 0));

console.log("\n=== All Error Handling Tests Passed! ===");
