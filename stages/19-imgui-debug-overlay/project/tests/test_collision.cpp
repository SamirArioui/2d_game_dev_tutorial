// ============================================================================
// Tests for the AABB overlap test (carried from stage 16, re-verified here).
//
// Collision is pure geometry, so it is some of the most satisfying code to
// test: every case has one correct answer. Pong leans on this directly
// (ball vs paddle), so we want it rock-solid before building the game on top.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "collision/aabb.hpp"

using collision::AABB;
using collision::aabb_vs_aabb;
using gmath::Vec2f;

TEST_CASE("overlapping boxes report a hit with penetration", "[aabb]") {
    const AABB a{Vec2f{0, 0}, Vec2f{10, 10}};
    const AABB b{Vec2f{5, 5}, Vec2f{10, 10}};

    const auto hit = aabb_vs_aabb(a, b);
    REQUIRE(hit.has_value());
    // a's centre is up-left of b's, so a is pushed further up-left: (-5, -5).
    REQUIRE(hit->penetration.x == Catch::Approx(-5.0f));
    REQUIRE(hit->penetration.y == Catch::Approx(-5.0f));
}

TEST_CASE("penetration points away from the other box", "[aabb]") {
    // `a` overlaps `b` from the RIGHT this time, so it must be pushed right (+x).
    const AABB a{Vec2f{8, 0}, Vec2f{10, 10}};
    const AABB b{Vec2f{0, 0}, Vec2f{10, 10}};

    const auto hit = aabb_vs_aabb(a, b);
    REQUIRE(hit.has_value());
    REQUIRE(hit->penetration.x == Catch::Approx(2.0f));  // shallow overlap of 2, pushed right
}

TEST_CASE("separated boxes do not collide", "[aabb]") {
    const AABB a{Vec2f{0, 0}, Vec2f{10, 10}};
    const AABB b{Vec2f{20, 20}, Vec2f{10, 10}};
    REQUIRE_FALSE(aabb_vs_aabb(a, b).has_value());
}

TEST_CASE("boxes that merely touch are NOT overlapping", "[aabb][edge]") {
    // b's left edge sits exactly on a's right edge (x == 10).
    const AABB a{Vec2f{0, 0}, Vec2f{10, 10}};
    const AABB b{Vec2f{10, 0}, Vec2f{10, 10}};
    REQUIRE_FALSE(aabb_vs_aabb(a, b).has_value());
}
