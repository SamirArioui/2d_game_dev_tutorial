// ============================================================================
// Pong logic implementation — pure functions, no SFML.  (YOUR TASK)
//
// This is the STARTER. Implement the game-rule bodies below using what the
// README taught (sections 2 and 3) so tests/test_pong.cpp goes from RED to
// GREEN and the pong app plays. The SFML shell — input, rendering, audio, and
// the game loop in src/main.cpp — is already complete; only the bodies marked
// TODO are yours. (collision/aabb.cpp is a separate TODO for this stage too.)
//
// A complete reference is in ../solution/src/pong.cpp — try it yourself first,
// then compare.
// ============================================================================
#include "pong/pong.hpp"

#include <algorithm>  // std::clamp
#include <cmath>      // std::abs

namespace pong {

collision::AABB paddle_box(const Paddle& p) { return collision::AABB{p.pos, p.size}; }
collision::AABB ball_box(const Ball& b) { return collision::AABB{b.pos, b.size}; }

void integrate(Ball& ball, float dt) {
    // TODO(stage 17->18 motion): advance the ball by velocity * time — the one
    // line of physics from stage 15: ball.pos += ball.vel * dt. dt is seconds.
    (void)ball;
    (void)dt;
}

void clamp_paddle(Paddle& p, const Court& court) {
    // TODO(stage 18): keep the whole paddle on screen vertically. Its top may
    // not go above 0, and its bottom (pos.y + size.y) may not pass court.height,
    // so clamp p.pos.y into [0, court.height - p.size.y] (std::clamp).
    (void)p;
    (void)court;
}

bool bounce_off_walls(Ball& ball, const Court& court) {
    // TODO(stage 18): reflect the ball off the TOP and BOTTOM walls; return true
    // if it bounced this call (the app beeps on true).
    //   * top    (ball.pos.y < 0): snap pos.y to 0 and force vel.y DOWNWARD
    //     (+std::abs(vel.y)) so a double-call can't leave it stuck in the wall.
    //   * bottom (ball.pos.y > court.height - ball.size.y): snap pos.y to that
    //     max and force vel.y UPWARD (-std::abs(vel.y)).
    (void)ball;
    (void)court;
    return false;  // placeholder
}

bool ball_hits_paddle(const Ball& ball, const Paddle& paddle) {
    // TODO(stage 18): does the ball overlap the paddle? Reuse the tested AABB
    // overlap: aabb_vs_aabb(ball_box(ball), paddle_box(paddle)).has_value().
    (void)ball;
    (void)paddle;
    return false;  // placeholder
}

void bounce_off_paddle(Ball& ball, const Paddle& paddle) {
    // TODO(stage 18): the classic Pong bounce (only call when ball_hits_paddle):
    //   1. remember the current speed = gmath::length(ball.vel).
    //   2. compute a contact offset in [-1, 1]: how far from the paddle's centre
    //      the ball hit, = (ball_center_y - paddle_center_y) / (paddle.size.y/2),
    //      clamped to [-1, 1].  (top -> -1 = up, bottom -> +1 = down)
    //   3. new horizontal direction is AWAY from the paddle: +1 if the ball was
    //      moving left (vel.x < 0), else -1.
    //   4. build dir = {dir_x, offset}, normalize it, and set vel = dir * speed
    //      so the ball keeps its speed but gains spin.
    //   5. NUDGE the ball just outside the paddle (set pos.x to paddle.pos.x -
    //      ball.size.x on the left side, or paddle.pos.x + paddle.size.x on the
    //      right) so it can't re-collide and stick next frame.
    (void)ball;
    (void)paddle;
}

Scorer check_scoring(const Ball& ball, const Court& court) {
    // TODO(stage 18): did the ball leave the court sideways?
    //   * fully past the LEFT edge  (pos.x + size.x < 0) -> Scorer::Right scores.
    //   * fully past the RIGHT edge (pos.x > court.width) -> Scorer::Left scores.
    //   * otherwise Scorer::None.
    (void)ball;
    (void)court;
    return Scorer::None;  // placeholder
}

Ball serve(const Court& court, bool to_left, float speed) {
    // TODO(stage 18): return a ball centred on the court, launched straight
    // horizontally (deterministic -> testable). Centre its box:
    //   pos.x = court.width*0.5 - size.x*0.5;  pos.y = court.height*0.5 - size.y*0.5;
    // and set vel = {to_left ? -speed : +speed, 0}.
    (void)court;
    (void)to_left;
    (void)speed;
    return Ball{};  // placeholder
}

GameState make_initial_state(const Court& court, bool serve_to_left) {
    // TODO(stage 18): build a fresh 0-0 game: left paddle hugging the left edge
    // (x ~= 24) vertically centred, right paddle hugging the right edge
    // (x = court.width - 24 - size.x) vertically centred, and ball = serve(court,
    // serve_to_left, DefaultBallSpeed).
    (void)court;
    (void)serve_to_left;
    return GameState{};  // placeholder
}

}  // namespace pong
