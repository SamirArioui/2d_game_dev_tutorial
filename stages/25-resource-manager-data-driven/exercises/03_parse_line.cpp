// Exercise 03 — Parse one config line (medium)
//
// Data-driven design starts with reading data. Parse a single "entity" directive of the form
//     entity <x> <y> <texture>
// into a struct. std::istringstream turns a line into a little stream you can read fields from
// (this is stage 10's file I/O, applied to a string).
//
// TODO: implement parse_entity so it reads the x, y, and texture out of `line`.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_parse_line.cpp -o ex && ./ex

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

struct EntitySpec {
    std::string texture;
    float x{0.0f};
    float y{0.0f};
};

EntitySpec parse_entity(const std::string& line) {
    EntitySpec spec;
    std::istringstream tokens(line);

    std::string key;
    tokens >> key;  // consume the leading "entity" keyword
    // TODO: read x, then y, then texture from `tokens` into `spec`.

    return spec;
}

int main() {
    const EntitySpec spec = parse_entity("entity 100 200 tiles/grass.png");
    assert(spec.x == 100.0f);
    assert(spec.y == 200.0f);
    assert(spec.texture == "tiles/grass.png");

    std::cout << "parsed: " << spec.texture << " at (" << spec.x << ", " << spec.y << ")\n";
    return 0;
}
