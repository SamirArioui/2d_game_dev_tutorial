#pragma once

// ============================================================================
// A tiny, SFML-free animation core.
//
// The whole lesson of this stage in one design rule: the MATH of "which frame
// of the sprite sheet is showing right now" has nothing to do with SFML. So we
// put it here, in plain functions and a small controller, and unit-test it
// without ever opening a window. `main.cpp` then just reads a frame index and
// tells the sprite to show that rectangle of the texture.
//
// Python note: no direct analogue to sprite sheets, but the split mirrors good
// Python practice — keep the pure "given elapsed time, what frame?" logic out
// of the pygame/rendering layer so you can test it with plain asserts.
// ============================================================================

namespace anim {

// A plain pixel rectangle — the SFML-free twin of sf::IntRect. The renderer
// converts this into an sf::IntRect when it sets the sprite's texture rect.
struct IntRect {
    int left = 0;
    int top = 0;
    int width = 0;
    int height = 0;

    bool operator==(const IntRect& r) const {
        return left == r.left && top == r.top && width == r.width && height == r.height;
    }
};

// A clip is a contiguous run of frames on the sheet, played at `fps`.
// Example: a 4-frame walk starting at sheet index 4 -> {first_frame:4,
// frame_count:4, fps:8, loop:true}.
struct Clip {
    int first_frame = 0;  // absolute index of the clip's first frame on the sheet
    int frame_count = 1;  // number of frames in the clip
    float fps = 8.0f;     // playback speed (frames per second)
    bool loop = true;     // true: wrap around; false: hold on the last frame
};

// Which frame WITHIN the clip (0 .. frame_count-1) shows after `elapsed`
// seconds of playback. Pure: same inputs always give the same answer, which is
// exactly what makes it trivial to unit-test.
int frame_index(const Clip& clip, float elapsed);

// Map a sheet frame index to its pixel rectangle. The sheet is a grid of
// `columns` frames per row, each frame `frame_w` x `frame_h`. Pure arithmetic.
IntRect frame_to_rect(int frame, int columns, int frame_w, int frame_h);

// The states our walk-cycle character can be in. `enum class` (stage 03) keeps
// these scoped and un-convertible to int by accident.
enum class State { Idle, Walk };

// A minimal animation controller: one clip per state, the active state, and a
// playback timer. This is the "animation controller" the stage asks for —
// state in, texture-rect-worthy frame out, no rendering knowledge inside.
class Animator {
public:
    Animator(Clip idle, Clip walk);

    // Switch state. Resets the timer ONLY when the state actually changes, so
    // re-asserting the same state every frame does not freeze the animation on
    // frame 0 (a classic beginner bug).
    void set_state(State s);
    State state() const { return state_; }

    // Advance playback by `dt` seconds (dt is always seconds, as a float).
    void update(float dt);

    // Frame 0..count-1 of the ACTIVE clip.
    int current_frame_in_clip() const;

    // Absolute frame index on the sheet (clip.first_frame + frame in clip).
    int current_sheet_frame() const;

private:
    const Clip& active_clip() const;

    Clip idle_;
    Clip walk_;
    State state_ = State::Idle;
    float elapsed_ = 0.0f;  // seconds spent in the current state
};

}  // namespace anim
