// ============================================================================
// Implementation of the debug-overlay helpers (SFML-free, unit-tested).  (YOUR TASK)
//
// This is the STARTER. The Pong rules (src/pong.cpp, src/aabb.cpp) are carried
// over COMPLETE from stage 18 and the ImGui/window boilerplate in src/main.cpp
// is done — your job this stage is only the two debug helpers below. Implement
// them using what the README taught (sections 3 and 4) so
// tests/test_tunables.cpp goes from RED to GREEN. (test_pong / test_collision
// are already green here — the game logic is unchanged from stage 18.)
//
// A complete reference is in ../solution/src/tunables.cpp — try it yourself
// first, then compare.
// ============================================================================
#include "debug/tunables.hpp"

#include <algorithm>  // std::max

namespace debug {

void set_ball_speed(pong::Ball& ball, float speed) {
    // TODO(stage 19): rescale the ball's velocity to `speed` while KEEPING its
    // direction. Take dir = gmath::normalize(ball.vel); normalize() returns the
    // zero vector unchanged, so if the ball is still (dir is {0,0}) fall back to
    // a rightward launch dir = {1, 0}. Then set ball.vel = dir * speed.
    (void)ball;
    (void)speed;
}

FrameProfiler::FrameProfiler(int window)
    : window_(std::max(1, window)), samples_(static_cast<std::size_t>(std::max(1, window)), 0.0f) {}

void FrameProfiler::add_sample(float dt_seconds) {
    // TODO(stage 19): push one frame time into the fixed-size ring buffer,
    // keeping the running `sum_` correct so average_dt() stays O(1):
    //   * while not full (count_ < window_): add dt to sum_, store it at
    //     samples_[next_], and ++count_.
    //   * once full: SUBTRACT the oldest sample (samples_[next_]) from sum_
    //     before overwriting it with dt and adding dt to sum_.
    //   * then advance the write index: next_ = (next_ + 1) % window_.
    (void)dt_seconds;
}

float FrameProfiler::average_dt() const {
    // TODO(stage 19): mean frame time = sum_ / count_, but return 0 (not a
    // divide-by-zero) when count_ == 0.
    return 0.0f;  // placeholder
}

float FrameProfiler::fps() const {
    // TODO(stage 19): frames per second = 1 / average_dt(), returning 0 when the
    // average is <= 0 (no samples yet).
    return 0.0f;  // placeholder
}

}  // namespace debug
