// Exercise 06 — solution  (PURE, no SFML)
#include <cassert>
#include <iostream>

struct Clip {
    int frame_count;
    float fps;
};

int frame_index(const Clip& clip, float elapsed) {
    if (clip.frame_count <= 0) {
        return 0;
    }
    const int frames_advanced = static_cast<int>(elapsed * clip.fps);
    return frames_advanced % clip.frame_count;
}

enum class State { Idle, Walk };

class Animator {
public:
    Animator(Clip idle, Clip walk) : idle_(idle), walk_(walk) {}

    void set_state(State s) {
        if (s != state_) {          // only a REAL change rewinds the timer
            state_ = s;
            elapsed_ = 0.0f;
        }
    }

    void update(float dt) { elapsed_ += dt; }

    int current_frame() const {
        const Clip& active = (state_ == State::Walk) ? walk_ : idle_;
        return frame_index(active, elapsed_);
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
    a.update(0.25f);
    assert(a.current_frame() == 2);

    a.set_state(State::Walk);  // same state — no rewind
    assert(a.current_frame() == 2);

    a.set_state(State::Idle);  // real change resets
    assert(a.current_frame() == 0);

    std::cout << "06_animator: all assertions passed\n";
    return 0;
}
