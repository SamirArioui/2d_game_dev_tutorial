// Exercise 03 — circle vs circle (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

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

std::optional<Vec2> circle_vs_circle(const Circle& a, const Circle& b) {
    const float dx = a.c.x - b.c.x;  // from B toward A
    const float dy = a.c.y - b.c.y;
    const float r = a.r + b.r;
    const float dist_sq = dx * dx + dy * dy;

    if (dist_sq >= r * r) {
        return std::nullopt;  // no sqrt needed to reject
    }

    const float dist = std::sqrt(dist_sq);
    const float depth = r - dist;
    if (dist > 1e-8f) {
        return Vec2{(dx / dist) * depth, (dy / dist) * depth};
    }
    return Vec2{depth, 0.0f};  // concentric: arbitrary +x direction
}

TEST_CASE("far apart circles miss") {
    REQUIRE_FALSE(circle_vs_circle(Circle{{0, 0}, 5}, Circle{{100, 0}, 5}).has_value());
}

TEST_CASE("overlapping circles report penetration toward A") {
    auto p = circle_vs_circle(Circle{{0, 0}, 5}, Circle{{8, 0}, 5});
    REQUIRE(p.has_value());
    REQUIRE(p->x == Catch::Approx(-2.0f));
    REQUIRE(p->y == Catch::Approx(0.0f));
}

TEST_CASE("just-touching circles are not overlapping") {
    REQUIRE_FALSE(circle_vs_circle(Circle{{0, 0}, 5}, Circle{{10, 0}, 5}).has_value());
}
