// Exercise 02 — assert() and NDEBUG (Debug vs Release)
//
// assert(cond) aborts the program with a message if cond is false — but ONLY when the macro
// NDEBUG is NOT defined. A Release build defines NDEBUG, which compiles every assert() out to
// nothing. So asserts are free self-checks during development that vanish in the shipping build.
//
// CMake wires this up for you: -DCMAKE_BUILD_TYPE=Debug leaves asserts ON; Release adds -DNDEBUG.
// Here we do it by hand with clang++ so you can see the mechanism directly.
//
// Python note: Python also has `assert`, but it's disabled at RUN time by the -O flag. C++ decides
// at COMPILE time via the NDEBUG macro — a Release binary has no asserts compiled into it at all.
//
// TODO: add  assert(index >= 0 && index < static_cast<int>(data.size()));  at the top of get(),
//       so a bad index is caught in Debug builds instead of silently reading out of bounds.
//
// Build & run BOTH ways and compare:
//   # Debug: assert is active -> aborts on the out-of-range index below.
//   clang++ -std=c++17 -Wall -Wextra -g 02_assert_ndebug.cpp -o ex_debug && ./ex_debug
//
//   # Release: -DNDEBUG compiles the assert away -> no abort; it reads out of bounds (undefined
//   # behavior: it may print garbage, or crash — the very bug the assert was protecting you from).
//   clang++ -std=c++17 -Wall -Wextra -O2 -DNDEBUG 02_assert_ndebug.cpp -o ex_release && ./ex_release

#include <array>
#include <cassert>
#include <iostream>

int get(const std::array<int, 3>& data, int index) {
    // TODO: assert the index is in range [0, data.size()) before using it.
    return data[index];   // std::array::operator[] does NOT bounds-check (that's why we assert)
}

int main() {
    const std::array<int, 3> scores = {10, 20, 30};
    std::cout << "scores[1] = " << get(scores, 1) << "\n";
    std::cout << "scores[5] = " << get(scores, 5) << "   <-- out of bounds!\n";
    return 0;
}
