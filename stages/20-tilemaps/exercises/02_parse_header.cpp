// Exercise 02 — parse the map header line (easy-medium)
//
// Our map files start with one line of three ints: "W H tileSize". Parsing it
// is the first thing the loader does. std::istringstream turns a string into a
// stream you can read from with `>>`, exactly like reading from a file.
//
// KEY IDEA: `if (!(in >> a >> b >> c))` is true when ANY of the three reads
// failed — a token was missing or wasn't a number. That single check rejects
// both "header absent" and "header not numeric".
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 02_parse_header.cpp -o ex && ./ex
//
// TODO:
//   1. Read width, height, tile_size from `in`. Return false if the read fails
//      OR if any value is <= 0. Return true and fill the out-params otherwise.

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

bool parse_header(const std::string& line, int& width, int& height, int& tile_size) {
    std::istringstream in(line);
    // TODO 1: read the three ints; validate the stream and that all are > 0.
    (void)in;
    return false;  // <-- replace
}

int main() {
    int w = 0, h = 0, ts = 0;

    assert(parse_header("20 15 32", w, h, ts));
    assert(w == 20 && h == 15 && ts == 32);

    assert(!parse_header("", w, h, ts));            // empty
    assert(!parse_header("20 15", w, h, ts));       // missing tileSize
    assert(!parse_header("wide 15 32", w, h, ts));  // non-numeric
    assert(!parse_header("0 15 32", w, h, ts));     // non-positive

    std::cout << "all checks passed\n";
    return 0;
}
