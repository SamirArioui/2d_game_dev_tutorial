// Exercise 01 — Vec2 math (easy)
//
// Drill the engine's gmath::Vec2f API. Fill in the REQUIRE(...) checks marked
// TODO with the correct expected values, then build and run this file. It links
// only Catch2 — no SFML — because gmath is pure.
//
//   cmake -S . -B build && cmake --build build --target 01_vec2_math
//   ./build/01_vec2_math

#include <catch2/catch_test_macros.hpp>

#include "eng/math.hpp"

using gmath::Vec2f;

TEST_CASE("vector arithmetic", "[ex01]") {
    const Vec2f a{3.0f, 4.0f};
    const Vec2f b{1.0f, 1.0f};

    // TODO: what is a + b ?
    REQUIRE((a + b) == Vec2f{0.0f, 0.0f});  // <-- fix the expected value

    // TODO: the length of a 3-4 vector is a famous whole number. Which?
    REQUIRE(gmath::length(a) == 0.0f);  // <-- fix

    // TODO: normalized() should return a unit-length vector. Fill in the length.
    const Vec2f n = gmath::normalized(a);
    REQUIRE(gmath::length(n) == 0.0f);  // <-- fix
}
