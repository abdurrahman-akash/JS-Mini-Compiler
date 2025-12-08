// Test 1: If statement
let x = 10;
if (x > 5) {
    print(100);
}

// Test 2: If-else statement
let y = 3;
if (y > 5) {
    print(200);
} else {
    print(300);
}

// Test 3: Comparison operators
let a = 10;
let b = 20;
if (a < b) {
    print(1);
}
if (a <= 10) {
    print(1);
}
if (b > a) {
    print(1);
}
if (b >= 20) {
    print(1);
}
if (a == 10) {
    print(1);
}
if (a != b) {
    print(1);
}

// Test 4: Logical operators
let p = 1;
let q = 0;
if (p && q) {
    print(400);
} else {
    print(500);
}

if (p || q) {
    print(600);
}

if (!q) {
    print(700);
}

// Test 5: While loop
let counter = 0;
while (counter < 3) {
    print(counter);
    counter = counter + 1;
}

// Test 6: Nested control flow
let i = 0;
while (i < 2) {
    if (i == 0) {
        print(800);
    } else {
        print(900);
    }
    i = i + 1;
}

// Test 7: Boolean values
let t = true;
let f = false;
if (t) {
    print(1000);
}
if (!f) {
    print(1100);
}
