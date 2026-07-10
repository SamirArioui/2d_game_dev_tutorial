// Stage 25 mini-project — the data-driven level parser (implementation) — YOUR TASK.
//
// This is the STARTER: parse_level() is stubbed with a TODO and a placeholder return so the
// project compiles and links, but tests/test_level_config.cpp FAILS until you implement it. The
// declarations live in level_config.hpp; the `trim` helper and the file-opening load_level()
// wrapper are provided for you. A complete reference is in ../solution/src/level_config.cpp.
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
    // TODO(stage 25): read `in` line by line and build a LevelConfig. For each line:
    //   1. Drop a trailing comment: erase everything from the first '#' to end-of-line.
    //   2. Read the first whitespace-delimited token as the directive `key` (an istringstream over
    //      the line is handy); a line with no token (blank / comment-only) is skipped.
    //   3. Dispatch on `key`:
    //        "title"  -> the REST of the line is the title (may contain spaces); trim() it.
    //        "size"   -> read two ints into config.width and config.height.
    //        "entity" -> read x, y (floats) then a texture path into an EntitySpec; append it.
    //      Any other key is ignored on purpose (forward-compatible with new directives).
    return {};  // placeholder
}

LevelConfig load_level(const std::string& path) {
    std::ifstream file(path);  // RAII: closes itself when this function returns (stage 10)
    if (!file.is_open()) {
        throw std::runtime_error("load_level: cannot open '" + path + "'");
    }
    return parse_level(file);
}

}  // namespace game
