// Exercise 04 — fade a particle out over its life (medium)
//
// Particles look best when they fade instead of vanishing. The alpha (opacity,
// 0..255) is just the fraction of life remaining, scaled up:
//
//   ratio = clamp(life / max_life, 0, 1)
//   alpha = ratio * 255
//
// This is the value the renderer puts in each particle's colour. Here you write
// it as a pure function (no SFML needed).
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra 04_fade.cpp -o ex && ./ex
//
// TODO:
//   1. Compute the clamped life ratio and return it times 255 as an int.
//      Guard against max_life <= 0 (return 0 to avoid dividing by zero).

#include <cassert>
#include <iostream>

int fade_alpha(float life, float max_life) {
    // TODO 1: ratio = clamp(life/max_life, 0, 1); return int(ratio * 255).
    (void)life;
    (void)max_life;
    return 255;  // <-- replace
}

int main() {
    assert(fade_alpha(1.0f, 1.0f) == 255);  // fresh -> fully opaque
    assert(fade_alpha(0.5f, 1.0f) == 127);  // half life -> ~half alpha (127.5 -> 127)
    assert(fade_alpha(0.0f, 1.0f) == 0);    // dead -> transparent
    assert(fade_alpha(-1.0f, 1.0f) == 0);   // past death clamps to 0
    assert(fade_alpha(5.0f, 1.0f) == 255);  // over-full clamps to 255
    assert(fade_alpha(1.0f, 0.0f) == 0);    // guard: no divide-by-zero
    std::cout << "all checks passed\n";
    return 0;
}
