// Exercise 01 — solution
#include <catch2/catch_test_macros.hpp>

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
    // REQUIRE: a hard assertion. If it fails, the test stops right here.
    REQUIRE(clamp(-5, 0, 10) == 0);
    REQUIRE(clamp(42, 0, 10) == 10);
    REQUIRE(clamp(3, 0, 10) == 3);

    // CHECK: a soft assertion. On failure it records the problem but keeps
    // running, so you see BOTH endpoint results from a single run.
    CHECK(clamp(0, 0, 10) == 0);
    CHECK(clamp(10, 0, 10) == 10);
}
