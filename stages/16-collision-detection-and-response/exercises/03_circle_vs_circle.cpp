// Exercise 03 — circle vs circle (medium) — a Catch2 drill.
//
// Circles are the cheapest collider of all: two circles overlap iff the distance
// between their centres is less than the sum of their radii. Compare SQUARED
// distances to skip a sqrt when they miss. When they do overlap, the penetration
// points from B's centre toward A's (so it pushes A away) with length
// (rA + rB) - distance.
//
// Return std::optional<Vec2>: nullopt = miss (like Python's `return None`), a
// value = the penetration vector.
//
// TODO: implement circle_vs_circle().
//
// Build & test:
//   cmake --build build --target 03_circle_vs_circle && ./build/03_circle_vs_circle

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <optional>

struct Vec2 {
    float x;
    float y;
};

struct Circle {
    Vec2 c;  // centre
    float r;
};

std::optional<Vec2> circle_vs_circle(const Circle& a, const Circle& b) {
    // TODO:
    //   dx = a.c.x - b.c.x; dy = a.c.y - b.c.y;  (from B toward A)
    //   r = a.r + b.r;  if dx*dx+dy*dy >= r*r return std::nullopt;
    //   dist = sqrt(...); depth = r - dist;
    //   return the unit (dx,dy)/dist scaled by depth.
    (void)a;
    (void)b;
    return std::nullopt;
}

TEST_CASE("far apart circles miss") {
    REQUIRE_FALSE(circle_vs_circle(Circle{{0, 0}, 5}, Circle{{100, 0}, 5}).has_value());
}

TEST_CASE("overlapping circles report penetration toward A") {
    auto p = circle_vs_circle(Circle{{0, 0}, 5}, Circle{{8, 0}, 5});  // depth 2
    REQUIRE(p.has_value());
    REQUIRE(p->x == Catch::Approx(-2.0f));  // push A away from B (-x)
    REQUIRE(p->y == Catch::Approx(0.0f));
}

TEST_CASE("just-touching circles are not overlapping") {
    REQUIRE_FALSE(circle_vs_circle(Circle{{0, 0}, 5}, Circle{{10, 0}, 5}).has_value());
}
