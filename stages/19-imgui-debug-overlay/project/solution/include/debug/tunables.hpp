#pragma once

// ============================================================================
// SFML-free helpers behind the ImGui debug overlay.
//
// The overlay itself (ImGui calls) lives in main.cpp, but the two things it
// actually DOES to the game are pure logic, so they live here and get tested:
//   * set_ball_speed(): the "Ball speed" slider rescales the ball's velocity
//     WITHOUT changing its direction — pure vector math.
//   * FrameProfiler: the FPS readout is a rolling average of recent frame
//     times — a tiny ring buffer, pure and testable.
//
// Python comparison for the overlay as a whole is in the README: ImGui is
// IMMEDIATE-mode UI (you re-issue the whole panel every frame) versus the
// RETAINED-mode widget trees of tkinter/Qt (build once, mutate later).
// ============================================================================

#include <vector>

#include "gmath/vec2.hpp"
#include "pong/pong.hpp"

namespace debug {

// Rescale the ball's velocity to `speed` while preserving its direction. If the
// ball is momentarily still (zero velocity), fall back to a rightward launch so
// the slider still does something sensible.
void set_ball_speed(pong::Ball& ball, float speed);

// A fixed-window rolling average of frame times, so the FPS number in the
// overlay is smooth instead of jittering wildly every frame.
class FrameProfiler {
public:
    // `window` = how many recent frames to average over.
    explicit FrameProfiler(int window = 120);

    // Record one frame's delta time (seconds). Oldest sample drops out once the
    // window is full.
    void add_sample(float dt_seconds);

    // Average frame time over the window (0 if no samples yet).
    float average_dt() const;

    // Frames per second implied by the average (0 if not enough data).
    float fps() const;

    // How many samples are currently held.
    int sample_count() const { return count_; }

private:
    int window_;                 // capacity
    int count_ = 0;              // samples currently stored (<= window_)
    int next_ = 0;               // ring-buffer write index
    float sum_ = 0.0f;           // running sum of stored samples
    std::vector<float> samples_;  // the ring buffer
};

}  // namespace debug
