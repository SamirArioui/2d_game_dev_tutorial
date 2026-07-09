// Exercise 01 — solution  (PURE, no SFML)
#include <cassert>
#include <cmath>
#include <iostream>

struct V {
    float x, y;
};

float length(V v) { return std::sqrt(v.x * v.x + v.y * v.y); }

V set_speed(V v, float speed) {
    const float len = length(v);
    if (len == 0.0f) {
        return V{speed, 0.0f};  // no direction -> default rightward
    }
    const float k = speed / len;  // scale factor to reach the target speed
    return V{v.x * k, v.y * k};
}

int main() {
    V a = set_speed(V{3.0f, 4.0f}, 10.0f);
    assert(std::abs(length(a) - 10.0f) < 1e-3f);
    assert(std::abs(a.x - 6.0f) < 1e-3f);
    assert(std::abs(a.y - 8.0f) < 1e-3f);

    V z = set_speed(V{0.0f, 0.0f}, 5.0f);
    assert(std::abs(length(z) - 5.0f) < 1e-3f);
    assert(std::abs(z.x - 5.0f) < 1e-3f);
    assert(std::abs(z.y - 0.0f) < 1e-3f);

    std::cout << "01_speed_scale: all assertions passed\n";
    return 0;
}
