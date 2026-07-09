// Exercise 01 — AABB overlap test  (PURE, no SFML)
//
// Pong's ball-vs-paddle check is just "do two axis-aligned boxes overlap?".
// Two boxes overlap when they overlap on BOTH axes at once. On one axis, ranges
// [aLo, aHi] and [bLo, bHi] overlap when aLo < bHi AND bLo < aHi.
//
//   clang++ -std=c++17 -Wall -Wextra 01_aabb_overlap.cpp -o ex && ./ex
//
// TODO:
//   Implement overlaps(). A box is (x, y) top-left plus (w, h) size, so its
//   x-range is [x, x+w] and y-range is [y, y+h]. Return true only if the boxes
//   overlap on BOTH axes. Boxes that merely touch (share an edge) do NOT count.

#include <cassert>
#include <iostream>

bool overlaps(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
    // TODO: return true iff the two boxes overlap on both x and y.
    (void)ax; (void)ay; (void)aw; (void)ah;
    (void)bx; (void)by; (void)bw; (void)bh;
    return false;
}

int main() {
    // clearly overlapping
    assert(overlaps(0, 0, 10, 10, 5, 5, 10, 10));
    // clearly apart
    assert(!overlaps(0, 0, 10, 10, 20, 20, 10, 10));
    // overlap on x only (stacked far apart on y) -> NOT a collision
    assert(!overlaps(0, 0, 10, 10, 2, 50, 10, 10));
    // just touching on the right edge -> NOT a collision
    assert(!overlaps(0, 0, 10, 10, 10, 0, 10, 10));

    std::cout << "01_aabb_overlap: all assertions passed\n";
    return 0;
}
