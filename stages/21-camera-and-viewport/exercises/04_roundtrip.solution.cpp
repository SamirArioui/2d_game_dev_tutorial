// Exercise 04 — solution
#include <cassert>
#include <cmath>
#include <iostream>

#include "game/camera.hpp"

using game::screen_to_world;
using game::world_to_screen;
using gmath::Vec2f;

int main() {
    const Vec2f center{1000.0f, 750.0f};
    const Vec2f view{400.0f, 300.0f};
    const Vec2f window{800.0f, 600.0f};

    const Vec2f start{980.0f, 720.0f};

    const Vec2f pixel = world_to_screen(start, center, view, window);
    const Vec2f back = screen_to_world(pixel, center, view, window);

    std::cout << "start=(" << start.x << "," << start.y << ")  back=(" << back.x << "," << back.y
              << ")\n";
    assert(std::fabs(back.x - start.x) < 1e-3f);
    assert(std::fabs(back.y - start.y) < 1e-3f);
    std::cout << "all checks passed\n";
    return 0;
}
