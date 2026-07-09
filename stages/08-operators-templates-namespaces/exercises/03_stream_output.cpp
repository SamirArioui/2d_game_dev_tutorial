// Exercise 03 — Overloading operator<< for printing (medium)
//
// To make `std::cout << myObject` work, overload operator<< as a FREE function (not a member):
// its left operand is the stream, not your type. It takes the stream by reference and your value
// by const&, and RETURNS the stream so calls can chain (a << b << c).
//
// This is the rough equivalent of Python's __repr__ / __str__.
//
// TODO:
//   1. Implement operator<< so it prints "Vec2i(x, y)".
//   2. Return `os` so `std::cout << a << "\n"` chains correctly.
//   3. In main, print a couple of Vec2i values with std::cout << v.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_stream_output.cpp -o ex && ./ex

#include <iostream>

struct Vec2i {
    int x;
    int y;
};

// A free function: the left operand is std::ostream&, so this can't be a member of Vec2i.
std::ostream& operator<<(std::ostream& os, const Vec2i& v) {
    // TODO: os << "Vec2i(" << v.x << ", " << v.y << ")";  then  return os;
    return os;
}

int main() {
    Vec2i a{3, 4};
    Vec2i b{-1, 0};
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    return 0;
}
