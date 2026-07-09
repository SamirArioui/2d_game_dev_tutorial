// Exercise 02 — std::array (solution)
#include <algorithm>
#include <array>
#include <iostream>

int main() {
    std::array<int, 6> rolls = {1, 2, 3, 4, 5, 6};

    int total = 0;
    for (int r : rolls) {  // int is cheap to copy, so no const& needed here
        total += r;
    }
    std::cout << "sum of rolls: " << total << "\n";

    // Algorithms work on any [begin, end) iterator range: array, vector, ...
    // max_element returns an iterator to the biggest element; * reads it.
    std::cout << "max roll: " << *std::max_element(rolls.begin(), rolls.end()) << "\n";

    return 0;
}
