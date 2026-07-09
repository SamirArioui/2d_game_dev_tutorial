// Exercise 01 — Vec2 math (solution)
#include <catch2/catch_test_macros.hpp>

#include "eng/math.hpp"

using gmath::Vec2f;

TEST_CASE("vector arithmetic", "[ex01]") {
    const Vec2f a{3.0f, 4.0f};
    const Vec2f b{1.0f, 1.0f};

    REQUIRE((a + b) == Vec2f{4.0f, 5.0f});
    REQUIRE(gmath::length(a) == 5.0f);  // 3-4-5 right triangle

    const Vec2f n = gmath::normalized(a);
    REQUIRE(gmath::length(n) == 1.0f);  // normalized vectors are unit length
}
