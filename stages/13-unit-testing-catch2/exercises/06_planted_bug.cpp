// Exercise 06 — the planted-bug hunt
//
// There is a BUG in lerp() below. The test is correct and currently FAILS —
// which is a test doing its job: catching a regression before it ships.
//
//   cmake --build build --target 06_planted_bug && ./build/06_planted_bug   # RED
//
// TODO:
//   1. Run it and read the failure. Catch2 prints the expression AND the actual
//      value, e.g.  `lerp(10.0f, 20.0f, 0.5f) == Approx(15.0)` with the real
//      result (25.0) shown beside it — that gap is your clue.
//   2. Fix the one wrong operator in lerp so the whole test goes green.
//      (Hint: linear interpolation walks the SLOPE from a to b. What is the
//       slope of a straight line from a to b?)

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

float lerp(float a, float b, float t) {
    // BUG: this should interpolate from a to b, but one operator is wrong.
    return a + (b + a) * t;
}

TEST_CASE("lerp blends from a to b", "[ex06]") {
    REQUIRE(lerp(0.0f, 10.0f, 0.0f) == Catch::Approx(0.0f));
    REQUIRE(lerp(0.0f, 10.0f, 1.0f) == Catch::Approx(10.0f));
    REQUIRE(lerp(10.0f, 20.0f, 0.0f) == Catch::Approx(10.0f));
    REQUIRE(lerp(10.0f, 20.0f, 0.5f) == Catch::Approx(15.0f));  // buggy code -> 25
    REQUIRE(lerp(10.0f, 20.0f, 1.0f) == Catch::Approx(20.0f));  // buggy code -> 40
}
