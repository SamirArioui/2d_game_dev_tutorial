// Exercise 01 — dot product & length (easy) — a Catch2 drill.
//
// The two most fundamental vector operations. `dot(a,b)` combines two vectors
// into a scalar that measures alignment; `length(v)` is the magnitude via
// Pythagoras. Everything later (normalize, distance, reflection) is built on
// these two.
//
// TODO: implement dot() and length() for the tiny Vec2 below.
//   dot(a,b)  = a.x*b.x + a.y*b.y
//   length(v) = sqrt(dot(v,v))
//
// Build & test (from exercises/):
//   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
//   cmake --build build --target 01_dot_length
//   ./build/01_dot_length          # starts RED

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>

struct Vec2 {
    float x;
    float y;
};

float dot(const Vec2& a, const Vec2& b) {
    // TODO: return the dot product.
    (void)a;
    (void)b;
    return 0.0f;
}

float length(const Vec2& v) {
    // TODO: return the magnitude (hint: reuse dot).
    (void)v;
    return 0.0f;
}

TEST_CASE("dot measures alignment") {
    REQUIRE(dot(Vec2{3.0f, 4.0f}, Vec2{2.0f, 1.0f}) == Catch::Approx(10.0f));
    REQUIRE(dot(Vec2{1.0f, 0.0f}, Vec2{0.0f, 1.0f}) == Catch::Approx(0.0f));   // perpendicular
    REQUIRE(dot(Vec2{1.0f, 0.0f}, Vec2{-1.0f, 0.0f}) == Catch::Approx(-1.0f)); // opposed
}

TEST_CASE("length is Pythagoras") {
    REQUIRE(length(Vec2{3.0f, 4.0f}) == Catch::Approx(5.0f));
    REQUIRE(length(Vec2{0.0f, 0.0f}) == Catch::Approx(0.0f));
}
