// Exercise 02 — normalize a vector (medium) — a Catch2 drill.
//
// normalize(v) returns a vector of length 1 pointing the same way as v. It
// separates DIRECTION from MAGNITUDE, so you can then scale by any speed. The
// catch is the ZERO vector: it has no direction, and dividing by its length
// (zero) gives NaN. You must guard it.
//
// TODO: implement normalize().
//   - if length(v) is ~0, return {0, 0}
//   - otherwise return {v.x/len, v.y/len}
//
// Build & test:
//   cmake --build build --target 02_normalize && ./build/02_normalize

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
    // TODO: guard the zero vector, then divide by the length.
    (void)v;
    return Vec2{0.0f, 0.0f};
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
    REQUIRE_FALSE(std::isnan(z.x));  // the whole point of the guard
}
