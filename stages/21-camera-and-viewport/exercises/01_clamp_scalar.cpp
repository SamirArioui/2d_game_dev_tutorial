// Exercise 01 — clamp a value to a range (easy)
//
// Clamping is the atom of camera work: to keep the camera inside the level you
// clamp its center; to keep a bar from overflowing you clamp its width. Here
// you write `clamp` from scratch.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra 01_clamp_scalar.cpp -o ex && ./ex
//
// TODO:
//   1. Return `lo` if value < lo, `hi` if value > hi, otherwise value itself.

#include <cassert>
#include <iostream>

float clamp(float value, float lo, float hi) {
    // TODO 1: implement the three cases.
    return value;  // <-- replace
}

int main() {
    assert(clamp(-5.0f, 0.0f, 10.0f) == 0.0f);   // below -> lo
    assert(clamp(42.0f, 0.0f, 10.0f) == 10.0f);  // above -> hi
    assert(clamp(3.0f, 0.0f, 10.0f) == 3.0f);    // inside -> unchanged
    assert(clamp(0.0f, 0.0f, 10.0f) == 0.0f);    // endpoints inclusive
    assert(clamp(10.0f, 0.0f, 10.0f) == 10.0f);
    std::cout << "all checks passed\n";
    return 0;
}
