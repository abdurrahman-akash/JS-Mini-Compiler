let f = function(n) {
    print(n);
    if (n <= 0) {
        return 100;
    }
    return f(n - 1);
};

print(f(3));
