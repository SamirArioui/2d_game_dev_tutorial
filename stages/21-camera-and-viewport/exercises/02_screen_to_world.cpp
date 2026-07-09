// Exercise 02 — click-to-world: convert a screen pixel to a world coordinate
// (medium)
//
// A mouse click arrives in SCREEN pixels (0..window). But the thing it should
// select lives in WORLD space, which the camera has scrolled and possibly
// zoomed. The conversion is:
//
//   top_left = view_center - view_size/2          (world corner the view shows)
//   world    = top_left + (pixel / window_size) * view_size
//
// This is exactly what sf::RenderWindow::mapPixelToCoords computes. Doing it by
// hand once makes that call stop being magic.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include 02_screen_to_world.cpp -o ex && ./ex
//
// TODO:
//   1. Implement screen_to_world using the formula above.

#include <cassert>
#include <iostream>

#include "game/gmath.hpp"

using gmath::Vec2f;

Vec2f screen_to_world(Vec2f pixel, Vec2f view_center, Vec2f view_size, Vec2f window_size) {
    // TODO 1: compute the view's top-left world corner, then map the pixel.
    (void)pixel;
    (void)view_center;
    (void)view_size;
    (void)window_size;
    return Vec2f{0.0f, 0.0f};  // <-- replace
}

int main() {
    const Vec2f center{1000.0f, 750.0f};
    const Vec2f view{800.0f, 600.0f};
    const Vec2f window{800.0f, 600.0f};

    // The center pixel maps to the view center.
    const Vec2f mid = screen_to_world(Vec2f{400.0f, 300.0f}, center, view, window);
    assert(mid.x == 1000.0f && mid.y == 750.0f);

    // The top-left pixel maps to center - view/2.
    const Vec2f tl = screen_to_world(Vec2f{0.0f, 0.0f}, center, view, window);
    assert(tl.x == 600.0f && tl.y == 450.0f);

    // A zoomed-in view (half size) still works.
    const Vec2f zoom{400.0f, 300.0f};
    const Vec2f z = screen_to_world(Vec2f{800.0f, 600.0f}, center, zoom, window);
    assert(z.x == 1200.0f && z.y == 900.0f);

    std::cout << "all checks passed\n";
    return 0;
}
