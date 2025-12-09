// Test 1: Strings
let greeting = "Hello";
let name = "World";
let message = greeting + " " + name + "!";
print(message);

// Test 2: String comparison
let str1 = "apple";
let str2 = "banana";
if (str1 < str2) {
    print("apple comes before banana");
}

// Test 3: Arrays
let numbers = [1, 2, 3, 4, 5];
print(numbers);

let arr = [10, 20, 30];
print(arr.length);

// Test 4: Array indexing
let first = arr[0];
let second = arr[1];
print(first);
print(second);

// Test 5: Objects
let person = {name: "Alice", age: 30};
print(person);

// Test 6: Object member access
print(person.name);
print(person.age);

// Test 7: Functions
function add(a, b) {
    return a + b;
}

let sum = add(5, 3);
print(sum);

// Test 8: Function with strings
function greet(name) {
    return "Hello, " + name;
}

print(greet("Bob"));

// Test 9: Factorial function
function factorial(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

print(factorial(5));

// Test 10: Array and function
function sumArray(arr) {
    let total = 0;
    let i = 0;
    while (i < arr.length) {
        total = total + arr[i];
        i = i + 1;
    }
    return total;
}

let myNumbers = [1, 2, 3, 4, 5];
print(sumArray(myNumbers));
