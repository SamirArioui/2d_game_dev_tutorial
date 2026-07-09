// Exercise 06 — Random dice roller (solution)
#include <array>
#include <iostream>
#include <random>

int main() {
    // Fixed seed => reproducible sequence (great for tutorials and tests).
    // Real games use std::random_device{}() to seed unpredictably.
    std::mt19937 rng(12345);

    // The distribution maps the engine's raw bits onto a fair 1..6 range.
    std::uniform_int_distribution<int> die(1, 6);

    const int rolls = 20;
    std::array<int, 6> counts{};  // the {} value-initialises all six ints to 0

    for (int i = 0; i < rolls; ++i) {
        auto face = die(rng);     // 'auto' deduces int here; face is in [1, 6]
        counts[face - 1] += 1;    // face 1 -> index 0, ..., face 6 -> index 5
    }

    std::cout << "Rolled a d6 " << rolls << " times:\n";
    for (int face = 1; face <= 6; ++face) {
        int count = counts[face - 1];
        // static_cast<double> avoids integer division so the percentage keeps its fraction.
        double pct = static_cast<double>(count) / rolls * 100.0;
        std::cout << " " << face << ": " << count << "  (" << pct << "%)\n";
    }

    return 0;
}
