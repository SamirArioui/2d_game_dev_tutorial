// Exercise 06 — easing & a ping-pong tween (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>

float smoothstep(float t) {
    return t * t * (3.0f - 2.0f * t);  // 3t² - 2t³
}

float ping_pong(float t) {
    float w = t - 2.0f * std::floor(t / 2.0f);  // wrap into [0, 2)
    return w <= 1.0f ? w : 2.0f - w;            // fold the top half back down
}

TEST_CASE("smoothstep passes through 0, 0.5, 1 and is an S-curve") {
    REQUIRE(smoothstep(0.0f) == Catch::Approx(0.0f));
    REQUIRE(smoothstep(0.5f) == Catch::Approx(0.5f));
    REQUIRE(smoothstep(1.0f) == Catch::Approx(1.0f));
    REQUIRE(smoothstep(0.25f) < 0.25f);
    REQUIRE(smoothstep(0.75f) > 0.75f);
}

TEST_CASE("ping_pong sweeps up and back and repeats") {
    REQUIRE(ping_pong(0.0f) == Catch::Approx(0.0f));
    REQUIRE(ping_pong(0.5f) == Catch::Approx(0.5f));
    REQUIRE(ping_pong(1.0f) == Catch::Approx(1.0f));
    REQUIRE(ping_pong(1.5f) == Catch::Approx(0.5f));
    REQUIRE(ping_pong(2.0f) == Catch::Approx(0.0f));
    REQUIRE(ping_pong(2.5f) == Catch::Approx(0.5f));
}
