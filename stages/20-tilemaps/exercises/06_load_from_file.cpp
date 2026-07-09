// Exercise 06 — round-trip a map through a real file (medium)
//
// Games ship maps as files. Here you WRITE a map to a temp file with an
// ofstream, then LOAD it back with Tilemap::from_file and confirm the parsed
// dimensions match. This reuses the file I/O from stage 10, and mirrors the
// "round-trip" testing idea from stage 13: what we save must be what we load.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/tilemap.cpp 06_load_from_file.cpp -o ex && ./ex
//
// TODO:
//   1. Write `contents` to the path `file_path` using an std::ofstream.
//   2. Load it back with Tilemap::from_file and return the resulting map.

#include <cassert>
#include <cstdio>  // std::remove
#include <fstream>
#include <iostream>
#include <string>

#include "game/tilemap.hpp"

using game::Tilemap;

Tilemap write_then_load(const std::string& file_path, const std::string& contents) {
    // TODO 1: open an ofstream on file_path and write `contents` to it. The
    //         stream closes itself (RAII) at the end of its scope.

    // TODO 2: load the file back and return the Tilemap.
    (void)file_path;
    (void)contents;
    return Tilemap{};  // <-- replace
}

int main() {
    const std::string path = "ex06_temp.map";
    const std::string contents = "5 4 16\n"
                                 "1 1 1 1 1\n"
                                 "1 0 0 0 1\n"
                                 "1 0 0 0 1\n"
                                 "1 1 1 1 1\n";

    const Tilemap map = write_then_load(path, contents);
    std::cout << "loaded " << map.width() << "x" << map.height()
              << " @ " << map.tile_size() << "px\n";
    assert(map.width() == 5);
    assert(map.height() == 4);
    assert(map.tile_size() == 16);
    assert(map.tile_at(1, 1) == 0);  // interior hole
    assert(map.tile_at(0, 0) == 1);  // corner wall

    std::remove(path.c_str());  // clean up the temp file
    std::cout << "all checks passed\n";
    return 0;
}
