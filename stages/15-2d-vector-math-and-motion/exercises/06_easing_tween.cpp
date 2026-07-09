// Exercise 06 — easing & a ping-pong tween (medium-hard) — a Catch2 drill.
//
// A linear lerp moves at constant speed and stops abruptly. EASING reshapes the
// time fraction t (in [0,1]) along a curve so motion starts and stops gently.
// smoothstep is the classic S-curve. To make a platform go back and forth
// forever, feed an ever-growing time into ping_pong, which folds it into a
// 0->1->0 triangle wave of period 2.
//
// TODO: implement smoothstep() and ping_pong().
//   smoothstep(t) = 3t² - 2t³            (f(0)=0, f(0.5)=0.5, f(1)=1)
//   ping_pong(t): wrap t into [0,2); if the wrapped value w <= 1 return w,
//                 else return 2 - w.
//
// Build & test:
//   cmake --build build --target 06_easing_tween && ./build/06_easing_tween

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cmath>

float smoothstep(float t) {
    // TODO: return the S-curve value 3t² - 2t³.
    (void)t;
    return 0.0f;
}

float ping_pong(float t) {
    // TODO: fold t into a 0->1->0 triangle wave of period 2.
    (void)t;
    return 0.0f;
}

TEST_CASE("smoothstep passes through 0, 0.5, 1 and is an S-curve") {
    REQUIRE(smoothstep(0.0f) == Catch::Approx(0.0f));
    REQUIRE(smoothstep(0.5f) == Catch::Approx(0.5f));
    REQUIRE(smoothstep(1.0f) == Catch::Approx(1.0f));
    // Below the diagonal early (slow start), above it late (slow stop).
    REQUIRE(smoothstep(0.25f) < 0.25f);
    REQUIRE(smoothstep(0.75f) > 0.75f);
}

TEST_CASE("ping_pong sweeps up and back and repeats") {
    REQUIRE(ping_pong(0.0f) == Catch::Approx(0.0f));
    REQUIRE(ping_pong(0.5f) == Catch::Approx(0.5f));
    REQUIRE(ping_pong(1.0f) == Catch::Approx(1.0f));  // peak
    REQUIRE(ping_pong(1.5f) == Catch::Approx(0.5f));  // descending
    REQUIRE(ping_pong(2.0f) == Catch::Approx(0.0f));  // period complete
    REQUIRE(ping_pong(2.5f) == Catch::Approx(0.5f));  // repeats
}
