// Exercise 05 — swap by pointer and by reference (solution)
#include <iostream>

// Pointer version: the caller must pass ADDRESSES (swap_ptr(&x, &y)); we work through *.
void swap_ptr(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Reference version: the caller passes variables directly (swap_ref(m, n)); cleaner to read
// and guaranteed non-null. This is the idiomatic C++ choice when "no object" is impossible.
void swap_ref(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

int main() {
    int x = 1, y = 2;
    swap_ptr(&x, &y);
    std::cout << "after swap_ptr: x=" << x << " y=" << y << "\n";

    int m = 10, n = 20;
    swap_ref(m, n);
    std::cout << "after swap_ref: m=" << m << " n=" << n << "\n";
    return 0;
}
