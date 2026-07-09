// Exercise 03 — nullptr and choosing pointer vs reference (medium)
//
// Two rules of thumb for "pointer or reference?":
//   * Use a REFERENCE when the thing must always exist (a required argument you will use).
//   * Use a POINTER when "no object" is a valid answer — a pointer can be `nullptr`, a
//     reference cannot. And ALWAYS check a pointer for null before dereferencing it.
//
// PYTHON NOTE: Python uses `None` for "no value" and you test `if x is None`. C++'s `nullptr`
// is the pointer equivalent; `if (p != nullptr)` is the `is not None` check.
//
// Below is a roster of hp values. Implement `find_first_dead`, which returns a POINTER to the
// first entry whose hp is 0 (so the caller can revive it), or `nullptr` if everyone is alive.
// Returning a pointer lets us express "nobody is dead" as nullptr — a reference couldn't.
//
// TODO:
//   1. Finish find_first_dead so it returns &hps[i] for the first 0, else nullptr.
//   2. In main, call it; if the result is non-null, revive through the pointer (set it to 10);
//      otherwise print "party is healthy".
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_null_and_safety.cpp -o ex && ./ex

#include <iostream>

// Returns a pointer to the first zero in the array, or nullptr if there is none.
int* find_first_dead(int hps[], int count) {
    // TODO: loop; return &hps[i] when hps[i] == 0; return nullptr after the loop.
    return nullptr;
}

int main() {
    int hps[] = {12, 30, 0, 7};
    int count = 4;

    // TODO: call find_first_dead, null-check the result, revive or report.
    return 0;
}
