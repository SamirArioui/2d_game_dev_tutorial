// ============================================================================
// Tests for the window-free simulation logic (sim.hpp).
//
// This file is the proof that keeping logic OUT of the render loop pays off: a
// bouncing ball is a "visual" feature, yet every rule that governs it — how far
// it moves, when it flips direction, how the accumulator slices time — is a pure
// function we can pin down without ever opening a window.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "sim.hpp"

using gmath::Vec2f;

TEST_CASE("integrate moves by velocity * dt", "[sim][integrate]") {
    sim::Ball ball{Vec2f{0.0f, 0.0f}, Vec2f{100.0f, -50.0f}, 10.0f};

    sim::integrate(ball, 0.5f);  // half a second

    REQUIRE(ball.pos.x == Catch::Approx(50.0f));
    REQUIRE(ball.pos.y == Catch::Approx(-25.0f));
    // Constant velocity: motion is unchanged.
    REQUIRE(ball.vel.x == Catch::Approx(100.0f));
    REQUIRE(ball.vel.y == Catch::Approx(-50.0f));
}

TEST_CASE("motion is frame-rate independent", "[sim][integrate]") {
    // One 0.2 s step must land in the same place as two 0.1 s steps: distance
    // per real second does not depend on how the second is sliced.
    sim::Ball coarse{Vec2f{0.0f, 0.0f}, Vec2f{300.0f, 0.0f}, 10.0f};
    sim::Ball fine{Vec2f{0.0f, 0.0f}, Vec2f{300.0f, 0.0f}, 10.0f};

    sim::integrate(coarse, 0.2f);
    sim::integrate(fine, 0.1f);
    sim::integrate(fine, 0.1f);

    REQUIRE(coarse.pos.x == Catch::Approx(fine.pos.x));
}

TEST_CASE("reflect_in_bounds bounces off walls and flips velocity", "[sim][reflect]") {
    const float width = 800.0f;
    const float height = 600.0f;

    SECTION("crossing the left wall (x=0) reverses vx and snaps inside") {
        sim::Ball ball{Vec2f{5.0f, 300.0f}, Vec2f{-100.0f, 0.0f}, 10.0f};
        bool bounced = sim::reflect_in_bounds(ball, width, height);
        REQUIRE(bounced);
        REQUIRE(ball.pos.x == Catch::Approx(10.0f));  // snapped to radius
        REQUIRE(ball.vel.x == Catch::Approx(100.0f));  // reversed
    }

    SECTION("crossing the bottom wall (y=height) reverses vy — +y is DOWN") {
        sim::Ball ball{Vec2f{400.0f, 595.0f}, Vec2f{0.0f, 100.0f}, 10.0f};
        bool bounced = sim::reflect_in_bounds(ball, width, height);
        REQUIRE(bounced);
        REQUIRE(ball.pos.y == Catch::Approx(590.0f));  // height - radius
        REQUIRE(ball.vel.y == Catch::Approx(-100.0f));
    }

    SECTION("a ball safely inside does not bounce") {
        sim::Ball ball{Vec2f{400.0f, 300.0f}, Vec2f{100.0f, 100.0f}, 10.0f};
        REQUIRE_FALSE(sim::reflect_in_bounds(ball, width, height));
        REQUIRE(ball.vel == Vec2f{100.0f, 100.0f});
    }
}

TEST_CASE("steps_to_run drains the accumulator in whole fixed slices", "[sim][accumulator]") {
    const float fixed_dt = 1.0f / 120.0f;  // ~0.008333 s

    SECTION("a short frame banks time but runs zero steps") {
        float acc = 0.0f;
        int steps = sim::steps_to_run(acc, 0.004f, fixed_dt, 8);
        REQUIRE(steps == 0);
        REQUIRE(acc == Catch::Approx(0.004f));  // time is saved for later
    }

    SECTION("a 1/60 s frame runs exactly two 1/120 s steps") {
        float acc = 0.0f;
        int steps = sim::steps_to_run(acc, 1.0f / 60.0f, fixed_dt, 8);
        REQUIRE(steps == 2);
        REQUIRE(acc == Catch::Approx(0.0f).margin(1e-6));
    }

    SECTION("leftover time carries across frames") {
        float acc = 0.0f;
        // 0.01 s = one full step (0.00833..) with ~0.00167 s left over.
        int steps = sim::steps_to_run(acc, 0.01f, fixed_dt, 8);
        REQUIRE(steps == 1);
        REQUIRE(acc == Catch::Approx(0.01f - fixed_dt));
    }

    SECTION("the max_steps cap prevents a spiral of death") {
        float acc = 0.0f;
        // A giant 1-second frame would want 120 steps; the cap holds it to 8.
        int steps = sim::steps_to_run(acc, 1.0f, fixed_dt, 8);
        REQUIRE(steps == 8);
    }
}

TEST_CASE("clamp_frame_dt bounds a single frame's time", "[sim][accumulator]") {
    REQUIRE(sim::clamp_frame_dt(0.016f, 0.25f) == Catch::Approx(0.016f));  // normal frame
    REQUIRE(sim::clamp_frame_dt(3.0f, 0.25f) == Catch::Approx(0.25f));     // stall clamped
}

TEST_CASE("render_lerp blends previous and current positions", "[sim][render]") {
    Vec2f a{0.0f, 0.0f};
    Vec2f b{100.0f, 200.0f};
    REQUIRE(sim::render_lerp(a, b, 0.0f) == a);
    REQUIRE(sim::render_lerp(a, b, 1.0f) == b);
    Vec2f mid = sim::render_lerp(a, b, 0.5f);
    REQUIRE(mid.x == Catch::Approx(50.0f));
    REQUIRE(mid.y == Catch::Approx(100.0f));
}
