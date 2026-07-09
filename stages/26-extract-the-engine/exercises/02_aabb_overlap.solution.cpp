// Exercise 02 — AABB overlap (solution)
#include <catch2/catch_test_macros.hpp>

#include "eng/collision.hpp"

using gmath::AABB;
using gmath::Hit;
using gmath::Vec2f;

TEST_CASE("overlap and minimum-translation vector", "[ex02]") {
    const AABB a{{0.0f, 0.0f}, {10.0f, 10.0f}};
    const AABB b{{7.0f, 3.0f}, {10.0f, 10.0f}};

    const Hit hit = gmath::aabb_vs_aabb(a, b);

    REQUIRE(hit.collided == true);
    // Overlap is 3 on X, 7 on Y -> resolve on X. a is left of b -> push left.
    REQUIRE(hit.penetration == Vec2f{-3.0f, 0.0f});

    // Applying the penetration must separate them.
    const AABB moved{a.pos + hit.penetration, a.size};
    REQUIRE_FALSE(gmath::aabb_vs_aabb(moved, b).collided);
}
