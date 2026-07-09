#pragma once

// ============================================================================
// eng::Tilemap — a grid of integer tile ids, loaded from a simple text format.
//
// Parsing and queries are SFML-FREE so they unit-test cleanly (a map is just
// data). Drawing is a SEPARATE concern handled by TileRenderer (tile_renderer.hpp),
// which turns this data into a vertex array.
//
// File format (whitespace separated):
//     W H tileSize        <- header: columns, rows, pixel size of one tile
//     row 0 ...           <- H rows, each with W integer tile ids
//     ...
//
// Example (4x3 map, 32px tiles; 1 = wall, 0 = floor):
//     4 3 32
//     1 1 1 1
//     1 0 0 1
//     1 1 1 1
// ============================================================================

#include <string>
#include <unordered_set>
#include <vector>

#include "eng/math.hpp"

namespace eng {

class Tilemap {
public:
    // Parse from an in-memory string (used by tests) or a file. Return false on
    // a malformed header / wrong tile count so callers can react instead of
    // silently rendering garbage.
    bool load_from_string(const std::string& text);
    bool load_from_file(const std::string& path);

    int width() const { return width_; }
    int height() const { return height_; }
    int tile_size() const { return tile_size_; }

    // Tile id at grid cell (x, y). Returns -1 for out-of-bounds cells so callers
    // can distinguish "off the map" from any real id.
    int tile_at(int x, int y) const;

    // Is the cell containing this WORLD-SPACE (pixel) position solid? Positions
    // outside the map count as solid, so entities can't walk off the edge.
    bool solid_at(const gmath::Vec2f& world_pos) const;

    // Mark a tile id as solid (or not). By convention id 1 is solid by default.
    void set_solid(int tile_id, bool solid);
    bool is_solid_id(int tile_id) const;

    // Size of the whole map in pixels — handy for clamping a camera.
    gmath::Vec2f pixel_size() const {
        return {static_cast<float>(width_ * tile_size_), static_cast<float>(height_ * tile_size_)};
    }

    const std::vector<int>& tiles() const { return tiles_; }

private:
    int width_{0};
    int height_{0};
    int tile_size_{1};
    std::vector<int> tiles_;                 // row-major: tiles_[y * width_ + x]
    std::unordered_set<int> solid_ids_{1};   // id 1 is a wall unless reconfigured
};

}  // namespace eng
