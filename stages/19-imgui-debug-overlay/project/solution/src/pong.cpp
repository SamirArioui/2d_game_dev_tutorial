// ============================================================================
// Pong logic implementation — pure functions, no SFML.
// ============================================================================
#include "pong/pong.hpp"

#include <algorithm>  // std::clamp
#include <cmath>      // std::abs

namespace pong {

collision::AABB paddle_box(const Paddle& p) { return collision::AABB{p.pos, p.size}; }
collision::AABB ball_box(const Ball& b) { return collision::AABB{b.pos, b.size}; }

void integrate(Ball& ball, float dt) {
    // The one line of physics: position changes by velocity times time
    // (stage 15). dt is always seconds.
    ball.pos += ball.vel * dt;
}

void clamp_paddle(Paddle& p, const Court& court) {
    const float max_y = court.height - p.size.y;
    p.pos.y = std::clamp(p.pos.y, 0.0f, max_y);
}

bool bounce_off_walls(Ball& ball, const Court& court) {
    bool bounced = false;

    // Top wall: snap to 0 and force velocity DOWNWARD (+y). Using abs() instead
    // of just negating avoids the "stuck vibrating in the wall" bug if we get
    // called twice before the ball clears.
    if (ball.pos.y < 0.0f) {
        ball.pos.y = 0.0f;
        ball.vel.y = std::abs(ball.vel.y);
        bounced = true;
    }

    // Bottom wall: snap so the ball's bottom edge sits on the floor, force
    // velocity UPWARD (-y).
    const float max_y = court.height - ball.size.y;
    if (ball.pos.y > max_y) {
        ball.pos.y = max_y;
        ball.vel.y = -std::abs(ball.vel.y);
        bounced = true;
    }

    return bounced;
}

bool ball_hits_paddle(const Ball& ball, const Paddle& paddle) {
    // Reuse the tested AABB overlap from stage 16.
    return collision::aabb_vs_aabb(ball_box(ball), paddle_box(paddle)).has_value();
}

void bounce_off_paddle(Ball& ball, const Paddle& paddle) {
    // Speed we want to keep after the bounce.
    const float speed = gmath::length(ball.vel);

    // How far up/down the paddle we hit, mapped to [-1, 1]:
    //   -1 = hit the very top, 0 = dead centre, +1 = the very bottom.
    const float paddle_center_y = paddle.pos.y + paddle.size.y * 0.5f;
    const float ball_center_y = ball.pos.y + ball.size.y * 0.5f;
    float offset = (ball_center_y - paddle_center_y) / (paddle.size.y * 0.5f);
    offset = std::clamp(offset, -1.0f, 1.0f);

    // New horizontal direction: away from the paddle it just hit.
    const float dir_x = (ball.vel.x < 0.0f) ? 1.0f : -1.0f;

    // Build a new velocity: mostly horizontal, with spin from the contact
    // offset, then renormalize so the ball keeps its speed (classic Pong feel).
    gmath::Vec2f new_dir{dir_x, offset};
    new_dir = gmath::normalize(new_dir);
    ball.vel = new_dir * speed;

    // Nudge the ball just outside the paddle so next frame's overlap test is
    // false and we don't bounce repeatedly (a "sticky paddle" bug otherwise).
    const float paddle_center_x = paddle.pos.x + paddle.size.x * 0.5f;
    const float ball_center_x = ball.pos.x + ball.size.x * 0.5f;
    if (ball_center_x < paddle_center_x) {
        ball.pos.x = paddle.pos.x - ball.size.x;  // ball is left of paddle
    } else {
        ball.pos.x = paddle.pos.x + paddle.size.x;  // ball is right of paddle
    }
}

Scorer check_scoring(const Ball& ball, const Court& court) {
    const float ball_right = ball.pos.x + ball.size.x;
    if (ball_right < 0.0f) {
        return Scorer::Right;  // ball left via the LEFT edge -> right player scores
    }
    if (ball.pos.x > court.width) {
        return Scorer::Left;  // ball left via the RIGHT edge -> left player scores
    }
    return Scorer::None;
}

Ball serve(const Court& court, bool to_left, float speed) {
    Ball ball;  // default size
    // Centre the ball's box on the court centre.
    ball.pos.x = court.width * 0.5f - ball.size.x * 0.5f;
    ball.pos.y = court.height * 0.5f - ball.size.y * 0.5f;
    // Straight horizontal launch (deterministic -> testable). The paddle spin
    // adds vertical motion once play begins.
    ball.vel = gmath::Vec2f{to_left ? -speed : speed, 0.0f};
    return ball;
}

GameState make_initial_state(const Court& court, bool serve_to_left) {
    GameState s;

    // Left paddle hugs the left edge, vertically centred.
    s.left.pos = gmath::Vec2f{24.0f, court.height * 0.5f - s.left.size.y * 0.5f};
    // Right paddle hugs the right edge.
    s.right.pos =
        gmath::Vec2f{court.width - 24.0f - s.right.size.x, court.height * 0.5f - s.right.size.y * 0.5f};

    s.ball = serve(court, serve_to_left, DefaultBallSpeed);
    return s;
}

}  // namespace pong
