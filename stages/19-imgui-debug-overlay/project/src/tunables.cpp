// ============================================================================
// Implementation of the debug-overlay helpers (SFML-free, unit-tested).
// ============================================================================
#include "debug/tunables.hpp"

#include <algorithm>  // std::max

namespace debug {

void set_ball_speed(pong::Ball& ball, float speed) {
    gmath::Vec2f dir = gmath::normalize(ball.vel);
    // normalize() returns the zero vector unchanged; if the ball is still, pick
    // a default direction so dragging the slider still launches it.
    if (dir.x == 0.0f && dir.y == 0.0f) {
        dir = gmath::Vec2f{1.0f, 0.0f};
    }
    ball.vel = dir * speed;
}

FrameProfiler::FrameProfiler(int window)
    : window_(std::max(1, window)), samples_(static_cast<std::size_t>(std::max(1, window)), 0.0f) {}

void FrameProfiler::add_sample(float dt_seconds) {
    if (count_ < window_) {
        // Still filling the window: just append.
        sum_ += dt_seconds;
        samples_[static_cast<std::size_t>(next_)] = dt_seconds;
        ++count_;
    } else {
        // Full: overwrite the oldest sample, keeping the running sum correct.
        sum_ -= samples_[static_cast<std::size_t>(next_)];
        sum_ += dt_seconds;
        samples_[static_cast<std::size_t>(next_)] = dt_seconds;
    }
    next_ = (next_ + 1) % window_;
}

float FrameProfiler::average_dt() const {
    if (count_ == 0) {
        return 0.0f;
    }
    return sum_ / static_cast<float>(count_);
}

float FrameProfiler::fps() const {
    const float avg = average_dt();
    if (avg <= 0.0f) {
        return 0.0f;
    }
    return 1.0f / avg;
}

}  // namespace debug
