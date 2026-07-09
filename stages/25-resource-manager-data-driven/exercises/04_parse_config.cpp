// Exercise 04 — Parse a whole config stream (medium-hard)
//
// Now the full loop: read a multi-line config, one directive per line, ignoring comments and
// blank lines. Build up a LevelConfig as you go.
//
// TODO: complete the parse loop:
//   - strip a trailing "# comment" from each line;
//   - read the first token as `key`; skip the line if there's no token;
//   - handle "size <w> <h>" and "entity <x> <y> <texture>"; ignore anything else.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_parse_config.cpp -o ex && ./ex

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct EntitySpec {
    std::string texture;
    float x{0.0f};
    float y{0.0f};
};
struct LevelConfig {
    int width{0};
    int height{0};
    std::vector<EntitySpec> entities;
};

LevelConfig parse(std::istream& in) {
    LevelConfig config;
    std::string line;
    while (std::getline(in, line)) {
        // TODO:
        //   1. if line contains '#', erase from there to the end (strip the comment);
        //   2. make an istringstream over the line and read `key`; `continue` if none;
        //   3. if key == "size", read width and height;
        //   4. if key == "entity", read x, y, texture and push_back an EntitySpec.
        (void)line;
    }
    return config;
}

int main() {
    std::istringstream in(
        "# demo\n"
        "size 320 240\n"
        "\n"
        "entity 10 20 grass.png   # ground\n"
        "gravity 9.8\n"  // unknown directive -> ignored
        "entity 40 20 wall.png\n");

    const LevelConfig level = parse(in);
    assert(level.width == 320);
    assert(level.height == 240);
    assert(level.entities.size() == 2);
    assert(level.entities[0].texture == "grass.png");
    assert(level.entities[1].x == 40.0f);

    std::cout << "parsed " << level.entities.size() << " entities\n";
    return 0;
}
