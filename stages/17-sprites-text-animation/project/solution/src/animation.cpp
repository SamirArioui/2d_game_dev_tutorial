// ============================================================================
// Implementation of the SFML-free animation core.  (REFERENCE SOLUTION)
//
// This is the complete implementation. The starter you fill in is one level up
// at ../../src/animation.cpp — try it yourself first, then compare.
// ============================================================================
#include "anim/animation.hpp"

#include <algorithm>  // std::max, std::min

namespace anim {

int frame_index(const Clip& clip, float elapsed) {
    // Degenerate clip: nothing to show, so frame 0.
    if (clip.frame_count <= 0) {
        return 0;
    }
    // Negative time can happen if a caller subtracts too much; clamp to 0 so we
    // never index before the start.
    const float t = std::max(0.0f, elapsed);

    // How many whole frames have played. int() truncates toward zero, which is
    // exactly "the frame we are currently sitting on".
    const int frames_advanced = static_cast<int>(t * clip.fps);

    if (clip.loop) {
        // Wrap: after the last frame, start over. Modulo does this cleanly.
        return frames_advanced % clip.frame_count;
    }
    // One-shot: hold on the final frame once we reach it.
    return std::min(frames_advanced, clip.frame_count - 1);
}

IntRect frame_to_rect(int frame, int columns, int frame_w, int frame_h) {
    // Guard against a zero/negative column count so we never divide by zero.
    const int cols = std::max(1, columns);
    const int col = frame % cols;
    const int row = frame / cols;
    return IntRect{col * frame_w, row * frame_h, frame_w, frame_h};
}

Animator::Animator(Clip idle, Clip walk) : idle_(idle), walk_(walk) {}

void Animator::set_state(State s) {
    if (s != state_) {
        state_ = s;
        elapsed_ = 0.0f;  // restart the new clip from its first frame
    }
}

void Animator::update(float dt) { elapsed_ += dt; }

const Clip& Animator::active_clip() const { return state_ == State::Walk ? walk_ : idle_; }

int Animator::current_frame_in_clip() const { return frame_index(active_clip(), elapsed_); }

int Animator::current_sheet_frame() const {
    return active_clip().first_frame + current_frame_in_clip();
}

}  // namespace anim
