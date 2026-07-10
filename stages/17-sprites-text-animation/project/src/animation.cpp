// ============================================================================
// Implementation of the SFML-free animation core.  (YOUR TASK)
//
// This is the STARTER. Implement the frame-math and controller bodies below
// using what the README taught (sections 3 and 4), so tests/test_animation.cpp
// goes from RED to GREEN and the walk_cycle app animates. The boilerplate — the
// window/sprite-sheet code in src/main.cpp, the sf::Text setup, and all the
// header declarations — is already done; only the bodies marked TODO are yours.
//
// A complete reference is in ../solution/src/animation.cpp — try it yourself
// first, then compare.
// ============================================================================
#include "anim/animation.hpp"

#include <algorithm>  // std::max, std::min

namespace anim {

int frame_index(const Clip& clip, float elapsed) {
    // TODO(stage 17): return WHICH frame (0 .. frame_count-1) shows after
    // `elapsed` seconds. Steps:
    //   * a clip with frame_count <= 0 has nothing to show -> return 0.
    //   * clamp negative `elapsed` up to 0 (std::max) so we never index before
    //     the start.
    //   * whole frames played = int(elapsed * clip.fps)  (truncates toward 0).
    //   * if clip.loop: wrap with % clip.frame_count.
    //   * otherwise (one-shot): hold on the last frame -> std::min(advanced,
    //     clip.frame_count - 1).
    (void)clip;
    (void)elapsed;
    return 0;  // placeholder
}

IntRect frame_to_rect(int frame, int columns, int frame_w, int frame_h) {
    // TODO(stage 17): map a sheet `frame` index to its pixel rectangle on a grid
    // that is `columns` frames wide, each frame frame_w x frame_h. Guard columns
    // to at least 1 (avoid divide-by-zero), then col = frame % columns,
    // row = frame / columns, and return
    // IntRect{col*frame_w, row*frame_h, frame_w, frame_h}.
    (void)frame;
    (void)columns;
    (void)frame_w;
    (void)frame_h;
    return IntRect{};  // placeholder
}

Animator::Animator(Clip idle, Clip walk) : idle_(idle), walk_(walk) {}

void Animator::set_state(State s) {
    // TODO(stage 17): switch to state `s`, but reset the playback timer
    // (elapsed_ = 0) ONLY when the state actually CHANGES. Re-asserting the same
    // state every frame must NOT rewind the animation (the classic freeze-on-
    // frame-0 bug the tests check for).
    (void)s;
}

void Animator::update(float dt) {
    // TODO(stage 17): advance playback by `dt` seconds (accumulate into elapsed_).
    (void)dt;
}

const Clip& Animator::active_clip() const { return state_ == State::Walk ? walk_ : idle_; }

int Animator::current_frame_in_clip() const {
    // TODO(stage 17): return frame_index() of the ACTIVE clip at elapsed_.
    return 0;  // placeholder
}

int Animator::current_sheet_frame() const {
    // TODO(stage 17): absolute sheet index = active clip's first_frame +
    // current_frame_in_clip().
    return 0;  // placeholder
}

}  // namespace anim
