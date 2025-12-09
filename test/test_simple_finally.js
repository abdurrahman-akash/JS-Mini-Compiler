function test() {
    try {
        console.log("In try");
        return 42;
    } finally {
        console.log("In finally");
    }
}

console.log("Result: " + test());
console.log("Done");
