// Exercise 03 — solution
#include <cassert>
#include <iostream>

#include "game/camera.hpp"

using game::clamp_view_center;
using gmath::Vec2f;

int main() {
    const Vec2f view{800.0f, 600.0f};
    const Vec2f level{1280.0f, 640.0f};

    const Vec2f mid = clamp_view_center(Vec2f{640.0f, 320.0f}, view, level);
    assert(mid.x == 640.0f && mid.y == 320.0f);

    const Vec2f corner = clamp_view_center(Vec2f{0.0f, 0.0f}, view, level);
    const float expected_x = 400.0f;  // half of the 800-wide view
    const float expected_y = 300.0f;  // half of the 600-tall view
    std::cout << "corner clamp = (" << corner.x << ", " << corner.y << ")\n";
    assert(corner.x == expected_x && corner.y == expected_y);

    std::cout << "all checks passed\n";
    return 0;
}
