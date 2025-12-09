let fib = function(n) {
    if (n <= 1) {
        return n;
    }
    let a = fib(n - 1);
    let b = fib(n - 2);
    print(a);
    print(b);
    return a + b;
};

print(fib(3));
