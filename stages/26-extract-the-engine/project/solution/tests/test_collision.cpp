// ============================================================================
// Tests for the engine's pure math + collision (gmath). No SFML, no window —
// this is exactly why we kept these SFML-free: they test in milliseconds.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include "eng/collision.hpp"
#include "eng/math.hpp"

using gmath::AABB;
using gmath::Hit;
using gmath::Vec2f;

TEST_CASE("Vec2 arithmetic behaves like a 2D vector", "[math]") {
    const Vec2f a{3.0f, 4.0f};
    const Vec2f b{1.0f, 2.0f};

    REQUIRE((a + b) == Vec2f{4.0f, 6.0f});
    REQUIRE((a - b) == Vec2f{2.0f, 2.0f});
    REQUIRE((a * 2.0f) == Vec2f{6.0f, 8.0f});
    REQUIRE(gmath::length(a) == 5.0f);          // 3-4-5 triangle
    REQUIRE(gmath::dot(a, b) == 11.0f);         // 3*1 + 4*2
}

TEST_CASE("normalize of the zero vector is zero (no divide-by-zero)", "[math]") {
    REQUIRE(gmath::normalized(Vec2f{0.0f, 0.0f}) == Vec2f{0.0f, 0.0f});

    const Vec2f n = gmath::normalized(Vec2f{0.0f, 5.0f});
    REQUIRE(n == Vec2f{0.0f, 1.0f});
}

TEST_CASE("clamp constrains to the range", "[math]") {
    REQUIRE(gmath::clamp(5.0f, 0.0f, 10.0f) == 5.0f);
    REQUIRE(gmath::clamp(-3.0f, 0.0f, 10.0f) == 0.0f);
    REQUIRE(gmath::clamp(99.0f, 0.0f, 10.0f) == 10.0f);
}

TEST_CASE("separated boxes do not collide", "[collision]") {
    const AABB a{{0.0f, 0.0f}, {10.0f, 10.0f}};
    const AABB b{{20.0f, 0.0f}, {10.0f, 10.0f}};  // 10px gap on X
    REQUIRE_FALSE(gmath::aabb_vs_aabb(a, b).collided);
}

TEST_CASE("overlapping boxes report the minimum-translation vector", "[collision]") {
    // `a` overlaps `b` by 2 on X and 8 on Y -> resolve on the SMALLER (X) axis.
    const AABB a{{0.0f, 0.0f}, {10.0f, 10.0f}};
    const AABB b{{8.0f, 2.0f}, {10.0f, 10.0f}};

    const Hit hit = gmath::aabb_vs_aabb(a, b);
    REQUIRE(hit.collided);
    // a's centre is left of b's, so a is pushed LEFT (negative x) by the overlap.
    REQUIRE(hit.penetration == Vec2f{-2.0f, 0.0f});

    // Applying the penetration separates them.
    const AABB moved{a.pos + hit.penetration, a.size};
    REQUIRE_FALSE(gmath::aabb_vs_aabb(moved, b).collided);
}

TEST_CASE("resolution picks the vertical axis when Y overlap is smaller", "[collision]") {
    const AABB a{{0.0f, 0.0f}, {10.0f, 10.0f}};
    const AABB b{{2.0f, 8.0f}, {10.0f, 10.0f}};  // overlap X=8, Y=2 -> resolve on Y

    const Hit hit = gmath::aabb_vs_aabb(a, b);
    REQUIRE(hit.collided);
    REQUIRE(hit.penetration == Vec2f{0.0f, -2.0f});
}
