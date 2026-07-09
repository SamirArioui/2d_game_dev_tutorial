// Exercise 04 — Parse a whole config stream (solution)
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
        const auto hash = line.find('#');
        if (hash != std::string::npos) {
            line.erase(hash);
        }

        std::istringstream tokens(line);
        std::string key;
        if (!(tokens >> key)) {
            continue;
        }

        if (key == "size") {
            tokens >> config.width >> config.height;
        } else if (key == "entity") {
            EntitySpec spec;
            tokens >> spec.x >> spec.y >> spec.texture;
            config.entities.push_back(spec);
        }
    }
    return config;
}

int main() {
    std::istringstream in(
        "# demo\n"
        "size 320 240\n"
        "\n"
        "entity 10 20 grass.png   # ground\n"
        "gravity 9.8\n"
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
