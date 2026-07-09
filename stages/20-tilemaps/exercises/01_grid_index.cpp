// Exercise 01 — a 2D grid stored as a flat 1D array (easy)
//
// A tilemap is a 2D grid, but we store it as ONE std::vector laid out row by
// row ("row-major"). To read cell (x, y) you compute a single index:
//
//     index = y * width + x
//
// Python comparison: instead of `grid[y][x]` on a list-of-lists, we keep one
// flat list and do the arithmetic ourselves. It's more cache-friendly and it's
// how the vertex array will walk the map later.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra 01_grid_index.cpp -o ex && ./ex
//
// TODO:
//   1. Implement `index(x, y, width)` to return the flat index (y*width + x).
//   2. In main, use it to WRITE a value into the flat vector, then READ it back.

#include <cassert>
#include <iostream>
#include <vector>

// TODO 1: return the row-major flat index of cell (x, y) in a grid `width` wide.
int index(int x, int y, int width) {
    return 0;  // <-- replace
}

int main() {
    const int width = 4;
    const int height = 3;
    std::vector<int> grid(static_cast<std::size_t>(width * height), 0);

    // TODO 2: set cell (2, 1) to 9 using index(), then read it back into `got`.
    // grid[index(2, 1, width)] = 9;
    int got = -1;  // <-- read grid[index(2,1,width)] here

    std::cout << "cell (2,1) = " << got << " (expected 9)\n";
    assert(got == 9);
    assert(index(0, 0, width) == 0);
    assert(index(3, 2, width) == 11);  // last cell of a 4x3 grid
    std::cout << "all checks passed\n";
    return 0;
}
