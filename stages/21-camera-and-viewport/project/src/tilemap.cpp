// ============================================================================
// Tilemap implementation — pure parsing + queries, no SFML.
// ============================================================================

#include "game/tilemap.hpp"

#include <cmath>      // std::floor
#include <fstream>    // std::ifstream (file loading)
#include <sstream>    // std::istringstream (tokenising the text)
#include <stdexcept>  // std::runtime_error

namespace game {

Tilemap Tilemap::from_text(const std::string& text) {
    std::istringstream in(text);

    Tilemap map;

    // --- header line: three ints "W H tileSize" -----------------------------
    // Reading into ints with `>>` fails (sets the stream's failbit) if a token
    // is missing or isn't a number, so one `if (!(in >> ...))` catches "header
    // absent" and "header not numeric" together.
    if (!(in >> map.width_ >> map.height_ >> map.tile_size_)) {
        throw std::runtime_error("tilemap: missing or non-numeric header (want 'W H tileSize')");
    }
    if (map.width_ <= 0 || map.height_ <= 0 || map.tile_size_ <= 0) {
        throw std::runtime_error("tilemap: width, height and tileSize must all be positive");
    }

    // --- body: exactly width*height tile ids --------------------------------
    // `>>` skips whitespace (including newlines) automatically, so we don't
    // care how the rows are wrapped — we just need the right COUNT of ints.
    const std::size_t expected = static_cast<std::size_t>(map.width_) *
                                 static_cast<std::size_t>(map.height_);
    map.tiles_.reserve(expected);

    int value = 0;
    while (in >> value) {
        map.tiles_.push_back(value);
    }

    // Too few tiles (file truncated) OR too many (junk trailing data) is an
    // error — a malformed map should fail loudly, not silently half-load.
    if (map.tiles_.size() != expected) {
        throw std::runtime_error("tilemap: expected " + std::to_string(expected) +
                                 " tile ids but read " + std::to_string(map.tiles_.size()));
    }

    return map;
}

Tilemap Tilemap::from_file(const std::string& path) {
    // RAII: the ifstream opens here and closes itself when it goes out of scope
    // (stage 10). No manual close, even on the throw path below.
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("tilemap: cannot open file '" + path + "'");
    }

    // Slurp the whole file into a string, then reuse the tested text parser.
    std::stringstream buffer;
    buffer << file.rdbuf();
    return from_text(buffer.str());
}

int Tilemap::tile_at(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return 0;  // off the map reads as empty
    }
    return tiles_[static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) +
                  static_cast<std::size_t>(x)];
}

bool Tilemap::solid_at(gmath::Vec2f world) const {
    // Guard against a zero tile size (an unloaded map) so we never divide by 0.
    if (tile_size_ <= 0) {
        return false;
    }
    // std::floor (not a plain int cast) so that e.g. world.x == -0.5 maps to
    // tile column -1, not 0. Truncation-toward-zero would be wrong for the
    // half-open cells left of the origin.
    const int tx = static_cast<int>(std::floor(world.x / static_cast<float>(tile_size_)));
    const int ty = static_cast<int>(std::floor(world.y / static_cast<float>(tile_size_)));
    return is_solid_id(tile_at(tx, ty));
}

}  // namespace game
