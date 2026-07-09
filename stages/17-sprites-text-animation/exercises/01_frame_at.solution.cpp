// Exercise 01 — solution  (PURE, no SFML)
#include <algorithm>  // std::max, std::min
#include <cassert>
#include <iostream>

int frame_index(int frame_count, float fps, float elapsed, bool loop) {
    if (frame_count <= 0) {
        return 0;  // nothing to show
    }
    const float t = std::max(0.0f, elapsed);           // clamp negative time
    const int frames_advanced = static_cast<int>(t * fps);
    if (loop) {
        return frames_advanced % frame_count;          // wrap around
    }
    return std::min(frames_advanced, frame_count - 1);  // hold last frame
}

int main() {
    assert(frame_index(4, 10.0f, 0.00f, true) == 0);
    assert(frame_index(4, 10.0f, 0.10f, true) == 1);
    assert(frame_index(4, 10.0f, 0.35f, true) == 3);
    assert(frame_index(4, 10.0f, 0.40f, true) == 0);

    assert(frame_index(3, 10.0f, 0.20f, false) == 2);
    assert(frame_index(3, 10.0f, 9.90f, false) == 2);

    std::cout << "01_frame_at: all assertions passed\n";
    return 0;
}
