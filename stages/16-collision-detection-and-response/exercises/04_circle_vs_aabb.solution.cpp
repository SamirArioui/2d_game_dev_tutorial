// Exercise 04 — circle vs AABB (solution, centre-outside case).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
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
    float x;
    float y;
    float w;
    float h;
};

std::optional<Vec2> circle_vs_aabb(const Circle& c, const AABB& box) {
    const float closest_x = std::clamp(c.c.x, box.x, box.x + box.w);
    const float closest_y = std::clamp(c.c.y, box.y, box.y + box.h);

    const float dx = c.c.x - closest_x;  // closest point -> centre
    const float dy = c.c.y - closest_y;
    const float dist_sq = dx * dx + dy * dy;

    if (dist_sq > c.r * c.r) {
        return std::nullopt;
    }

    const float dist = std::sqrt(dist_sq);
    // (centre-inside would give dist == 0; the project's full version ejects
    //  through the nearest face there.)
    return Vec2{(dx / dist) * (c.r - dist), (dy / dist) * (c.r - dist)};
}

TEST_CASE("circle just past the right face overlaps") {
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
