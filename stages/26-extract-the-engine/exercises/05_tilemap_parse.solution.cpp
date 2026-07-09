// Exercise 05 — Tilemap parsing & queries (solution)
#include <catch2/catch_test_macros.hpp>

#include "eng/tilemap.hpp"

using eng::Tilemap;
using gmath::Vec2f;

TEST_CASE("parse and query a small map", "[ex05]") {
    const char* text =
        "4 3 32\n"
        "1 1 1 1\n"
        "1 0 0 1\n"
        "1 1 1 1\n";

    Tilemap map;
    REQUIRE(map.load_from_string(text));

    REQUIRE(map.width() == 4);
    REQUIRE(map.height() == 3);
    REQUIRE(map.tile_size() == 32);

    REQUIRE(map.tile_at(1, 1) == 0);  // interior floor

    REQUIRE(map.solid_at(Vec2f{48.0f, 48.0f}) == false);  // floor is not solid
    REQUIRE(map.solid_at(Vec2f{10.0f, 10.0f}) == true);   // corner wall is solid
    REQUIRE(map.solid_at(Vec2f{-5.0f, -5.0f}) == true);   // off the map = solid
}
