#pragma once

// ============================================================================
// Pong — the game LOGIC, with zero SFML.
//
// This is the payoff of the whole course so far: a real game's rules expressed
// as small PURE functions over plain data. No window, no input polling, no
// audio inside here — just "given this state and dt, what is the next state?".
// That is what lets `tests/test_pong.cpp` verify serving, bouncing, and scoring
// on a headless machine, and it reuses the tested pieces from earlier stages:
//   * motion   (stage 15): integrate() is pos += vel * dt.
//   * collision (stage 16): ball_hits_paddle() uses aabb_vs_aabb().
//
// `main.cpp` is then a thin shell: read the keyboard, call these functions,
// draw rectangles and text, beep. All the interesting behavior is here and
// tested.
// ============================================================================

#include "collision/aabb.hpp"
#include "gmath/vec2.hpp"

namespace pong {

// Default dimensions. Logical units are pixels; the window is `Court` sized.
inline constexpr float DefaultBallSpeed = 420.0f;  // px/sec

// The playfield. Origin (0,0) is the top-left; +y points DOWN (SFML convention).
struct Court {
    float width = 800.0f;
    float height = 600.0f;
};

// A paddle: a tall box that moves vertically. `speed` is its move rate.
struct Paddle {
    gmath::Vec2f pos;                 // top-left corner
    gmath::Vec2f size{16.0f, 100.0f};
    float speed = 480.0f;             // px/sec when a key is held
};

// The ball: a small box with a velocity.
struct Ball {
    gmath::Vec2f pos;                 // top-left corner
    gmath::Vec2f size{14.0f, 14.0f};
    gmath::Vec2f vel;                 // px/sec
};

// Who just scored (if anyone) this frame.
enum class Scorer { None, Left, Right };

// The whole game state in one struct — like a small dataclass in Python.
struct GameState {
    Paddle left;
    Paddle right;
    Ball ball;
    int score_left = 0;
    int score_right = 0;
};

// --- viewing entities as collision boxes ------------------------------------
collision::AABB paddle_box(const Paddle& p);
collision::AABB ball_box(const Ball& b);

// --- pure step functions (each does ONE thing, so each is easy to test) -----

// Advance the ball by dt (reuse of stage-15 motion).
void integrate(Ball& ball, float dt);

// Keep a paddle fully inside the court vertically.
void clamp_paddle(Paddle& p, const Court& court);

// Reflect the ball off the top/bottom walls. Returns true if it bounced this
// call (the app plays a beep on true). Snaps the ball back in-bounds and makes
// its vertical velocity point away from the wall, so it can't stick.
bool bounce_off_walls(Ball& ball, const Court& court);

// Do the ball and paddle overlap? (AABB vs AABB.)
bool ball_hits_paddle(const Ball& ball, const Paddle& paddle);

// Bounce the ball off a paddle: reverse horizontal direction, add vertical
// "spin" based on WHERE it hit (top of paddle -> upward, bottom -> downward),
// preserve overall speed, and nudge the ball clear so it won't re-collide next
// frame. Only call this when ball_hits_paddle() is true.
void bounce_off_paddle(Ball& ball, const Paddle& paddle);

// Did the ball leave the court left/right? Left paddle defends the left edge:
// if the ball exits the LEFT, the RIGHT player scores, and vice versa.
Scorer check_scoring(const Ball& ball, const Court& court);

// Put the ball back at the centre, launched toward the left (to_left=true) or
// the right, at `speed`. Deterministic (no randomness) so tests can pin it.
Ball serve(const Court& court, bool to_left, float speed);

// A fresh game: paddles centred at each edge, ball served, scores 0-0.
GameState make_initial_state(const Court& court, bool serve_to_left = true);

}  // namespace pong
