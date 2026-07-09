// Exercise 04 — Function overloading (medium)
//
// C++ lets TWO functions share the same NAME as long as their parameter lists differ.
// The compiler picks the right one based on the argument types you pass. This is called
// OVERLOADING. (Return type alone is NOT enough to tell them apart — the parameters must.)
//
//   int    square(int x)    { return x * x; }
//   double square(double x) { return x * x; }
//   square(4);     // calls the int version
//   square(2.5);   // calls the double version
//
// Python does NOT have this — a second `def square` simply replaces the first. Python
// leans on dynamic typing / default args / *args instead.
//
// TODO:
//   1. Define two overloads of  describe:
//        void describe(int level)    -> prints  "Level 5 (integer)"
//        void describe(double ratio) -> prints  "Ratio 0.75 (real number)"
//   2. Define two overloads of  max_of:
//        int    max_of(int a, int b)       -> returns the larger int
//        double max_of(double a, double b) -> returns the larger double
//   3. In main, call describe(5), describe(0.75), and print max_of(3, 9) and max_of(2.5, 1.5).
//      Notice you never say which overload to use — the argument types decide.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_overloading.cpp -o ex && ./ex

#include <iostream>

// TODO: define the describe() overloads and the max_of() overloads.

int main() {
    // TODO: call describe(5); describe(0.75);
    // TODO: print max_of(3, 9) and max_of(2.5, 1.5).
    return 0;
}
