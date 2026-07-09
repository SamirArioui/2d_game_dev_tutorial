// Exercise 02 — normalize a vector (solution).

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
        return Vec2{0.0f, 0.0f};  // no direction -> no NaN
    }
    return Vec2{v.x / len, v.y / len};
}

TEST_CASE("normalize returns a unit vector") {
    Vec2 n = normalize(Vec2{0.0f, 5.0f});
    REQUIRE(length(n) == Catch::Approx(1.0f));
    REQUIRE(n.y == Catch::Approx(1.0f));
}

TEST_CASE("normalize keeps the direction of a diagonal") {
    Vec2 d = normalize(Vec2{10.0f, 10.0f});
    REQUIRE(length(d) == Catch::Approx(1.0f));
    REQUIRE(d.x == Catch::Approx(d.y));
}

TEST_CASE("normalize(zero) is zero, not NaN") {
    Vec2 z = normalize(Vec2{0.0f, 0.0f});
    REQUIRE(z.x == Catch::Approx(0.0f));
    REQUIRE(z.y == Catch::Approx(0.0f));
    REQUIRE_FALSE(std::isnan(z.x));
}
