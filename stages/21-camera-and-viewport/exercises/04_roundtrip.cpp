// Exercise 04 — world->screen->world round-trip (medium)
//
// screen_to_world and world_to_screen must be exact inverses: convert a world
// point to a pixel and back, and you should land on the same world point. This
// "round-trip property" is the same testing idea as save/load in stage 13 —
// you don't hard-code the middle value, you assert the pair cancels out.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/camera.cpp 04_roundtrip.cpp -o ex && ./ex
//
// TODO:
//   1. Convert `start` to a pixel with world_to_screen, then back to world with
//      screen_to_world. Store the result in `back`.

#include <cassert>
#include <cmath>
#include <iostream>

#include "game/camera.hpp"

using game::screen_to_world;
using game::world_to_screen;
using gmath::Vec2f;

int main() {
    const Vec2f center{1000.0f, 750.0f};
    const Vec2f view{400.0f, 300.0f};  // a zoomed-in view
    const Vec2f window{800.0f, 600.0f};

    const Vec2f start{980.0f, 720.0f};

    Vec2f back{0.0f, 0.0f};
    // TODO 1: pixel = world_to_screen(start, ...); back = screen_to_world(pixel, ...);
    (void)start;

    std::cout << "start=(" << start.x << "," << start.y << ")  back=(" << back.x << "," << back.y
              << ")\n";
    assert(std::fabs(back.x - start.x) < 1e-3f);
    assert(std::fabs(back.y - start.y) < 1e-3f);
    std::cout << "all checks passed\n";
    return 0;
}
