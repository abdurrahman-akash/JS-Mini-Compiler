function fib0() {
    return 0;
}

function fib1() {
    return 1;
}

function fib2() {
    return fib1() + fib0();
}

print(fib2());
