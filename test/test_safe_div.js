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
console.log("Done!");
