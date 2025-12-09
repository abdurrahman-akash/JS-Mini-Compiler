console.log("Test 1: Basic try-catch");
try {
    throw "Test error";
} catch (e) {
    console.log("Caught: " + e);
}

console.log("Test 2: Try-finally");
try {
    console.log("In try");
} finally {
    console.log("In finally");
}

console.log("Test 3: Try-catch-finally");
try {
    throw "Error";
} catch (e) {
    console.log("Caught: " + e);
} finally {
    console.log("Finally");
}

console.log("All tests passed!");
