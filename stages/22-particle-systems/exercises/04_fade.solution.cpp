// Exercise 04 — solution
#include <cassert>
#include <iostream>

int fade_alpha(float life, float max_life) {
    if (max_life <= 0.0f) {
        return 0;  // guard against divide-by-zero
    }
    float ratio = life / max_life;
    if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    return static_cast<int>(ratio * 255.0f);
}

int main() {
    assert(fade_alpha(1.0f, 1.0f) == 255);
    assert(fade_alpha(0.5f, 1.0f) == 127);
    assert(fade_alpha(0.0f, 1.0f) == 0);
    assert(fade_alpha(-1.0f, 1.0f) == 0);
    assert(fade_alpha(5.0f, 1.0f) == 255);
    assert(fade_alpha(1.0f, 0.0f) == 0);
    std::cout << "all checks passed\n";
    return 0;
}
