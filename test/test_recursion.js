// Test multiple recursive functions

// Fibonacci
let fib = function(n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
};

print(fib(10));

// Greatest Common Divisor (Euclidean algorithm)
let gcd = function(a, b) {
    if (b == 0) {
        return a;
    }
    let quotient = a / b;
    return gcd(b, a - b * quotient);
};

print(gcd(48, 18));

// Power function
let pow = function(base, exp) {
    if (exp == 0) {
        return 1;
    }
    return base * pow(base, exp - 1);
};

print(pow(2, 10));

// Sum of array (recursive)
let sum = function(arr, idx) {
    if (idx >= arr.length) {
        return 0;
    }
    return arr[idx] + sum(arr, idx + 1);
};

let numbers = [1, 2, 3, 4, 5];
print(sum(numbers, 0));
