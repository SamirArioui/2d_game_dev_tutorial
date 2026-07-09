// Exercise 01 — Overloading operator+ (easy)
//
// C++ lets you define what the built-in operators mean for YOUR types. Writing `a + b` on a user
// type calls a function named `operator+`. This is exactly Python's __add__, under another name.
//
// PYTHON NOTE:
//   class Color:
//       def __add__(self, other):
//           return Color(self.r + other.r, self.g + other.g, self.b + other.b)
//   C++:  Color operator+(const Color& other) const { ... }
//
// TODO:
//   1. Implement operator+ so it adds the r/g/b channels pairwise.
//   2. In main, add two colours and print the result.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_operator_plus.cpp -o ex && ./ex

#include <iostream>

struct Color {
    int r;
    int g;
    int b;

    // TODO: return a Color whose channels are the sums of *this and other.
    Color operator+(const Color& other) const {
        return Color{0, 0, 0};  // <-- replace
    }
};

int main() {
    Color red{255, 0, 0};
    Color green{0, 128, 0};
    Color sum = red + green;
    std::cout << "sum = (" << sum.r << ", " << sum.g << ", " << sum.b << ")\n";
    return 0;
}
