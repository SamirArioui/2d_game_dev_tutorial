// Exercise 02 — solution  (PURE, no SFML)
#include <cassert>
#include <cmath>
#include <iostream>

struct Vel {
    float x, y;
};

Vel bounce(Vel in, float offset) {
    // Reverse horizontal direction; vertical spin scales with the contact point.
    return Vel{-in.x, offset * std::abs(in.x)};
}

int main() {
    Vel a = bounce(Vel{-300.0f, 0.0f}, 0.0f);
    assert(a.x > 0.0f);
    assert(std::abs(a.y) < 1e-3f);

    Vel b = bounce(Vel{-300.0f, 0.0f}, 0.5f);
    assert(b.x > 0.0f && b.y > 0.0f);

    Vel c = bounce(Vel{300.0f, 0.0f}, -0.5f);
    assert(c.x < 0.0f && c.y < 0.0f);

    std::cout << "02_paddle_bounce: all assertions passed\n";
    return 0;
}
