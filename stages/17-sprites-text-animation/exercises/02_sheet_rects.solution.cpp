// Exercise 02 — solution  (PURE, no SFML)
#include <cassert>
#include <iostream>

struct Rect {
    int left, top, width, height;
    bool operator==(const Rect& r) const {
        return left == r.left && top == r.top && width == r.width && height == r.height;
    }
};

Rect frame_to_rect(int frame, int columns, int w, int h) {
    const int column = frame % columns;
    const int row = frame / columns;  // integer division floors toward the row
    return Rect{column * w, row * h, w, h};
}

int main() {
    assert((frame_to_rect(0, 4, 32, 32) == Rect{0, 0, 32, 32}));
    assert((frame_to_rect(3, 4, 32, 32) == Rect{96, 0, 32, 32}));
    assert((frame_to_rect(4, 4, 32, 32) == Rect{0, 32, 32, 32}));
    assert((frame_to_rect(6, 4, 32, 32) == Rect{64, 32, 32, 32}));

    std::cout << "02_sheet_rects: all assertions passed\n";
    return 0;
}
