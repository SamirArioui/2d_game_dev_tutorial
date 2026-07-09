// Exercise 03 — clamp the camera to the level bounds (medium)
//
// A follow-camera centers on the player, but naively that shows empty space
// past the level edges. game::clamp_view_center fixes the target center so the
// view stays inside the level (or centers on a level smaller than the view).
// Here you USE it and reason about the answers.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/camera.cpp 03_clamp_view.cpp -o ex && ./ex
//
// TODO:
//   1. Fill in the expected clamped center for a target near the top-left corner
//      of the level (think: the center can't go below half the view size).

#include <cassert>
#include <iostream>

#include "game/camera.hpp"

using game::clamp_view_center;
using gmath::Vec2f;

int main() {
    const Vec2f view{800.0f, 600.0f};
    const Vec2f level{1280.0f, 640.0f};  // the mini-project's level size

    // A target in the middle is left alone.
    const Vec2f mid = clamp_view_center(Vec2f{640.0f, 320.0f}, view, level);
    assert(mid.x == 640.0f && mid.y == 320.0f);

    // A target at the very top-left corner (0,0): the center is pushed in to
    // half the view size so the view's edge sits on the level's edge.
    const Vec2f corner = clamp_view_center(Vec2f{0.0f, 0.0f}, view, level);
    // TODO 1: replace the two zeros with the expected clamped center.
    const float expected_x = 0.0f;  // <-- half of the view width?
    const float expected_y = 0.0f;  // <-- half of the view height?
    std::cout << "corner clamp = (" << corner.x << ", " << corner.y << ")\n";
    assert(corner.x == expected_x && corner.y == expected_y);

    std::cout << "all checks passed\n";
    return 0;
}
