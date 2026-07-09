// Exercise 02 — sheet index -> pixel rectangle  (PURE, no SFML)
//
// A sprite sheet is one big image laid out as a grid of equal cells. To show
// frame N you tell the sprite which rectangle of the texture to display. That
// rectangle is pure grid arithmetic — no SFML needed to get it right.
//
//   clang++ -std=c++17 -Wall -Wextra 02_sheet_rects.cpp -o ex && ./ex
//
// TODO:
//   Fill in frame_to_rect(). The sheet has `columns` cells per row, each cell
//   `w` wide and `h` tall. For frame index `frame`:
//     column = frame % columns
//     row    = frame / columns   (integer division)
//     left   = column * w,  top = row * h,  width = w,  height = h

#include <cassert>
#include <iostream>

struct Rect {
    int left, top, width, height;
    bool operator==(const Rect& r) const {
        return left == r.left && top == r.top && width == r.width && height == r.height;
    }
};

Rect frame_to_rect(int frame, int columns, int w, int h) {
    // TODO: replace with the grid math above.
    (void)frame;
    (void)columns;
    return Rect{0, 0, w, h};
}

int main() {
    assert((frame_to_rect(0, 4, 32, 32) == Rect{0, 0, 32, 32}));
    assert((frame_to_rect(3, 4, 32, 32) == Rect{96, 0, 32, 32}));   // last of row 0
    assert((frame_to_rect(4, 4, 32, 32) == Rect{0, 32, 32, 32}));   // wraps to row 1
    assert((frame_to_rect(6, 4, 32, 32) == Rect{64, 32, 32, 32}));

    std::cout << "02_sheet_rects: all assertions passed\n";
    return 0;
}
