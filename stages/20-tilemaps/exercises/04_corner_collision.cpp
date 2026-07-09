// Exercise 04 — world-space collision with solid_at (medium)
//
// `solid_at(world)` converts a WORLD position (pixels) to a grid cell and tells
// you if that cell is solid. A character is a box, not a point, so to decide if
// it can occupy a spot we check its FOUR CORNERS. This is exactly the check the
// mini-project uses to move the player.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/tilemap.cpp 04_corner_collision.cpp -o ex && ./ex
//
// TODO:
//   1. Return true if ANY of the box's four corners (top-left `pos`, size
//      `size`) lies on a solid tile. Use gmath::Vec2f and map.solid_at.

#include <cassert>
#include <iostream>
#include <string>

#include "game/gmath.hpp"
#include "game/tilemap.hpp"

using game::Tilemap;
using gmath::Vec2f;

bool box_blocked(const Tilemap& map, Vec2f pos, Vec2f size) {
    // TODO 1: build the 4 corners from pos/size and return true if any is solid.
    (void)map;
    (void)pos;
    (void)size;
    return false;
}

int main() {
    // 32px tiles. Border (id 1) is solid; the interior is open.
    const Tilemap map = Tilemap::from_text("4 3 32\n"
                                           "1 1 1 1\n"
                                           "1 0 0 1\n"
                                           "1 1 1 1\n");
    const Vec2f size{20.0f, 20.0f};

    // A box fully inside the open cell (1,1) -> world [32,64) x [32,64).
    assert(!box_blocked(map, Vec2f{36.0f, 36.0f}, size));

    // Nudge it left so its left edge crosses into the solid border column 0.
    assert(box_blocked(map, Vec2f{28.0f, 36.0f}, size));

    std::cout << "all checks passed\n";
    return 0;
}
