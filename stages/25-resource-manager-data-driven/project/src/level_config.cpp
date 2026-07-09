#include "game/level_config.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace game {

namespace {

// Trim leading/trailing whitespace — used so a multi-word `title` reads cleanly.
std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

}  // namespace

LevelConfig parse_level(std::istream& in) {
    LevelConfig config;
    std::string line;

    while (std::getline(in, line)) {
        // Everything from a '#' to end-of-line is a comment — drop it.
        const auto hash = line.find('#');
        if (hash != std::string::npos) {
            line.erase(hash);
        }

        std::istringstream tokens(line);
        std::string key;
        if (!(tokens >> key)) {
            continue;  // blank line (or a line that was only a comment)
        }

        if (key == "title") {
            std::string rest;
            std::getline(tokens, rest);  // the remainder of the line, so titles may have spaces
            config.title = trim(rest);
        } else if (key == "size") {
            tokens >> config.width >> config.height;
        } else if (key == "entity") {
            EntitySpec spec;
            tokens >> spec.x >> spec.y >> spec.texture;
            config.entities.push_back(spec);
        }
        // Any other key is ignored on purpose — forward-compatible with new directives.
    }

    return config;
}

LevelConfig load_level(const std::string& path) {
    std::ifstream file(path);  // RAII: closes itself when this function returns (stage 10)
    if (!file.is_open()) {
        throw std::runtime_error("load_level: cannot open '" + path + "'");
    }
    return parse_level(file);
}

}  // namespace game
