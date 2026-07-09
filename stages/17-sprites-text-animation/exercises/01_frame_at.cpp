// Exercise 01 — frame index from elapsed time  (PURE, no SFML)
//
// The heart of frame animation is one small function: given how long a clip has
// been playing, which frame is on screen right now? It is pure arithmetic, so
// we can write and test it with zero graphics. Build & run it directly:
//
//   clang++ -std=c++17 -Wall -Wextra 01_frame_at.cpp -o ex && ./ex
//
// TODO:
//   Implement frame_index() so that, for a clip of `frame_count` frames playing
//   at `fps` frames per second after `elapsed` seconds:
//     1. Compute how many whole frames have played:  int(elapsed * fps).
//     2. If `loop` is true, wrap with % frame_count.
//     3. If `loop` is false, hold on the last frame (frame_count - 1).
//   The asserts in main() must all pass.

#include <cassert>
#include <iostream>

int frame_index(int frame_count, float fps, float elapsed, bool loop) {
    // TODO: replace this stub with the real logic described above.
    (void)fps;
    (void)elapsed;
    (void)loop;
    (void)frame_count;
    return 0;
}

int main() {
    // 4 frames at 10 fps => 0.1s per frame.
    assert(frame_index(4, 10.0f, 0.00f, true) == 0);
    assert(frame_index(4, 10.0f, 0.10f, true) == 1);
    assert(frame_index(4, 10.0f, 0.35f, true) == 3);
    assert(frame_index(4, 10.0f, 0.40f, true) == 0);  // loop wraps

    // one-shot holds on the last frame
    assert(frame_index(3, 10.0f, 0.20f, false) == 2);
    assert(frame_index(3, 10.0f, 9.90f, false) == 2);

    std::cout << "01_frame_at: all assertions passed\n";
    return 0;
}
