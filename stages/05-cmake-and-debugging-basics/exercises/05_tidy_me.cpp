// Exercise 05 — Read a clang-tidy warning and fix it
//
// clang-tidy is a linter / static analyzer — the C++ cousin of flake8 / pylint. It flags
// bug-prone or non-idiomatic code that compiles perfectly well. It uses the repo's .clang-tidy
// config (found by searching upward from the file). Run it:
//   clang-tidy 05_tidy_me.cpp -- -std=c++17
//
// The `--` separates clang-tidy's own options from the COMPILER flags it needs to parse the file
// (here just -std=c++17). Each warning names the check that fired in [brackets], e.g.
// [modernize-use-nullptr]. Read the name, understand it, fix the code.
//
// TODO: fix every warning until clang-tidy prints no diagnostics. Compare with
//       05_tidy_me.solution.cpp. What it will flag here:
//         - an uninitialized local variable   [cppcoreguidelines-init-variables]
//         - the literal 0 used as a pointer    [modernize-use-nullptr]
//         - a C-style cast (double)            [modernize-avoid-c-style-cast]

#include <iostream>

int main() {
    int score;                          // declared but not initialized
    score = 100;

    int* checkpoint = 0;                // 0 used as a pointer value

    double bonus = (double)score / 3;   // C-style cast

    if (checkpoint == 0) {              // 0 used as a pointer again
        std::cout << "score=" << score << " bonus=" << bonus << " (no checkpoint)\n";
    }
    return 0;
}
