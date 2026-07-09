// Exercise 04 — circle vs AABB (medium-hard) — a Catch2 drill.
//
// The trick that makes this easy: the closest point on a box to any point is
// just that point with its x CLAMPED to [left, right] and its y clamped to
// [top, bottom]. If the circle's centre is within one radius of that closest
// point, they collide. The penetration pushes the circle out along the line from
// the closest point to the centre, by (radius - distance).
//
// (This drill covers the common case where the circle's centre is OUTSIDE the
// box. The centre-inside case needs an extra "eject through nearest face" step —
// see collision.hpp in the project for the full version.)
//
// TODO: implement circle_vs_aabb() for the centre-outside case.
//
// Build & test:
//   cmake --build build --target 04_circle_vs_aabb && ./build/04_circle_vs_aabb

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>  // std::clamp
#include <cmath>
#include <optional>

struct Vec2 {
    float x;
    float y;
};

struct Circle {
    Vec2 c;
    float r;
};

struct AABB {
    float x;  // top-left
    float y;
    float w;
    float h;
};

std::optional<Vec2> circle_vs_aabb(const Circle& c, const AABB& box) {
    // TODO:
    //   closest_x = clamp(c.c.x, box.x, box.x+box.w)
    //   closest_y = clamp(c.c.y, box.y, box.y+box.h)
    //   dx = c.c.x - closest_x; dy = c.c.y - closest_y;
    //   if dx*dx+dy*dy > c.r*c.r return std::nullopt;
    //   dist = sqrt(...); return (dx,dy)/dist * (c.r - dist);
    (void)c;
    (void)box;
    return std::nullopt;
}

TEST_CASE("circle just past the right face overlaps") {
    // Centre 4 px past the right face (x=100), radius 10 -> penetration 6 on +x.
    auto p = circle_vs_aabb(Circle{{104, 50}, 10}, AABB{0, 0, 100, 100});
    REQUIRE(p.has_value());
    REQUIRE(p->x == Catch::Approx(6.0f));
    REQUIRE(p->y == Catch::Approx(0.0f));
}

TEST_CASE("circle far away misses") {
    REQUIRE_FALSE(circle_vs_aabb(Circle{{200, 50}, 10}, AABB{0, 0, 100, 100}).has_value());
}

TEST_CASE("corner overlap uses the diagonal normal") {
    auto p = circle_vs_aabb(Circle{{103, 104}, 10}, AABB{0, 0, 100, 100});
    REQUIRE(p.has_value());
    REQUIRE(p->x > 0.0f);
    REQUIRE(p->y > 0.0f);
}
