// Exercise 01 — solution
#include <cassert>
#include <iostream>

float clamp(float value, float lo, float hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

int main() {
    assert(clamp(-5.0f, 0.0f, 10.0f) == 0.0f);
    assert(clamp(42.0f, 0.0f, 10.0f) == 10.0f);
    assert(clamp(3.0f, 0.0f, 10.0f) == 3.0f);
    assert(clamp(0.0f, 0.0f, 10.0f) == 0.0f);
    assert(clamp(10.0f, 0.0f, 10.0f) == 10.0f);
    std::cout << "all checks passed\n";
    return 0;
}
