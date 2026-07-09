// Exercise 02 — AABB overlap (easy-medium)
//
// gmath::aabb_vs_aabb returns a Hit with a `penetration` vector: the smallest
// move that separates box `a` from box `b`. Predict it for the setup below.

#include <catch2/catch_test_macros.hpp>

#include "eng/collision.hpp"

using gmath::AABB;
using gmath::Hit;
using gmath::Vec2f;

TEST_CASE("overlap and minimum-translation vector", "[ex02]") {
    // `a` sits at the origin (10x10). `b` is shifted so the boxes overlap by 3
    // on X and 7 on Y. Resolution happens along the SMALLER-overlap axis.
    const AABB a{{0.0f, 0.0f}, {10.0f, 10.0f}};
    const AABB b{{7.0f, 3.0f}, {10.0f, 10.0f}};

    const Hit hit = gmath::aabb_vs_aabb(a, b);

    // TODO: do they collide?
    REQUIRE(hit.collided == false);  // <-- fix

    // TODO: which axis wins, and what is the penetration vector?
    // (Hint: a's centre is up-left of b's, so a is pushed the negative way on
    //  the smaller-overlap axis.)
    REQUIRE(hit.penetration == Vec2f{0.0f, 0.0f});  // <-- fix
}
