// Exercise 05 — reject malformed maps with try/catch (medium-hard)
//
// Tilemap::from_text THROWS a std::runtime_error when the text is malformed
// (bad header, non-integer tile, wrong tile count). A robust loader shouldn't
// crash on a bad file — it should catch the error and report "invalid".
//
// Python comparison: this is `try: parse() except ValueError: ...`. C++
// exceptions work the same way; you name the exception TYPE in the `catch`.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/tilemap.cpp 05_reject_malformed.cpp -o ex && ./ex
//
// TODO:
//   1. Return true if from_text(text) succeeds, false if it throws
//      std::runtime_error. (Wrap the call in try/catch.)

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "game/tilemap.hpp"

using game::Tilemap;

bool is_valid_map(const std::string& text) {
    // TODO 1: try to build the map; on std::runtime_error return false.
    (void)text;
    return true;  // <-- replace
}

int main() {
    // Valid.
    assert(is_valid_map("2 2 16\n1 1\n1 1\n"));

    // Invalid: bad header, truncated body, non-integer tile.
    assert(!is_valid_map(""));
    assert(!is_valid_map("2 2 16\n1 1\n"));    // only 2 of 4 tiles
    assert(!is_valid_map("2 1 16\n1 nope\n"));  // non-integer id

    std::cout << "all checks passed\n";
    return 0;
}
