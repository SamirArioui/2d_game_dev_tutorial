// Exercise 03 — Overloading operator<< for printing (solution)
#include <iostream>

struct Vec2i {
    int x;
    int y;
};

std::ostream& operator<<(std::ostream& os, const Vec2i& v) {
    os << "Vec2i(" << v.x << ", " << v.y << ")";
    return os;   // returning the stream is what lets `cout << a << "\n"` chain
}

int main() {
    Vec2i a{3, 4};
    Vec2i b{-1, 0};
    std::cout << "a = " << a << "\n";   // a = Vec2i(3, 4)
    std::cout << "b = " << b << "\n";   // b = Vec2i(-1, 0)
    return 0;
}
