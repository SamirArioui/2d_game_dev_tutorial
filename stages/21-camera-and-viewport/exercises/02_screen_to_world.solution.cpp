// Exercise 02 — solution
#include <cassert>
#include <iostream>

#include "game/gmath.hpp"

using gmath::Vec2f;

Vec2f screen_to_world(Vec2f pixel, Vec2f view_center, Vec2f view_size, Vec2f window_size) {
    const Vec2f top_left{view_center.x - view_size.x * 0.5f, view_center.y - view_size.y * 0.5f};
    return Vec2f{top_left.x + (pixel.x / window_size.x) * view_size.x,
                 top_left.y + (pixel.y / window_size.y) * view_size.y};
}

int main() {
    const Vec2f center{1000.0f, 750.0f};
    const Vec2f view{800.0f, 600.0f};
    const Vec2f window{800.0f, 600.0f};

    const Vec2f mid = screen_to_world(Vec2f{400.0f, 300.0f}, center, view, window);
    assert(mid.x == 1000.0f && mid.y == 750.0f);

    const Vec2f tl = screen_to_world(Vec2f{0.0f, 0.0f}, center, view, window);
    assert(tl.x == 600.0f && tl.y == 450.0f);

    const Vec2f zoom{400.0f, 300.0f};
    const Vec2f z = screen_to_world(Vec2f{800.0f, 600.0f}, center, zoom, window);
    assert(z.x == 1200.0f && z.y == 900.0f);

    std::cout << "all checks passed\n";
    return 0;
}
