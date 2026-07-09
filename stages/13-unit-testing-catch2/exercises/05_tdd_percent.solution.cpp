// Exercise 05 — solution
#include <catch2/catch_test_macros.hpp>

#include <cmath>  // std::lround

// Pure: depends only on its arguments, returns a value, touches nothing else.
int percent(int current, int max) {
    if (max <= 0) {
        return 0;  // guard: no division, and "nothing out of nothing" is 0%
    }
    const int p = static_cast<int>(std::lround(100.0 * current / max));
    if (p < 0) {
        return 0;
    }
    if (p > 100) {
        return 100;  // clamp the top end
    }
    return p;
}

TEST_CASE("percent reports current/max as an integer 0..100", "[ex05]") {
    REQUIRE(percent(0, 100) == 0);
    REQUIRE(percent(50, 100) == 50);
    REQUIRE(percent(100, 100) == 100);
    REQUIRE(percent(3, 4) == 75);
    REQUIRE(percent(1, 3) == 33);
    REQUIRE(percent(2, 3) == 67);
    REQUIRE(percent(200, 100) == 100);
    REQUIRE(percent(5, 0) == 0);
}
