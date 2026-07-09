// Exercise 05 — Tilemap parsing & queries (medium-hard)
//
// Tilemap is pure data: it parses a "W H tileSize" header + rows of ids, then
// answers tile_at()/solid_at() questions. Parse the little map below and fill in
// the expected answers.

#include <catch2/catch_test_macros.hpp>

#include "eng/tilemap.hpp"

using eng::Tilemap;
using gmath::Vec2f;

TEST_CASE("parse and query a small map", "[ex05]") {
    // 4 wide, 3 tall, 32px tiles. 1 = wall (solid by default), 0 = floor.
    const char* text =
        "4 3 32\n"
        "1 1 1 1\n"
        "1 0 0 1\n"
        "1 1 1 1\n";

    Tilemap map;
    REQUIRE(map.load_from_string(text));

    // TODO: fill in the map dimensions.
    REQUIRE(map.width() == 0);      // <-- fix
    REQUIRE(map.height() == 0);     // <-- fix
    REQUIRE(map.tile_size() == 0);  // <-- fix

    // TODO: what tile id is at cell (1,1)? (the interior)
    REQUIRE(map.tile_at(1, 1) == -999);  // <-- fix

    // TODO: cell (1,1) is at pixel (32..64, 32..64). Is that floor solid?
    REQUIRE(map.solid_at(Vec2f{48.0f, 48.0f}) == true);  // <-- fix

    // TODO: a pixel in the top-left corner tile (a wall) — solid?
    REQUIRE(map.solid_at(Vec2f{10.0f, 10.0f}) == false);  // <-- fix

    // Off the map counts as solid (can't walk off the edge).
    REQUIRE(map.solid_at(Vec2f{-5.0f, -5.0f}) == true);
}
