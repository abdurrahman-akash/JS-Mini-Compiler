// Comprehensive Error Handling Demo

console.log("=== Error Handling with Arrays ===");

let numbers = [1, 2, 3, 4, 5];

try {
    console.log("Array length: " + numbers.length);
    console.log("First element: " + numbers[0]);
    throw "Array processing error";
} catch (e) {
    console.log("Caught: " + e);
}

console.log("\n=== Error Handling with Objects ===");

let user = {
    name: "John",
    age: 30
};

try {
    console.log("User: " + user.name);
    if (user.age < 18) {
        throw "User too young";
    }
    console.log("Age check passed");
} catch (e) {
    console.log("Error: " + e);
}

console.log("\n=== Error in Loop ===");

let i = 0;
while (i < 5) {
    try {
        if (i == 3) {
            throw "Error at iteration 3";
        }
        console.log("Iteration: " + i);
    } catch (e) {
        console.log("Caught in loop: " + e);
    }
    i = i + 1;
}

console.log("\n=== Validation Function ===");

function validateAge(age) {
    try {
        if (age < 0) {
            throw "Age cannot be negative";
        }
        if (age > 150) {
            throw "Age too high";
        }
        return true;
    } catch (e) {
        console.log("Validation error: " + e);
        return false;
    } finally {
        console.log("Validation check completed");
    }
}

validateAge(25);
validateAge(-5);
validateAge(200);

console.log("\n=== Safe Division ===");

function safeDivide(a, b) {
    let result = 0;
    try {
        if (b == 0) {
            throw "Cannot divide by zero";
        }
        result = a / b;
    } catch (e) {
        console.log("Error: " + e);
        result = 0;
    } finally {
        console.log("Operation completed");
    }
    return result;
}

console.log("Result: " + safeDivide(10, 2));
console.log("Result: " + safeDivide(10, 0));

console.log("\n=== All Demos Completed! ===");
