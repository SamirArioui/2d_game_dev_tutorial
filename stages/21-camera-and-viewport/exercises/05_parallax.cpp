// Exercise 05 — parallax scroll offsets (medium)
//
// Parallax fakes depth: layers "further away" scroll slower than the camera. A
// layer at depth `factor` (0..1) has scrolled `camera_center * factor`. The sky
// (factor 0) never moves; the foreground (factor 1) moves with the world; the
// distant hills (small factor) drift slowly.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include 05_parallax.cpp -o ex && ./ex
//
// TODO:
//   1. Implement parallax_offset: return camera_center scaled by factor.

#include <cassert>
#include <iostream>

#include "game/gmath.hpp"

using gmath::Vec2f;

Vec2f parallax_offset(Vec2f camera_center, float factor) {
    // TODO 1: scale both components by factor.
    (void)camera_center;
    (void)factor;
    return Vec2f{0.0f, 0.0f};  // <-- replace
}

int main() {
    const Vec2f cam{1000.0f, 400.0f};

    // Factors chosen to be exactly representable in float, so we can use ==.
    const Vec2f sky = parallax_offset(cam, 0.0f);
    const Vec2f hills = parallax_offset(cam, 0.5f);
    const Vec2f world = parallax_offset(cam, 1.0f);

    assert(sky.x == 0.0f && sky.y == 0.0f);          // pinned
    assert(hills.x == 500.0f && hills.y == 200.0f);  // half speed
    assert(world.x == 1000.0f && world.y == 400.0f); // full speed

    // The nearer layer must always have scrolled at least as far as the farther.
    assert(hills.x < world.x);
    assert(sky.x < hills.x);

    std::cout << "all checks passed\n";
    return 0;
}
