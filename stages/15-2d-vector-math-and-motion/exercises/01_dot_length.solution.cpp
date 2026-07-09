// Exercise 01 — dot product & length (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>

struct Vec2 {
    float x;
    float y;
};

float dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

float length(const Vec2& v) {
    return std::sqrt(dot(v, v));  // sqrt(x² + y²)
}

TEST_CASE("dot measures alignment") {
    REQUIRE(dot(Vec2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}) == Catch::Approx(10.0f));
    REQUIRE(dot(Vec2{1.0f, 0.0f}, Vec2{0.0f, 1.0f}) == Catch::Approx(0.0f));
    REQUIRE(dot(Vec2{1.0f, 0.0f}, Vec2{-1.0f, 0.0f}) == Catch::Approx(-1.0f));
}

TEST_CASE("length is Pythagoras") {
    REQUIRE(length(Vec2{3.0f, 4.0f}) == Catch::Approx(5.0f));
    REQUIRE(length(Vec2{0.0f, 0.0f}) == Catch::Approx(0.0f));
}
