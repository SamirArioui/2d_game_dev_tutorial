#pragma once

// ============================================================================
// Tilemap — the course's canonical tile-grid type (stage 20).
//
// A tilemap is the data behind every 2D level built from square tiles: a grid
// of integer "tile ids" plus how big each tile is in pixels. This is the exact
// same idea as a Python 2D list of ints...
//
//     grid = [
//         [1, 1, 1, 1],     # row 0
//         [1, 0, 0, 1],     # row 1
//         [1, 1, 1, 1],     # row 2
//     ]
//     grid[y][x]            # tile at column x, row y
//
// ...with three C++ differences worth internalising:
//
//   1. TYPED and FIXED-SHAPE. Every cell is an `int`; the grid is W*H, decided
//      at load time. No ragged rows, no mixing ints and strings.
//   2. STORED FLAT. Instead of a vector-of-vectors we keep ONE `std::vector<int>`
//      of length W*H and index it as `tiles_[y * width_ + x]` (row-major). One
//      contiguous block is cache-friendly and is exactly what the GPU-facing
//      vertex array will walk in stage-order later.
//   3. SEPARATED FROM RENDERING. This class is PURE data + queries: no SFML, no
//      window. That is deliberate — it makes the parser unit-testable without a
//      display (the whole point of stage 13's "keep logic pure" rule). Turning a
//      Tilemap into pixels lives in a separate SFML function (tilemap_render.*).
//
// ON-DISK TEXT FORMAT (the course-wide contract):
//
//     W H tileSize        <- first line: three ints (width, height, tile pixels)
//     id id id ... id      <- then H rows, each of W space-separated tile ids
//     ...
//
// Example (4x3 map, 32px tiles, a walled room):
//
//     4 3 32
//     1 1 1 1
//     1 0 0 1
//     1 1 1 1
//
// TILE-ID CONVENTION: id 0 means "empty / non-solid" (walkable). Any NON-ZERO
// id is a solid tile you collide with. (Different non-zero ids can still be
// drawn differently — e.g. 1 = grass wall, 2 = stone — but for collision the
// rule is simply "id != 0 is solid".)
// ============================================================================

#include <string>
#include <vector>

#include "game/gmath.hpp"

namespace game {

class Tilemap {
public:
    Tilemap() = default;

    // Build a map from an in-memory string in the on-disk format above. This is
    // the testable entry point: a test can hand it a literal and check the
    // result, no file needed. Throws std::runtime_error on malformed input
    // (bad header, non-integer token, or the wrong number of tiles).
    static Tilemap from_text(const std::string& text);

    // Load a map from a file path (reuses the file I/O from stage 10). Throws
    // std::runtime_error if the file cannot be opened, then defers to
    // from_text for the parse. Keeping the parse split out is what lets us test
    // it without touching the filesystem.
    static Tilemap from_file(const std::string& path);

    // --- dimensions (const accessors: they promise not to mutate the map) ---
    int width() const { return width_; }
    int height() const { return height_; }
    int tile_size() const { return tile_size_; }

    // Tile id at grid cell (x, y). OUT-OF-RANGE cells return 0 (empty) rather
    // than throwing, so collision code can query neighbours near the border
    // without bounds-checking every call. This is a common game-code choice:
    // "off the map reads as empty".
    int tile_at(int x, int y) const;

    // The collision rule in one place: non-zero ids are solid.
    bool is_solid_id(int id) const { return id != 0; }

    // Is the tile under this WORLD-space point solid? Converts the world
    // position to a grid cell (flooring, so negative coords work correctly)
    // and asks tile_at. This is the query the character controller uses.
    bool solid_at(gmath::Vec2f world) const;

    // Read-only view of the flat tile buffer, for the renderer to walk.
    const std::vector<int>& tiles() const { return tiles_; }

private:
    int width_ = 0;
    int height_ = 0;
    int tile_size_ = 0;
    std::vector<int> tiles_;  // row-major, size width_ * height_
};

}  // namespace game
