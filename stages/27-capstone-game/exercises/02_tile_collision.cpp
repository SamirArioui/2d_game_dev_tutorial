// Exercise 02 — Tile collision (medium)
//
// resolve_move slides a body by a delta but stops it at solid tiles. Predict
// where a body ends up when it walks into a wall.

#include <catch2/catch_test_macros.hpp>

#include "eng/collision.hpp"
#include "eng/tilemap.hpp"
#include "world.hpp"

using gmath::AABB;
using gmath::Vec2f;

TEST_CASE("body stops at the wall's edge", "[ex02]") {
    // 4x3 map, 32px tiles. Column x=2 (world 64..96) is a solid wall.
    eng::Tilemap map;
    REQUIRE(map.load_from_string(
        "4 3 32\n"
        "0 0 1 0\n"
        "0 0 1 0\n"
        "0 0 1 0\n"));

    // 16px body in the floor, moving right by 60px toward the wall at x=64.
    const AABB body{{10.0f, 40.0f}, {16.0f, 16.0f}};
    const Vec2f result = game::resolve_move(map, body, Vec2f{60.0f, 0.0f});

    // TODO: the body's RIGHT edge stops on the wall (x=64), so its LEFT edge is...?
    REQUIRE(result.x == 0.0f);   // <-- fix
    REQUIRE(result.y == 0.0f);   // <-- fix (no vertical move was requested)
}
