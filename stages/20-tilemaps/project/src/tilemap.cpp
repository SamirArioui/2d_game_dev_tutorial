// ============================================================================
// Tilemap implementation — pure parsing + queries, no SFML.   — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the four methods below so the parser tests in
// tests/test_tilemap.cpp go from RED to GREEN and the demo (src/main.cpp) can
// load and walk a level. The full contract — the on-disk text format, the
// tile-id convention, and what each method must do (including the edge cases
// the tests check) — is documented in include/game/tilemap.hpp; read it first.
//
// The starter already COMPILES and LINKS (each body has a placeholder return),
// but the tests fail until you fill in the TODOs. Everything else in this stage
// (the SFML renderer, the window, the header) is complete — the *lesson* is the
// pure, testable map logic, so that is what you write.
//
// A complete reference is in ../solution/src/tilemap.cpp — try it yourself first.
//
// Build & test (from this stage's project/ folder):
//   cmake -S . -B build && cmake --build build
//   ctest --test-dir build --output-on-failure   # RED until you implement these
// ============================================================================

#include "game/tilemap.hpp"

#include <cmath>      // std::floor
#include <fstream>    // std::ifstream (file loading)
#include <sstream>    // std::istringstream (tokenising the text)
#include <stdexcept>  // std::runtime_error

namespace game {

Tilemap Tilemap::from_text(const std::string& text) {
    // TODO(stage 20): parse the on-disk text format into a Tilemap.
    //  1. Feed `text` to a std::istringstream.
    //  2. Read the header "W H tileSize" into width_/height_/tile_size_ with
    //     `>>`; throw std::runtime_error if the read fails (missing/non-numeric)
    //     or any of the three values is <= 0.
    //  3. Read tile ids with `>>` (it skips whitespace/newlines) into `tiles_`,
    //     then throw if the count isn't exactly width_ * height_ (too few OR too
    //     many). Return the filled map.
    return {};  // placeholder
}

Tilemap Tilemap::from_file(const std::string& path) {
    // TODO(stage 20): open `path` with std::ifstream (RAII — no manual close);
    // throw std::runtime_error if it can't be opened. Slurp the whole file into
    // a string (e.g. via a std::stringstream and file.rdbuf()) and hand it to
    // from_text so the parse logic stays in one tested place.
    return {};  // placeholder
}

int Tilemap::tile_at(int x, int y) const {
    // TODO(stage 20): return the tile id at cell (x, y). Out-of-range cells
    // (x/y < 0 or >= width_/height_) must return 0 (empty) rather than throw, so
    // collision code can query neighbours near the border. Index the flat buffer
    // row-major: tiles_[y * width_ + x].
    return 0;  // placeholder
}

bool Tilemap::solid_at(gmath::Vec2f world) const {
    // TODO(stage 20): convert the WORLD-space point to a grid cell and return
    // whether that tile is solid. Guard tile_size_ <= 0 (unloaded map) -> false.
    // Use std::floor when dividing by tile_size_ so negative coords map to
    // negative cells (a plain int cast would wrongly truncate -0.5 to 0). Then
    // return is_solid_id(tile_at(tx, ty)).
    return false;  // placeholder
}

}  // namespace game
