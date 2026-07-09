// Exercise 03 — walk the grid with tile_at (medium)
//
// Now use the real Tilemap class. `tile_at(x, y)` returns the id at a cell (0
// off the map), and `is_solid_id(id)` says whether an id is solid (non-zero).
// Count how many solid tiles a map has by visiting every cell.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/tilemap.cpp 03_count_solid.cpp -o ex && ./ex
//
// TODO:
//   1. Loop y over [0, height), x over [0, width). For each cell, if the tile
//      is solid, increment `solid`. Return the total.

#include <cassert>
#include <iostream>
#include <string>

#include "game/tilemap.hpp"

using game::Tilemap;

int count_solid(const Tilemap& map) {
    int solid = 0;
    // TODO 1: nested loop over the grid, count solid tiles via tile_at + is_solid_id.
    (void)map;
    return solid;
}

int main() {
    // 4x3 walled room with two interior holes: 10 solid border/edge tiles, 2 empty.
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
