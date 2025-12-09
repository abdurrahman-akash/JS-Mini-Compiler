function validateAge(age) {
    try {
        if (age < 0) {
            throw "Age cannot be negative";
        }
        return true;
    } catch (e) {
        console.log("Error: " + e);
        return false;
    } finally {
        console.log("Finally");
    }
}

console.log("Test 1");
let r1 = validateAge(25);
console.log("Result 1: " + r1);

console.log("Test 2");
let r2 = validateAge(-5);
console.log("Result 2: " + r2);

console.log("Done");
