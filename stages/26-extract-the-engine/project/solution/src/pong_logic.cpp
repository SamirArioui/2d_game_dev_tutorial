// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
#include "pong_logic.hpp"

#include <cmath>

namespace pong {

namespace {

// Re-serve the ball from the centre. `dir_x` is +1 (serve rightward) or -1
// (leftward). Vertical speed is a fixed fraction so behaviour is deterministic
// (important: the unit tests rely on knowing exactly what a reset does).
void reset_ball(State& s, const Config& c, float dir_x) {
    s.ball_pos = gmath::Vec2f{(c.court_w - c.ball_size) * 0.5f, (c.court_h - c.ball_size) * 0.5f};
    s.ball_vel = gmath::Vec2f{dir_x * c.ball_speed, c.ball_speed * 0.3f};
}

}  // namespace

State make_initial(const Config& c) {
    State s;
    s.left_paddle_y = (c.court_h - c.paddle_h) * 0.5f;
    s.right_paddle_y = (c.court_h - c.paddle_h) * 0.5f;
    reset_ball(s, c, -1.0f);  // first serve goes left
    s.left_score = 0;
    s.right_score = 0;
    return s;
}

gmath::AABB left_paddle_box(const State& s, const Config& c) {
    return gmath::AABB{{c.paddle_margin, s.left_paddle_y}, {c.paddle_w, c.paddle_h}};
}

gmath::AABB right_paddle_box(const State& s, const Config& c) {
    return gmath::AABB{{c.court_w - c.paddle_margin - c.paddle_w, s.right_paddle_y},
                       {c.paddle_w, c.paddle_h}};
}

gmath::AABB ball_box(const State& s, const Config& c) {
    return gmath::AABB{s.ball_pos, {c.ball_size, c.ball_size}};
}

Event step(State& s, const Config& c, float dt, float move_left, float move_right) {
    Event event = Event::None;

    // 1) Move paddles, clamped so they stay fully inside the court.
    const float max_y = c.court_h - c.paddle_h;
    s.left_paddle_y = gmath::clamp(s.left_paddle_y + move_left * c.paddle_speed * dt, 0.0f, max_y);
    s.right_paddle_y =
        gmath::clamp(s.right_paddle_y + move_right * c.paddle_speed * dt, 0.0f, max_y);

    // 2) Move the ball.
    s.ball_pos += s.ball_vel * dt;

    // 3) Bounce off the top and bottom walls. We snap the ball back to the wall
    //    and FORCE the vertical velocity sign (abs / -abs) rather than just
    //    negating it, so a ball already moving the right way can't get "stuck".
    if (s.ball_pos.y <= 0.0f) {
        s.ball_pos.y = 0.0f;
        s.ball_vel.y = std::abs(s.ball_vel.y);
        event = Event::WallBounce;
    } else if (s.ball_pos.y + c.ball_size >= c.court_h) {
        s.ball_pos.y = c.court_h - c.ball_size;
        s.ball_vel.y = -std::abs(s.ball_vel.y);
        event = Event::WallBounce;
    }

    // 4) Bounce off the paddles. Reuse the engine's aabb_vs_aabb: the penetration
    //    vector pushes the ball back out, and we flip horizontal velocity away
    //    from the paddle it hit. (Adding vertical "english" is an exercise.)
    const gmath::Hit left_hit = gmath::aabb_vs_aabb(ball_box(s, c), left_paddle_box(s, c));
    if (left_hit.collided && s.ball_vel.x < 0.0f) {
        s.ball_pos += left_hit.penetration;
        s.ball_vel.x = std::abs(s.ball_vel.x);
        event = Event::PaddleBounce;
    }
    const gmath::Hit right_hit = gmath::aabb_vs_aabb(ball_box(s, c), right_paddle_box(s, c));
    if (right_hit.collided && s.ball_vel.x > 0.0f) {
        s.ball_pos += right_hit.penetration;
        s.ball_vel.x = -std::abs(s.ball_vel.x);
        event = Event::PaddleBounce;
    }

    // 5) Scoring: a point is scored when the ball leaves the court sideways.
    //    Scoring outranks any bounce this frame, so we return immediately.
    if (s.ball_pos.x > c.court_w) {
        s.left_score += 1;
        reset_ball(s, c, +1.0f);
        return Event::ScoreLeft;
    }
    if (s.ball_pos.x + c.ball_size < 0.0f) {
        s.right_score += 1;
        reset_ball(s, c, -1.0f);
        return Event::ScoreRight;
    }

    return event;
}

}  // namespace pong
