// Exercise 05 — Read a clang-tidy warning and fix it (solution)
//
// Three fixes, one per warning:
//   [cppcoreguidelines-init-variables] initialize `score` where it's declared.
//   [modernize-use-nullptr]            use nullptr, not 0, for pointer values.
//   [modernize-avoid-c-style-cast]     use static_cast<double>(...) instead of (double)...
// After these, `clang-tidy 05_tidy_me.solution.cpp -- -std=c++17` reports nothing.
#include <iostream>

int main() {
    int score = 100;

    int* checkpoint = nullptr;

    double bonus = static_cast<double>(score) / 3;

    if (checkpoint == nullptr) {
        std::cout << "score=" << score << " bonus=" << bonus << " (no checkpoint)\n";
    }
    return 0;
}
