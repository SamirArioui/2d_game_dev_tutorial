// ============================================================================
// Tests for the SFML-free animation core.
//
// None of this needs a window, a texture, or a GPU — it is pure arithmetic and
// a small state machine. That is the takeaway of the stage: pull the logic out
// of the render layer and it becomes as testable as any of the math from
// stage 13. These tests run under `ctest` on any machine, headless included.
// ============================================================================

#include <catch2/catch_test_macros.hpp>

#include "anim/animation.hpp"

using anim::Animator;
using anim::Clip;
using anim::frame_index;
using anim::frame_to_rect;
using anim::IntRect;
using anim::State;

TEST_CASE("frame_index advances with elapsed time", "[anim][frame]") {
    // 4-frame clip at 10 fps => each frame lasts 0.1s.
    const Clip walk{/*first_frame=*/0, /*frame_count=*/4, /*fps=*/10.0f, /*loop=*/true};

    SECTION("at t=0 we are on the first frame") { REQUIRE(frame_index(walk, 0.0f) == 0); }
    SECTION("just before the second frame we are still on the first") {
        REQUIRE(frame_index(walk, 0.09f) == 0);
    }
    SECTION("at exactly one frame duration we advance") {
        REQUIRE(frame_index(walk, 0.10f) == 1);
        REQUIRE(frame_index(walk, 0.35f) == 3);
    }
}

TEST_CASE("looping wraps back to the start", "[anim][frame][loop]") {
    const Clip walk{0, 4, 10.0f, true};
    // 4 frames * 0.1s = 0.4s per full cycle. At 0.40s we are back on frame 0.
    REQUIRE(frame_index(walk, 0.40f) == 0);
    REQUIRE(frame_index(walk, 0.55f) == 1);  // 0.4s wrap + 0.15s => frame 1
}

TEST_CASE("a one-shot clip holds on its last frame", "[anim][frame][oneshot]") {
    const Clip attack{0, 3, 10.0f, /*loop=*/false};
    REQUIRE(frame_index(attack, 0.20f) == 2);  // reached last frame
    REQUIRE(frame_index(attack, 5.00f) == 2);  // and stays there forever
}

TEST_CASE("frame_index is robust to bad input", "[anim][frame][edge]") {
    const Clip empty{0, 0, 10.0f, true};
    REQUIRE(frame_index(empty, 1.0f) == 0);       // no frames => frame 0, no crash
    const Clip walk{0, 4, 10.0f, true};
    REQUIRE(frame_index(walk, -1.0f) == 0);       // negative time clamps to 0
}

TEST_CASE("frame_to_rect walks the sheet grid", "[anim][sheet]") {
    // A sheet 4 columns wide, 32x32 frames.
    REQUIRE(frame_to_rect(0, 4, 32, 32) == IntRect{0, 0, 32, 32});
    REQUIRE(frame_to_rect(3, 4, 32, 32) == IntRect{96, 0, 32, 32});   // end of row 0
    REQUIRE(frame_to_rect(4, 4, 32, 32) == IntRect{0, 32, 32, 32});   // wraps to row 1
    REQUIRE(frame_to_rect(5, 4, 32, 32) == IntRect{32, 32, 32, 32});
}

TEST_CASE("Animator resets the timer only on a real state change", "[anim][controller]") {
    // idle: 1 frame at index 0; walk: 4 frames starting at sheet index 1.
    Animator anim(Clip{0, 1, 8.0f, true}, Clip{1, 4, 10.0f, true});

    REQUIRE(anim.state() == State::Idle);
    REQUIRE(anim.current_sheet_frame() == 0);

    SECTION("switching to Walk starts from the clip's first frame") {
        anim.set_state(State::Walk);
        REQUIRE(anim.current_frame_in_clip() == 0);
        REQUIRE(anim.current_sheet_frame() == 1);  // walk.first_frame + 0

        anim.update(0.25f);  // 0.25s at 10fps => 2 frames in
        REQUIRE(anim.current_frame_in_clip() == 2);
        REQUIRE(anim.current_sheet_frame() == 3);  // 1 + 2
    }

    SECTION("re-asserting the SAME state does NOT reset the timer") {
        anim.set_state(State::Walk);
        anim.update(0.25f);
        anim.set_state(State::Walk);  // same state again — must not rewind
        REQUIRE(anim.current_frame_in_clip() == 2);
    }

    SECTION("changing state DOES rewind") {
        anim.set_state(State::Walk);
        anim.update(0.25f);
        anim.set_state(State::Idle);
        anim.set_state(State::Walk);  // real change back to Walk
        REQUIRE(anim.current_frame_in_clip() == 0);
    }
}
