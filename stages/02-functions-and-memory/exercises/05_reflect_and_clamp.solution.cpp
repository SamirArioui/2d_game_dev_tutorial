// Exercise 05 — reflect() + overloaded clamp() (solution)
#include <iostream>

// Reference out-parameters: reflect updates the caller's vx and vy directly (no return).
void reflect(float& vx, float& vy) {
    vx = -vx;
    vy = -vy;
}

// Overload #1: integer clamp.
int clamp(int v, int lo, int hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

// Overload #2: same name, float parameters. The compiler picks by argument type.
float clamp(float v, float lo, float hi) {
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

int main() {
    float vx = 3.0f;
    float vy = -2.0f;

    reflect(vx, vy);   // flips both in place
    std::cout << "After reflect: vx=" << vx << " vy=" << vy << "\n";  // -3 2

    std::cout << "clamp(150, 0, 100)        = " << clamp(150, 0, 100) << "\n";          // 100 (int)
    std::cout << "clamp(-4.5f, 0.0f, 10.0f) = " << clamp(-4.5f, 0.0f, 10.0f) << "\n";   // 0   (float)

    return 0;
}
