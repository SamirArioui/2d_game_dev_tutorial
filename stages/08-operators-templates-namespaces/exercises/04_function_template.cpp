// Exercise 04 — Function templates (medium)
//
// A function template writes an algorithm ONCE for many types. The compiler generates a concrete
// version for each type you actually call it with, and type-checks each one. This is the
// compile-time cousin of Python's duck typing: `max_of` works for any type with `<`, but a
// mismatch is caught when you compile, not when the program runs.
//
// PYTHON NOTE: in Python `def max_of(a, b): return a if a > b else b` already works for any
// comparable type. C++ needs `template <class T>` to get the same flexibility WITH static typing.
//
// TODO:
//   1. Implement max_of<T>(a, b): return the larger (uses operator<).
//   2. Implement clamp<T>(v, lo, hi): return lo if v < lo, hi if v > hi, else v. (Previews the
//      gmath::clamp in the mini-project.)
//   3. Note in main how the SAME templates are called with int and double — no <T> needed, the
//      compiler deduces T from the arguments.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_function_template.cpp -o ex && ./ex

#include <iostream>

template <class T>
T max_of(T a, T b) {
    // TODO: return the larger of a and b
    return a;  // <-- replace
}

template <class T>
T clamp(T v, T lo, T hi) {
    // TODO: confine v to [lo, hi]
    return v;  // <-- replace
}

int main() {
    std::cout << "max_of(3, 9)        = " << max_of(3, 9) << "\n";           // int
    std::cout << "max_of(2.5, 1.5)    = " << max_of(2.5, 1.5) << "\n";       // double
    std::cout << "clamp(15, 0, 10)    = " << clamp(15, 0, 10) << "\n";       // int
    std::cout << "clamp(-1.0,0.0,9.0) = " << clamp(-1.0, 0.0, 9.0) << "\n";  // double
    return 0;
}
