// Exercise 04 — The fixed-timestep accumulator (medium-hard) — a Catch2 drill.
//
// This one has NO window: the accumulator is pure arithmetic, so we test it
// directly. Real frames arrive at irregular intervals; a fixed timestep needs
// to advance the simulation in constant slices. The accumulator banks real
// time until whole slices have built up.
//
// TODO: implement steps_to_run so it:
//   1. adds `frame_dt` to `accumulator`,
//   2. subtracts `fixed_dt` and counts a step while `accumulator >= fixed_dt`,
//   3. stops early once it has counted `max_steps` (spiral-of-death cap),
//   4. leaves the leftover in `accumulator` for next frame,
//   5. returns the number of steps counted.
//
// Build & test (from exercises/):
//   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
//   cmake --build build --target 04_fixed_accumulator
//   ./build/04_fixed_accumulator          # runs the tests (starts RED)

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

int steps_to_run(float& accumulator, float frame_dt, float fixed_dt, int max_steps) {
    // TODO: replace this stub with the real accumulator loop.
    (void)accumulator;
    (void)frame_dt;
    (void)fixed_dt;
    (void)max_steps;
    return 0;
}

TEST_CASE("a 1/60 s frame runs two 1/120 s steps") {
    const float fixed_dt = 1.0f / 120.0f;
    float acc = 0.0f;
    REQUIRE(steps_to_run(acc, 1.0f / 60.0f, fixed_dt, 8) == 2);
    REQUIRE(acc == Catch::Approx(0.0f).margin(1e-6));
}

TEST_CASE("a short frame banks time and runs no steps") {
    const float fixed_dt = 1.0f / 120.0f;
    float acc = 0.0f;
    REQUIRE(steps_to_run(acc, 0.004f, fixed_dt, 8) == 0);
    REQUIRE(acc == Catch::Approx(0.004f));
}

TEST_CASE("leftover time carries over to the next call") {
    const float fixed_dt = 1.0f / 120.0f;
    float acc = 0.0f;
    REQUIRE(steps_to_run(acc, 0.01f, fixed_dt, 8) == 1);
    REQUIRE(acc == Catch::Approx(0.01f - fixed_dt));
}

TEST_CASE("max_steps caps the work per call (no spiral of death)") {
    const float fixed_dt = 1.0f / 120.0f;
    float acc = 0.0f;
    REQUIRE(steps_to_run(acc, 1.0f, fixed_dt, 8) == 8);
}
