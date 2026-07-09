// Exercise 02 — a rolling FPS average  (PURE, no SFML)
//
// A raw per-frame FPS number flickers wildly (one slow frame and it plunges).
// The overlay shows a ROLLING AVERAGE over the last N frames instead. Keep a
// running sum so each update is O(1): add the new sample, subtract the one it
// evicted.
//
//   clang++ -std=c++17 -Wall -Wextra 02_frame_profiler.cpp -o ex && ./ex
//
// TODO:
//   Implement add_sample() and average().
//     - keep a fixed-size ring buffer `samples_` of size `window_`.
//     - maintain `sum_` so it always equals the sum of the STORED samples.
//     - average() = sum_ / count_ (0 when empty). fps() = 1/average.

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

class FrameProfiler {
public:
    explicit FrameProfiler(int window) : window_(window), samples_(window, 0.0f) {}

    void add_sample(float dt) {
        // TODO: append (or overwrite oldest), keep sum_ and count_ correct,
        // advance next_ around the ring.
        (void)dt;
    }

    float average() const {
        // TODO: sum_ / count_, or 0 when there are no samples.
        return 0.0f;
    }

    float fps() const {
        const float a = average();
        return a > 0.0f ? 1.0f / a : 0.0f;
    }

    int count() const { return count_; }

private:
    int window_;
    int count_ = 0;
    int next_ = 0;
    float sum_ = 0.0f;
    std::vector<float> samples_;
};

int main() {
    FrameProfiler p(4);
    assert(p.count() == 0);
    assert(std::abs(p.average()) < 1e-6f);

    for (int i = 0; i < 10; ++i) {
        p.add_sample(1.0f / 60.0f);  // steady 60 fps
    }
    assert(p.count() == 4);  // window caps the stored count
    assert(std::abs(p.average() - 1.0f / 60.0f) < 1e-5f);
    assert(std::abs(p.fps() - 60.0f) < 1e-2f);

    std::cout << "02_frame_profiler: all assertions passed\n";
    return 0;
}
