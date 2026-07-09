// Exercise 04 — new / delete and the heap (medium)
//
// Local variables live on the STACK: born when a function is entered, gone when it returns.
// Sometimes you need memory that OUTLIVES the function that made it, or whose size you only
// know at run time. That memory comes from the HEAP via `new`, and YOU must hand it back with
// `delete` (or `delete[]` for arrays). Every `new` needs exactly one matching `delete`.
//
// PYTHON NOTE: Python puts every object on the heap for you and a garbage collector frees it
// once no names refer to it. In C++ (before smart pointers, stage 11) you own the lifetime:
// forget to delete -> memory leak; delete twice -> corruption.
//
// TODO:
//   1. Allocate a single int on the heap holding 50 (int* hp = new int(50);). Print *hp,
//      then delete it.
//   2. Allocate an array of `n` ints with new[], fill entry i with i*i, print them, delete[] it.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_new_delete.cpp -o ex && ./ex

#include <iostream>

int main() {
    // TODO 1: a single heap int.

    // TODO 2: a heap array of n ints.
    int n = 5;
    (void)n;  // (remove this once you use n)

    return 0;
}
