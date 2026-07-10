// ============================================================================
// Tests for the pure Pong rules. Because pong::step is a pure function over
// plain data, we can drive the whole game forward in a test without ever
// opening a window — set a state, step it, assert the outcome.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include "eng/math.hpp"
#include "pong_logic.hpp"

using gmath::Vec2f;
using pong::Config;
using pong::Event;
using pong::State;

namespace {

// A small, tidy court so hand-computed expectations are easy.
Config small_court() {
    Config c;
    c.court_w = 200.0f;
    c.court_h = 100.0f;
    c.paddle_w = 10.0f;
    c.paddle_h = 40.0f;
    c.ball_size = 10.0f;
    c.paddle_margin = 10.0f;
    c.paddle_speed = 100.0f;
    c.ball_speed = 100.0f;
    return c;
}

}  // namespace

TEST_CASE("make_initial centres the paddles and serves from the middle", "[pong]") {
    const Config c = small_court();
    const State s = pong::make_initial(c);

    REQUIRE(s.left_score == 0);
    REQUIRE(s.right_score == 0);
    REQUIRE(s.left_paddle_y == (c.court_h - c.paddle_h) * 0.5f);
    REQUIRE(s.ball_pos.x == (c.court_w - c.ball_size) * 0.5f);
}

TEST_CASE("ball bounces off the top wall (vertical velocity flips down)", "[pong]") {
    const Config c = small_court();
    State s = pong::make_initial(c);
    s.ball_pos = Vec2f{100.0f, 1.0f};
    s.ball_vel = Vec2f{0.0f, -100.0f};  // heading up, into the top wall

    const Event event = pong::step(s, c, 0.1f, 0.0f, 0.0f);

    REQUIRE(event == Event::WallBounce);
    REQUIRE(s.ball_vel.y > 0.0f);   // now heading down
    REQUIRE(s.ball_pos.y == 0.0f);  // snapped to the wall
}

TEST_CASE("paddle movement is clamped to the court", "[pong]") {
    const Config c = small_court();
    State s = pong::make_initial(c);
    s.left_paddle_y = 0.0f;

    // Hold "up" hard for a long step; the paddle cannot go above y = 0.
    pong::step(s, c, 1.0f, -1.0f, 0.0f);
    REQUIRE(s.left_paddle_y == 0.0f);

    // Hold "down" hard; it cannot go below court_h - paddle_h.
    pong::step(s, c, 1.0f, +1.0f, 0.0f);
    REQUIRE(s.left_paddle_y == c.court_h - c.paddle_h);
}

TEST_CASE("ball bounces off the left paddle (horizontal velocity flips right)", "[pong]") {
    const Config c = small_court();
    State s = pong::make_initial(c);

    // Place the ball just touching the front face of the left paddle, moving left.
    s.left_paddle_y = 30.0f;                       // paddle spans y [30, 70]
    s.ball_pos = Vec2f{c.paddle_margin + c.paddle_w - 2.0f, 45.0f};  // overlapping by 2
    s.ball_vel = Vec2f{-100.0f, 0.0f};

    const Event event = pong::step(s, c, 0.0f, 0.0f, 0.0f);  // dt=0 isolates the bounce

    REQUIRE(event == Event::PaddleBounce);
    REQUIRE(s.ball_vel.x > 0.0f);  // now heading right, away from the paddle
}

TEST_CASE("ball past the right edge scores for the left player and resets", "[pong]") {
    const Config c = small_court();
    State s = pong::make_initial(c);
    s.ball_pos = Vec2f{c.court_w + 1.0f, 50.0f};  // already off the right edge
    s.ball_vel = Vec2f{100.0f, 0.0f};

    const Event event = pong::step(s, c, 0.1f, 0.0f, 0.0f);

    REQUIRE(event == Event::ScoreLeft);
    REQUIRE(s.left_score == 1);
    REQUIRE(s.right_score == 0);
    // Ball re-served from the centre.
    REQUIRE(s.ball_pos.x == (c.court_w - c.ball_size) * 0.5f);
}
