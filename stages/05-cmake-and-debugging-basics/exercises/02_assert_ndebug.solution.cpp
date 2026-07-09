// Exercise 02 — assert() and NDEBUG (solution)
//
// In a Debug build the assert fires on get(scores, 5) and aborts with a message like:
//   Assertion failed: (index >= 0 && index < static_cast<int>(data.size())), function get, ...
// In a Release build (-DNDEBUG) the assert is compiled out and the out-of-bounds read happens.
#include <array>
#include <cassert>
#include <iostream>

int get(const std::array<int, 3>& data, int index) {
    // Catches the programming error early and documents the precondition. Costs nothing in a
    // Release build because NDEBUG removes it.
    assert(index >= 0 && index < static_cast<int>(data.size()));
    return data[index];
}

int main() {
    const std::array<int, 3> scores = {10, 20, 30};
    std::cout << "scores[1] = " << get(scores, 1) << "\n";
    std::cout << "scores[5] = " << get(scores, 5) << "   <-- out of bounds!\n";
    return 0;
}
