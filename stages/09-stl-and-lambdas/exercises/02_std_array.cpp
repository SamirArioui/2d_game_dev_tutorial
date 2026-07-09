// Exercise 02 — std::array (easy–medium)
//
// std::array<T, N> is a FIXED-size array whose length N is baked in at compile
// time. Use it when the count never changes (the 6 faces of a die, the 4 corners
// of a rect). It lives on the stack — no heap allocation — so it's as cheap as a
// C array but with a real .size() and range-for support.
//
// Python has no true fixed-size list; the nearest idea is a tuple, but tuples can
// mix types. std::array is all one type, like a std::vector with a locked length.
//
// Fill in the TODOs. Expected output:
//   sum of rolls: 21
//   max roll: 6
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 02_std_array.cpp -o ex && ./ex

#include <algorithm>  // std::max_element
#include <array>
#include <iostream>

int main() {
    // Six dice faces. The size (6) is part of the type: std::array<int, 6>.
    std::array<int, 6> rolls = {1, 2, 3, 4, 5, 6};

    // TODO 1: sum the rolls with a range-for loop into `int total`, then print
    //         "sum of rolls: " << total

    // TODO 2: print "max roll: " followed by the largest value.
    //         std::max_element(rolls.begin(), rolls.end()) returns an ITERATOR
    //         (a pointer-like thing) to the max; dereference it with * to read
    //         the value: *std::max_element(...)

    return 0;
}
