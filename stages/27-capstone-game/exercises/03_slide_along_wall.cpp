// Exercise 03 — Slide along a wall (medium)
//
// resolve_move handles the X and Y axes SEPARATELY. That is what lets a body
// pressing diagonally into a wall keep sliding along it instead of sticking.
// Predict the result of a diagonal move where X is blocked but Y is free.

#include <catch2/catch_test_macros.hpp>

#include "eng/collision.hpp"
#include "eng/tilemap.hpp"
#include "world.hpp"

using gmath::AABB;
using gmath::Vec2f;

TEST_CASE("blocked on X, still slides on Y", "[ex03]") {
    eng::Tilemap map;
    REQUIRE(map.load_from_string(
        "4 3 32\n"
        "0 0 1 0\n"
        "0 0 1 0\n"
        "0 0 1 0\n"));

    const AABB body{{10.0f, 40.0f}, {16.0f, 16.0f}};

    // Move right (blocked by the wall) AND down (nothing there).
    const Vec2f result = game::resolve_move(map, body, Vec2f{60.0f, 10.0f});

    // TODO: X is stopped flush against the wall (same as exercise 02)...
    REQUIRE(result.x == 0.0f);  // <-- fix

    // TODO: ...but the downward move is unobstructed, so Y advances fully.
    REQUIRE(result.y == 0.0f);  // <-- fix
}
