// Exercise 05 — solution
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "game/tilemap.hpp"

using game::Tilemap;

bool is_valid_map(const std::string& text) {
    try {
        Tilemap::from_text(text);
        return true;
    } catch (const std::runtime_error&) {
        return false;
    }
}

int main() {
    assert(is_valid_map("2 2 16\n1 1\n1 1\n"));

    assert(!is_valid_map(""));
    assert(!is_valid_map("2 2 16\n1 1\n"));
    assert(!is_valid_map("2 1 16\n1 nope\n"));

    std::cout << "all checks passed\n";
    return 0;
}
