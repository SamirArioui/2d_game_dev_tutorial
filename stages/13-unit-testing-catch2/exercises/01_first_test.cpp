// Exercise 01 — your first Catch2 test (REQUIRE and CHECK)
//
// Catch2 is C++'s pytest: you write small TEST_CASEs full of assertions and a
// runner reports pass/fail. There is no main() here — Catch2::Catch2WithMain
// supplies it (wired up in exercises/CMakeLists.txt).
//
// This file is self-contained: the function under test (clamp) is defined right
// here so you can focus purely on the TESTING syntax.
//
// Build & run just this exercise (from exercises/):
//   cmake -S . -B build && cmake --build build --target 01_first_test
//   ./build/01_first_test
//
// TODO:
//   1. In the TEST_CASE below add REQUIRE checks that clamp:
//        - snaps -5 up to 0        REQUIRE(clamp(-5, 0, 10) == 0)
//        - snaps 42 down to 10      REQUIRE(clamp(42, 0, 10) == 10)
//        - leaves 3 unchanged       REQUIRE(clamp(3, 0, 10) == 3)
//   2. Add ONE CHECK (not REQUIRE) for an endpoint and note the difference:
//        REQUIRE stops the test on failure; CHECK records it and keeps going.

#include <catch2/catch_test_macros.hpp>

// --- function under test (normally this would live in a library) ------------
int clamp(int value, int lo, int hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

TEST_CASE("clamp constrains a value to a range", "[ex01]") {
    // TODO: write your REQUIRE / CHECK assertions here.
}
