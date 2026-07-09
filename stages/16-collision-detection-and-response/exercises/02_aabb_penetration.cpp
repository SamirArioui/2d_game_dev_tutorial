// Exercise 02 — AABB penetration vector / MTV (medium) — a Catch2 drill.
//
// Knowing two boxes overlap isn't enough to respond; you need to know HOW to pull
// them apart. The penetration vector (a.k.a. Minimum Translation Vector, MTV) is
// the shortest shift that separates them. For AABBs it lies along the axis of
// SMALLER overlap (you back out the short way), signed so it moves box A off B.
//
// Compute the overlap on each axis, pick the smaller, and sign it by comparing
// the two boxes' centres: if A's centre is left of B's, push A left (-x), etc.
//
// TODO: implement penetration(). Assume the boxes already overlap.
//
// Build & test:
//   cmake --build build --target 02_aabb_penetration && ./build/02_aabb_penetration

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>  // std::min, std::max

struct Vec2 {
    float x;
    float y;
};

struct AABB {
    float x;  // top-left
    float y;
    float w;
    float h;
};

Vec2 penetration(const AABB& a, const AABB& b) {
    // TODO:
    //   overlap_x = min(a.x+a.w, b.x+b.w) - max(a.x, b.x)
    //   overlap_y = min(a.y+a.h, b.y+b.h) - max(a.y, b.y)
    //   pick the smaller-overlap axis; sign it by centre comparison.
    (void)a;
    (void)b;
    return Vec2{0.0f, 0.0f};
}

TEST_CASE("separate along the shallow axis (x here)") {
    // a and b overlap 5 on x, 10 on y -> back out along x.
    Vec2 p = penetration(AABB{0, 0, 10, 10}, AABB{5, 0, 10, 10});
    REQUIRE(p.x == Catch::Approx(-5.0f));  // A is left of B, so push A left
    REQUIRE(p.y == Catch::Approx(0.0f));
}

TEST_CASE("separate along the shallow axis (y here)") {
    // Overlap 10 on x, 4 on y -> back out along y; A is above B so push A up (-y).
    Vec2 p = penetration(AABB{0, 0, 10, 10}, AABB{0, 6, 10, 10});
    REQUIRE(p.x == Catch::Approx(0.0f));
    REQUIRE(p.y == Catch::Approx(-4.0f));
}

TEST_CASE("sign flips when A is on the other side") {
    Vec2 p = penetration(AABB{5, 0, 10, 10}, AABB{0, 0, 10, 10});
    REQUIRE(p.x == Catch::Approx(5.0f));  // A is right of B, so push A right
}
