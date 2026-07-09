// Exercise 05 — solution
#include <cassert>
#include <iostream>

#include "game/gmath.hpp"

using gmath::Vec2f;

Vec2f parallax_offset(Vec2f camera_center, float factor) {
    return Vec2f{camera_center.x * factor, camera_center.y * factor};
}

int main() {
    const Vec2f cam{1000.0f, 400.0f};

    const Vec2f sky = parallax_offset(cam, 0.0f);
    const Vec2f hills = parallax_offset(cam, 0.5f);
    const Vec2f world = parallax_offset(cam, 1.0f);

    assert(sky.x == 0.0f && sky.y == 0.0f);
    assert(hills.x == 500.0f && hills.y == 200.0f);
    assert(world.x == 1000.0f && world.y == 400.0f);

    assert(hills.x < world.x);
    assert(sky.x < hills.x);

    std::cout << "all checks passed\n";
    return 0;
}
