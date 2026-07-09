// Exercise 05 — swap by pointer and by reference (medium)
//
// C++ passes arguments BY VALUE by default: a function gets a COPY, so changes inside don't
// leak back to the caller. To let a function modify the caller's variables you pass either a
// POINTER (and dereference it) or a REFERENCE (and just use it).
//
// PYTHON NOTE: Python can't swap two ints via a function without returning a tuple, because
// rebinding a parameter name inside the function doesn't touch the caller's name. C++ pointers
// and references give the function real access to the caller's variables.
//
// TODO:
//   1. Implement swap_ptr(int* a, int* b) using dereference (*a, *b) to exchange the values.
//   2. Implement swap_ref(int& a, int& b) using the references directly.
//   3. In main, swap two pairs of ints and print before/after.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_swap.cpp -o ex && ./ex

#include <iostream>

void swap_ptr(int* a, int* b) {
    // TODO: exchange *a and *b
}

void swap_ref(int& a, int& b) {
    // TODO: exchange a and b
}

int main() {
    int x = 1, y = 2;
    // TODO: call swap_ptr(&x, &y) and print x, y

    int m = 10, n = 20;
    // TODO: call swap_ref(m, n) and print m, n
    return 0;
}
