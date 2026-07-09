// Exercise 05 — aim a projectile (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>

struct Vec2 {
    float x;
    float y;
};

float length(const Vec2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

Vec2 normalize(const Vec2& v) {
    float len = length(v);
    if (len <= 1e-8f) {
        return Vec2{0.0f, 0.0f};
    }
    return Vec2{v.x / len, v.y / len};
}

Vec2 aim_velocity(const Vec2& origin, const Vec2& target, float speed) {
    Vec2 to_target{target.x - origin.x, target.y - origin.y};
    Vec2 dir = normalize(to_target);
    return Vec2{dir.x * speed, dir.y * speed};
}

TEST_CASE("aim straight down") {
    Vec2 v = aim_velocity(Vec2{100.0f, 100.0f}, Vec2{100.0f, 400.0f}, 250.0f);
    REQUIRE(length(v) == Catch::Approx(250.0f));
    REQUIRE(v.x == Catch::Approx(0.0f));
    REQUIRE(v.y == Catch::Approx(250.0f));
}

TEST_CASE("aim along a 3-4-5 diagonal") {
    Vec2 v = aim_velocity(Vec2{0.0f, 0.0f}, Vec2{3.0f, 4.0f}, 10.0f);
    REQUIRE(length(v) == Catch::Approx(10.0f));
    REQUIRE(v.x == Catch::Approx(6.0f));
    REQUIRE(v.y == Catch::Approx(8.0f));
}
