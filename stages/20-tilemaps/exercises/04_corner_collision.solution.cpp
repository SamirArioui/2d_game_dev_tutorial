// Exercise 04 — solution
#include <cassert>
#include <iostream>
#include <string>

#include "game/gmath.hpp"
#include "game/tilemap.hpp"

using game::Tilemap;
using gmath::Vec2f;

bool box_blocked(const Tilemap& map, Vec2f pos, Vec2f size) {
    const Vec2f corners[4] = {
        {pos.x, pos.y},
        {pos.x + size.x, pos.y},
        {pos.x, pos.y + size.y},
        {pos.x + size.x, pos.y + size.y},
    };
    for (const Vec2f& corner : corners) {
        if (map.solid_at(corner)) {
            return true;
        }
    }
    return false;
}

int main() {
    const Tilemap map = Tilemap::from_text("4 3 32\n"
                                           "1 1 1 1\n"
                                           "1 0 0 1\n"
                                           "1 1 1 1\n");
    const Vec2f size{20.0f, 20.0f};

    assert(!box_blocked(map, Vec2f{36.0f, 36.0f}, size));
    assert(box_blocked(map, Vec2f{28.0f, 36.0f}, size));

    std::cout << "all checks passed\n";
    return 0;
}
