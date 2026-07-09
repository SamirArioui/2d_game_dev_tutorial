#pragma once

// ===========================================================================
// DATA-DRIVEN DESIGN — describe the level in a text file, not in C++.
//
// Hardcoding "spawn a wall at (300,150)" means recompiling to move a wall. Instead we read a plain
// config file at run time into these structs, and the game builds itself from the data. Designers
// (or you, next week) can tweak the level with a text editor and no rebuild. This is the same idea
// as loading a JSON/YAML config in Python — here it's a tiny whitespace-delimited format so the
// parser stays teachable (it reuses the file I/O from stage 10).
//
// The parser is pure and works on any std::istream, so tests feed it an in-memory string — no
// files, no SFML.
// ===========================================================================

#include <istream>
#include <string>
#include <vector>

namespace game {

// One thing to place in the level: which texture, and where.
struct EntitySpec {
    std::string texture;
    float x{0.0f};
    float y{0.0f};
};

// The whole level, as data.
struct LevelConfig {
    std::string title;
    int width{0};
    int height{0};
    std::vector<EntitySpec> entities;
};

// Parse a level from any input stream. Format (one directive per line; '#' begins a comment;
// blank lines ignored):
//     title  My Level
//     size   640 480
//     entity 100 200 tiles/grass.png
// Unknown directives are skipped so the format can grow without breaking old files.
LevelConfig parse_level(std::istream& in);

// Convenience: open a file and parse it. Throws std::runtime_error if the file can't be opened.
LevelConfig load_level(const std::string& path);

}  // namespace game
