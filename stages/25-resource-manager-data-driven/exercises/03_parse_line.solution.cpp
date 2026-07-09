// Exercise 03 — Parse one config line (solution)
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
    tokens >> key;  // "entity"
    tokens >> spec.x >> spec.y >> spec.texture;

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
