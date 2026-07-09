// Exercise 01 — solution  (PURE, no SFML)
#include <cassert>
#include <iostream>

bool overlaps(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh) {
    // Overlap on X: a starts before b ends AND b starts before a ends.
    const bool x_overlap = ax < bx + bw && bx < ax + aw;
    const bool y_overlap = ay < by + bh && by < ay + ah;
    // A real overlap needs BOTH axes. Strict '<' makes touching edges NOT count.
    return x_overlap && y_overlap;
}

int main() {
    assert(overlaps(0, 0, 10, 10, 5, 5, 10, 10));
    assert(!overlaps(0, 0, 10, 10, 20, 20, 10, 10));
    assert(!overlaps(0, 0, 10, 10, 2, 50, 10, 10));
    assert(!overlaps(0, 0, 10, 10, 10, 0, 10, 10));

    std::cout << "01_aabb_overlap: all assertions passed\n";
    return 0;
}
