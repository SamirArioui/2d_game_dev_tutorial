// Exercise 02 — AABB penetration vector / MTV (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>

struct Vec2 {
    float x;
    float y;
};

struct AABB {
    float x;
    float y;
    float w;
    float h;
};

Vec2 penetration(const AABB& a, const AABB& b) {
    const float overlap_x = std::min(a.x + a.w, b.x + b.w) - std::max(a.x, b.x);
    const float overlap_y = std::min(a.y + a.h, b.y + b.h) - std::max(a.y, b.y);

    const float a_cx = a.x + a.w * 0.5f;
    const float a_cy = a.y + a.h * 0.5f;
    const float b_cx = b.x + b.w * 0.5f;
    const float b_cy = b.y + b.h * 0.5f;

    if (overlap_x < overlap_y) {
        const float sign = (a_cx < b_cx) ? -1.0f : 1.0f;
        return Vec2{sign * overlap_x, 0.0f};
    }
    const float sign = (a_cy < b_cy) ? -1.0f : 1.0f;
    return Vec2{0.0f, sign * overlap_y};
}

TEST_CASE("separate along the shallow axis (x here)") {
    Vec2 p = penetration(AABB{0, 0, 10, 10}, AABB{5, 0, 10, 10});
    REQUIRE(p.x == Catch::Approx(-5.0f));
    REQUIRE(p.y == Catch::Approx(0.0f));
}

TEST_CASE("separate along the shallow axis (y here)") {
    Vec2 p = penetration(AABB{0, 0, 10, 10}, AABB{0, 6, 10, 10});
    REQUIRE(p.x == Catch::Approx(0.0f));
    REQUIRE(p.y == Catch::Approx(-4.0f));
}

TEST_CASE("sign flips when A is on the other side") {
    Vec2 p = penetration(AABB{5, 0, 10, 10}, AABB{0, 0, 10, 10});
    REQUIRE(p.x == Catch::Approx(5.0f));
}
