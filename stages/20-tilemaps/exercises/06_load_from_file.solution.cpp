// Exercise 06 — solution
#include <cassert>
#include <cstdio>  // std::remove
#include <fstream>
#include <iostream>
#include <string>

#include "game/tilemap.hpp"

using game::Tilemap;

Tilemap write_then_load(const std::string& file_path, const std::string& contents) {
    {
        std::ofstream out(file_path);
        out << contents;
    }  // out closes here (RAII) before we read the file back
    return Tilemap::from_file(file_path);
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
    assert(map.tile_at(1, 1) == 0);
    assert(map.tile_at(0, 0) == 1);

    std::remove(path.c_str());
    std::cout << "all checks passed\n";
    return 0;
}
