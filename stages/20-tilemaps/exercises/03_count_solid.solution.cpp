// Exercise 03 — solution
#include <cassert>
#include <iostream>
#include <string>

#include "game/tilemap.hpp"

using game::Tilemap;

int count_solid(const Tilemap& map) {
    int solid = 0;
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            if (map.is_solid_id(map.tile_at(x, y))) {
                ++solid;
            }
        }
    }
    return solid;
}

int main() {
    const Tilemap map = Tilemap::from_text("4 3 32\n"
                                           "1 1 1 1\n"
                                           "1 0 0 1\n"
                                           "1 1 1 1\n");
    const int solid = count_solid(map);
    std::cout << "solid tiles = " << solid << " (expected 10)\n";
    assert(solid == 10);
    std::cout << "all checks passed\n";
    return 0;
}
