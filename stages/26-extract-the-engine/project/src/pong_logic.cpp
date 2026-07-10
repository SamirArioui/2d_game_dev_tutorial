// ============================================================================
// Stage 26 mini-project — Pong RULES (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the pure Pong rules declared in pong_logic.hpp
// using what the stage taught: plain data + free functions over gmath, reusing
// the engine's gmath::aabb_vs_aabb for the paddle bounces. NO SFML belongs here —
// that is the whole point of the pure/impure seam, and it is why the tests can
// drive these functions completely headless.
//
// The Config/State/Event types and the declarations you implement against are in
// pong_logic.hpp. The starter already compiles and links (each body has a
// placeholder), so `pong` and `pong_tests` build — but the pong tests in tests/
// are RED until you fill the TODOs in.
//
// A complete reference is in ../solution/src/pong_logic.cpp — try it yourself first.
//
// Build & test (from stages/26-extract-the-engine/project):
//   cmake -S . -B build && cmake --build build
//   ctest --test-dir build --output-on-failure
// ============================================================================
#include "pong_logic.hpp"

#include <cmath>

namespace pong {

State make_initial(const Config& c) {
    // TODO(stage 26): build a fresh match state:
    //   - centre both paddles vertically: left_paddle_y = right_paddle_y =
    //     (court_h - paddle_h) * 0.5f
    //   - serve the ball from the court centre moving LEFT on the first serve:
    //     ball_pos = centre of the court, ball_vel = {-ball_speed, ball_speed*0.3f}
    //   - zero both scores
    // Tip: a small local reset_ball(dir_x) helper keeps make_initial and step DRY.
    (void)c;
    return State{};  // placeholder
}

gmath::AABB left_paddle_box(const State& s, const Config& c) {
    // TODO(stage 26): return the left paddle's collider — top-left at
    // {paddle_margin, left_paddle_y}, size {paddle_w, paddle_h}.
    (void)s;
    (void)c;
    return gmath::AABB{};  // placeholder
}

gmath::AABB right_paddle_box(const State& s, const Config& c) {
    // TODO(stage 26): like left_paddle_box, but the right paddle sits in from the
    // RIGHT wall: x = court_w - paddle_margin - paddle_w.
    (void)s;
    (void)c;
    return gmath::AABB{};  // placeholder
}

gmath::AABB ball_box(const State& s, const Config& c) {
    // TODO(stage 26): return the ball's collider — top-left ball_pos, size
    // {ball_size, ball_size}.
    (void)s;
    (void)c;
    return gmath::AABB{};  // placeholder
}

Event step(State& s, const Config& c, float dt, float move_left, float move_right) {
    // TODO(stage 26): advance the simulation by dt and return the most significant
    // Event this step (the Scene uses it to decide which sound to play). In order:
    //   1. Move each paddle by move_* * paddle_speed * dt, clamped with
    //      gmath::clamp so it stays fully inside the court [0, court_h - paddle_h].
    //   2. Move the ball: ball_pos += ball_vel * dt.
    //   3. Bounce off the top/bottom walls: snap the ball flush to the wall and
    //      FORCE the vertical velocity sign (+/- std::abs). Record Event::WallBounce.
    //   4. Bounce off the paddles with gmath::aabb_vs_aabb(ball_box, *_paddle_box):
    //      on a hit while moving toward that paddle, add the penetration to
    //      ball_pos and flip ball_vel.x away. Record Event::PaddleBounce.
    //   5. Score when the ball fully leaves sideways: the LEFT player scores when
    //      it passes the RIGHT edge (Event::ScoreLeft), the RIGHT player on the
    //      LEFT edge (Event::ScoreRight); bump the score and re-serve. Scoring
    //      outranks any bounce this frame, so return immediately.
    (void)s;
    (void)c;
    (void)dt;
    (void)move_left;
    (void)move_right;
    return Event::None;  // placeholder
}

}  // namespace pong
