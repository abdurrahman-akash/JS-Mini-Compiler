// Mini JavaScript Compiler - Feature Demonstration
// This file showcases all working features

// === STRINGS ===
print("=== Strings ===");
let str1 = "Hello";
let str2 = "World";
let combined = str1 + " " + str2 + "!";
print(combined);

let firstName = "John";
let lastName = "Doe";
print(firstName + " " + lastName);

// String comparison
if ("apple" < "banana") {
    print("apple < banana");
}

// === ARRAYS ===
print("=== Arrays ===");
let numbers = [10, 20, 30, 40, 50];
print(numbers[0]);
print(numbers[1]);
print(numbers[4]);

let arr = [1, 2, 3];
print(arr.length);

// === OBJECTS ===
print("=== Objects ===");
let person = {name: "Alice", age: 25, city: "NYC"};
print(person.name);
print(person.age);
print(person.city);

let config = {debug: true, timeout: 5000};
print(config.debug);
print(config.timeout);

// === CONTROL FLOW ===
print("=== Control Flow ===");

// If-else
let score = 85;
if (score >= 90) {
    print("A grade");
} else {
    print("Not A grade");
}

// While loop
let count = 0;
while (count < 3) {
    print(count);
    count = count + 1;
}

// Comparisons
let a = 10;
let b = 20;
if (a < b && a > 5) {
    print("a is between 5 and b");
}

// === MIXED FEATURES ===
print("=== Mixed Features ===");

// Array with loop
let items = [100, 200, 300];
let i = 0;
while (i < items.length) {
    print(items[i]);
    i = i + 1;
}

// String concatenation in condition
let greeting = "Hi";
let suffix = "!";
let message = greeting + suffix;
if (message == "Hi!") {
    print("Match!");
}

// Object with strings
let user = {username: "admin", role: "superuser"};
print(user.username);
print(user.role);

print("=== All Tests Passed ===");
