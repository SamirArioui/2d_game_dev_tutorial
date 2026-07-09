// Exercise 02 — solution
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

bool parse_header(const std::string& line, int& width, int& height, int& tile_size) {
    std::istringstream in(line);
    if (!(in >> width >> height >> tile_size)) {
        return false;  // missing or non-numeric token
    }
    if (width <= 0 || height <= 0 || tile_size <= 0) {
        return false;  // dimensions must be positive
    }
    return true;
}

int main() {
    int w = 0, h = 0, ts = 0;

    assert(parse_header("20 15 32", w, h, ts));
    assert(w == 20 && h == 15 && ts == 32);

    assert(!parse_header("", w, h, ts));
    assert(!parse_header("20 15", w, h, ts));
    assert(!parse_header("wide 15 32", w, h, ts));
    assert(!parse_header("0 15 32", w, h, ts));

    std::cout << "all checks passed\n";
    return 0;
}
