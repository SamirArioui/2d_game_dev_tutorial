// Exercise 02 — Tile collision (solution)
#include <catch2/catch_test_macros.hpp>

#include "eng/collision.hpp"
#include "eng/tilemap.hpp"
#include "world.hpp"

using gmath::AABB;
using gmath::Vec2f;

TEST_CASE("body stops at the wall's edge", "[ex02]") {
    eng::Tilemap map;
    REQUIRE(map.load_from_string(
        "4 3 32\n"
        "0 0 1 0\n"
        "0 0 1 0\n"
        "0 0 1 0\n"));

    const AABB body{{10.0f, 40.0f}, {16.0f, 16.0f}};
    const Vec2f result = game::resolve_move(map, body, Vec2f{60.0f, 0.0f});

    REQUIRE(result.x == 48.0f);  // right edge 48+16 = 64 = wall's left side
    REQUIRE(result.y == 40.0f);
}
