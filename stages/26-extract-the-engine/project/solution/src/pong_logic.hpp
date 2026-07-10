#pragma once

// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
// ============================================================================
// Pong RULES — pure, SFML-free, unit-tested.
//
// This is the whole point of the engine split: the game's LOGIC (where the ball
// is, when it bounces, who scored) knows nothing about windows or drawing. It is
// plain data + free functions over gmath, so it runs and tests headless. The
// Scene (pong_scene.*) is the thin layer that draws this state and feeds it input.
// ============================================================================

#include "eng/collision.hpp"
#include "eng/math.hpp"

namespace pong {

// Fixed dimensions/speeds for one match. Kept in a struct so tests can build a
// small court and the game can use a big one.
struct Config {
    float court_w{800.0f};
    float court_h{600.0f};
    float paddle_w{16.0f};
    float paddle_h{100.0f};
    float ball_size{14.0f};
    float paddle_margin{30.0f};  // distance of each paddle from its wall
    float paddle_speed{460.0f};  // px/s
    float ball_speed{380.0f};    // px/s
};

// Everything that changes during play. `*_paddle_y` is the paddle's TOP edge.
struct State {
    gmath::Vec2f ball_pos;   // top-left of the ball
    gmath::Vec2f ball_vel;
    float left_paddle_y{0.0f};
    float right_paddle_y{0.0f};
    int left_score{0};
    int right_score{0};
};

// What happened during a step, so the Scene knows when to play a sound.
enum class Event {
    None,
    WallBounce,
    PaddleBounce,
    ScoreLeft,   // left player scored (ball passed the RIGHT edge)
    ScoreRight,  // right player scored (ball passed the LEFT edge)
};

// Build a fresh state: paddles centred, ball served from the middle.
State make_initial(const Config& c);

// Collider boxes, derived from the state (so rendering and physics agree).
gmath::AABB left_paddle_box(const State& s, const Config& c);
gmath::AABB right_paddle_box(const State& s, const Config& c);
gmath::AABB ball_box(const State& s, const Config& c);

// Advance the simulation by dt. `move_left`/`move_right` are in [-1, 1]
// (-1 = up, +1 = down). Returns the most significant Event this step for audio.
Event step(State& s, const Config& c, float dt, float move_left, float move_right);

}  // namespace pong
