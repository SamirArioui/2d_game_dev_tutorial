// Exercise 04 — The fixed-timestep accumulator (solution).

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

int steps_to_run(float& accumulator, float frame_dt, float fixed_dt, int max_steps) {
    accumulator += frame_dt;  // bank this frame's real time

    int steps = 0;
    // Pull out whole fixed slices, but never more than max_steps: a long stall
    // (breakpoint, window drag) would otherwise demand a huge catch-up burst.
    while (accumulator >= fixed_dt && steps < max_steps) {
        accumulator -= fixed_dt;
        ++steps;
    }
    // Whatever is left is under fixed_dt (or capped-out); it waits for next call.
    return steps;
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
