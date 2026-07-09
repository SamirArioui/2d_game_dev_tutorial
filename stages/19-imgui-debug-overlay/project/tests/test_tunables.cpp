// ============================================================================
// Tests for the debug-overlay helpers.
//
// The ImGui panel itself can't be unit-tested here (it needs a GL context and a
// window), but everything it DOES to the game is pure and lives in debug/. So
// we test that: the slider's rescale keeps the ball's direction, and the FPS
// profiler averages correctly. The overlay code in main.cpp then just wires
// these tested pieces to widgets.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "debug/tunables.hpp"
#include "pong/pong.hpp"

using debug::FrameProfiler;
using debug::set_ball_speed;
using gmath::Vec2f;

TEST_CASE("set_ball_speed rescales magnitude but keeps direction", "[debug][speed]") {
    pong::Ball ball;
    ball.vel = Vec2f{3.0f, 4.0f};  // length 5, direction (0.6, 0.8)

    set_ball_speed(ball, 500.0f);

    REQUIRE(gmath::length(ball.vel) == Catch::Approx(500.0f));
    // Direction preserved: components stay in the original 3:4 ratio.
    REQUIRE(ball.vel.x == Catch::Approx(300.0f));  // 0.6 * 500
    REQUIRE(ball.vel.y == Catch::Approx(400.0f));  // 0.8 * 500
}

TEST_CASE("set_ball_speed handles a still ball gracefully", "[debug][speed][edge]") {
    pong::Ball ball;
    ball.vel = Vec2f{0.0f, 0.0f};  // no direction to preserve

    set_ball_speed(ball, 250.0f);

    // Falls back to a rightward launch at the requested speed (no NaNs).
    REQUIRE(gmath::length(ball.vel) == Catch::Approx(250.0f));
    REQUIRE(ball.vel.x == Catch::Approx(250.0f));
    REQUIRE(ball.vel.y == Catch::Approx(0.0f));
}

TEST_CASE("FrameProfiler averages recent frame times", "[debug][profiler]") {
    FrameProfiler profiler(4);  // small window for an easy hand-check

    SECTION("empty profiler reports zero, not a divide-by-zero") {
        REQUIRE(profiler.sample_count() == 0);
        REQUIRE(profiler.average_dt() == Catch::Approx(0.0f));
        REQUIRE(profiler.fps() == Catch::Approx(0.0f));
    }

    SECTION("a steady 60 fps stream averages to ~1/60 s and 60 fps") {
        for (int i = 0; i < 10; ++i) {
            profiler.add_sample(1.0f / 60.0f);
        }
        REQUIRE(profiler.sample_count() == 4);  // window caps the count
        REQUIRE(profiler.average_dt() == Catch::Approx(1.0f / 60.0f));
        REQUIRE(profiler.fps() == Catch::Approx(60.0f));
    }

    SECTION("the window forgets old samples") {
        // Fill with slow frames, then flood with fast ones.
        for (int i = 0; i < 4; ++i) {
            profiler.add_sample(0.1f);  // 10 fps
        }
        REQUIRE(profiler.average_dt() == Catch::Approx(0.1f));
        for (int i = 0; i < 4; ++i) {
            profiler.add_sample(0.02f);  // 50 fps
        }
        // The four slow samples have all been evicted.
        REQUIRE(profiler.average_dt() == Catch::Approx(0.02f));
        REQUIRE(profiler.fps() == Catch::Approx(50.0f));
    }
}
