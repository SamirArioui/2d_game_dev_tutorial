// Exercise 06 — solution
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

float lerp(float a, float b, float t) {
    // FIXED: the slope from a to b is (b - a), not (b + a).
    return a + (b - a) * t;
}

TEST_CASE("lerp blends from a to b", "[ex06]") {
    REQUIRE(lerp(0.0f, 10.0f, 0.0f) == Catch::Approx(0.0f));
    REQUIRE(lerp(0.0f, 10.0f, 1.0f) == Catch::Approx(10.0f));
    REQUIRE(lerp(10.0f, 20.0f, 0.0f) == Catch::Approx(10.0f));
    REQUIRE(lerp(10.0f, 20.0f, 0.5f) == Catch::Approx(15.0f));
    REQUIRE(lerp(10.0f, 20.0f, 1.0f) == Catch::Approx(20.0f));
}
