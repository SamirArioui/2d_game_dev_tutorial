// Exercise 06 — a tiny animation controller  (PURE, no SFML)
//
// Ties 01 (frame math) together into the "controller" pattern the mini-project
// uses: hold a clip per state, a current state, and a timer. The subtle rule:
// switching to the SAME state you're already in must NOT reset the timer, or
// holding a movement key freezes the character on frame 0 forever.
//
//   clang++ -std=c++17 -Wall -Wextra 06_animator.cpp -o ex && ./ex
//
// TODO:
//   1. frame_index(): same looping logic as exercise 01.
//   2. Animator::set_state(): store the new state and reset elapsed_ to 0 ONLY
//      when the state actually changed.
//   3. Animator::current_frame(): frame_index of the active clip at elapsed_.

#include <cassert>
#include <iostream>

struct Clip {
    int frame_count;
    float fps;
};

int frame_index(const Clip& clip, float elapsed) {
    // TODO: return the looping frame index (int(elapsed*fps) % frame_count).
    (void)clip;
    (void)elapsed;
    return 0;
}

enum class State { Idle, Walk };

class Animator {
public:
    Animator(Clip idle, Clip walk) : idle_(idle), walk_(walk) {}

    void set_state(State s) {
        // TODO: only reset elapsed_ when s differs from the current state_.
        state_ = s;
    }

    void update(float dt) { elapsed_ += dt; }

    int current_frame() const {
        // TODO: use frame_index on whichever clip matches state_.
        return 0;
    }

private:
    Clip idle_;
    Clip walk_;
    State state_ = State::Idle;
    float elapsed_ = 0.0f;
};

int main() {
    Animator a(Clip{1, 8.0f}, Clip{4, 10.0f});

    a.set_state(State::Walk);
    a.update(0.25f);  // 0.25 * 10fps = 2 frames in
    assert(a.current_frame() == 2);

    a.set_state(State::Walk);  // same state — must NOT rewind
    assert(a.current_frame() == 2);

    a.set_state(State::Idle);  // real change resets the timer
    assert(a.current_frame() == 0);

    std::cout << "06_animator: all assertions passed\n";
    return 0;
}
