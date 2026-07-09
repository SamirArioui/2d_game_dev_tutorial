// Exercise 05 — Test-Driven Development + pure vs side-effect
//
// TDD flips the usual order: write the test FIRST (describing what you want),
// watch it FAIL, then write just enough code to make it pass ("red -> green").
//
// The test below is already written. `percent` is only a STUB (returns 0), so
// this exercise starts RED — build and run it and you'll see failures:
//   cmake --build build --target 05_tdd_percent && ./build/05_tdd_percent
//
// TODO: implement percent() so every REQUIRE passes ("green"):
//   percent(current, max) = round(100 * current / max), clamped to [0, 100],
//   and returns 0 when max <= 0 (guard against divide-by-zero).
//
// WHY this function is easy to test: it is PURE — it just RETURNS a number, so a
// test states the expected number. A version that instead PRINTED "HP: 75%" to
// the screen would be a SIDE-EFFECT: to test it you'd have to capture stdout.
// The stage's rule: keep logic pure and value-returning; leave the printing /
// rendering to a thin layer on top that you don't need to unit-test.

#include <catch2/catch_test_macros.hpp>

int percent(int current, int max) {
    // TODO: replace this stub with the real calculation.
    return 0;
}

TEST_CASE("percent reports current/max as an integer 0..100", "[ex05]") {
    REQUIRE(percent(0, 100) == 0);
    REQUIRE(percent(50, 100) == 50);
    REQUIRE(percent(100, 100) == 100);
    REQUIRE(percent(3, 4) == 75);
    REQUIRE(percent(1, 3) == 33);       // 33.33 -> rounds to 33
    REQUIRE(percent(2, 3) == 67);       // 66.66 -> rounds to 67
    REQUIRE(percent(200, 100) == 100);  // clamped; never exceeds 100
    REQUIRE(percent(5, 0) == 0);        // divide-by-zero guard
}
