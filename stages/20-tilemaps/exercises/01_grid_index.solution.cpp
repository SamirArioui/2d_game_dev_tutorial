// Exercise 01 — solution
#include <cassert>
#include <iostream>
#include <vector>

int index(int x, int y, int width) {
    return y * width + x;
}

int main() {
    const int width = 4;
    const int height = 3;
    std::vector<int> grid(static_cast<std::size_t>(width * height), 0);

    grid[static_cast<std::size_t>(index(2, 1, width))] = 9;
    int got = grid[static_cast<std::size_t>(index(2, 1, width))];

    std::cout << "cell (2,1) = " << got << " (expected 9)\n";
    assert(got == 9);
    assert(index(0, 0, width) == 0);
    assert(index(3, 2, width) == 11);
    std::cout << "all checks passed\n";
    return 0;
}
