// Exercise 01 — Overloading operator+ (solution)
#include <iostream>

struct Color {
    int r;
    int g;
    int b;

    // const method (it reads *this, doesn't change it), other by const& (no copy), returns a new
    // Color. Same shape as the Vec2 operators in the mini-project.
    Color operator+(const Color& other) const {
        return Color{r + other.r, g + other.g, b + other.b};
    }
};

int main() {
    Color red{255, 0, 0};
    Color green{0, 128, 0};
    Color sum = red + green;
    std::cout << "sum = (" << sum.r << ", " << sum.g << ", " << sum.b << ")\n";  // (255, 128, 0)
    return 0;
}
