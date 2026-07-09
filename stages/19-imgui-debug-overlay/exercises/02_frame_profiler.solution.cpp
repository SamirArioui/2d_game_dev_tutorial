// Exercise 02 — solution  (PURE, no SFML)
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

class FrameProfiler {
public:
    explicit FrameProfiler(int window) : window_(window), samples_(window, 0.0f) {}

    void add_sample(float dt) {
        if (count_ < window_) {
            sum_ += dt;
            samples_[next_] = dt;
            ++count_;
        } else {
            sum_ -= samples_[next_];  // evict the oldest from the running sum
            sum_ += dt;
            samples_[next_] = dt;
        }
        next_ = (next_ + 1) % window_;
    }

    float average() const { return count_ == 0 ? 0.0f : sum_ / static_cast<float>(count_); }

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
        p.add_sample(1.0f / 60.0f);
    }
    assert(p.count() == 4);
    assert(std::abs(p.average() - 1.0f / 60.0f) < 1e-5f);
    assert(std::abs(p.fps() - 60.0f) < 1e-2f);

    std::cout << "02_frame_profiler: all assertions passed\n";
    return 0;
}
