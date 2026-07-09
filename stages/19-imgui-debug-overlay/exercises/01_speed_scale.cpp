// Exercise 01 — rescale a velocity to a target speed  (PURE, no SFML)
//
// This is exactly what the overlay's "Ball speed" slider does: change HOW FAST
// the ball moves without changing WHICH WAY it moves. The trick is to normalize
// the velocity to a unit direction, then multiply by the new speed.
//
//   clang++ -std=c++17 -Wall -Wextra 01_speed_scale.cpp -o ex && ./ex
//
// TODO:
//   Implement set_speed(). Compute the length; if it's > 0, scale each
//   component by (speed / length). If the vector is zero (no direction), fall
//   back to a rightward launch: {speed, 0}.

#include <cassert>
#include <cmath>
#include <iostream>

struct V {
    float x, y;
};

float length(V v) { return std::sqrt(v.x * v.x + v.y * v.y); }

V set_speed(V v, float speed) {
    // TODO: preserve direction, set magnitude to `speed`; handle the zero case.
    (void)speed;
    return v;
}

int main() {
    V a = set_speed(V{3.0f, 4.0f}, 10.0f);  // length 5 -> scale by 2
    assert(std::abs(length(a) - 10.0f) < 1e-3f);
    assert(std::abs(a.x - 6.0f) < 1e-3f);
    assert(std::abs(a.y - 8.0f) < 1e-3f);

    V z = set_speed(V{0.0f, 0.0f}, 5.0f);  // no direction -> go right
    assert(std::abs(length(z) - 5.0f) < 1e-3f);
    assert(std::abs(z.x - 5.0f) < 1e-3f);
    assert(std::abs(z.y - 0.0f) < 1e-3f);

    std::cout << "01_speed_scale: all assertions passed\n";
    return 0;
}
