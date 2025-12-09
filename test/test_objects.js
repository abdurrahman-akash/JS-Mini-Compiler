function id(n) {
    return n;
}

function caller() {
    return id(1) + id(0);
}

print(caller());
