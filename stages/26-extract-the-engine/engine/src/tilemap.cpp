#include "eng/tilemap.hpp"

#include <cmath>
#include <fstream>
#include <sstream>

namespace eng {

bool Tilemap::load_from_string(const std::string& text) {
    std::istringstream in(text);

    int w = 0;
    int h = 0;
    int ts = 0;
    if (!(in >> w >> h >> ts) || w <= 0 || h <= 0 || ts <= 0) {
        return false;  // malformed / missing header
    }

    std::vector<int> tiles;
    tiles.reserve(static_cast<std::size_t>(w) * static_cast<std::size_t>(h));
    int id = 0;
    while (in >> id) {
        tiles.push_back(id);
    }

    // Exactly W*H ids must follow the header — no more, no fewer.
    if (tiles.size() != static_cast<std::size_t>(w) * static_cast<std::size_t>(h)) {
        return false;
    }

    width_ = w;
    height_ = h;
    tile_size_ = ts;
    tiles_ = std::move(tiles);
    return true;
}

bool Tilemap::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return load_from_string(buffer.str());
}

int Tilemap::tile_at(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return -1;
    }
    return tiles_[static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) +
                  static_cast<std::size_t>(x)];
}

bool Tilemap::solid_at(const gmath::Vec2f& world_pos) const {
    // Convert pixel position -> grid cell. floor() (not int truncation) so
    // negative coordinates map to the correct cell instead of toward zero.
    const int cell_x = static_cast<int>(std::floor(world_pos.x / tile_size_));
    const int cell_y = static_cast<int>(std::floor(world_pos.y / tile_size_));
    const int id = tile_at(cell_x, cell_y);
    if (id < 0) {
        return true;  // off the map = wall
    }
    return is_solid_id(id);
}

void Tilemap::set_solid(int tile_id, bool solid) {
    if (solid) {
        solid_ids_.insert(tile_id);
    } else {
        solid_ids_.erase(tile_id);
    }
}

bool Tilemap::is_solid_id(int tile_id) const {
    return solid_ids_.count(tile_id) > 0;
}

}  // namespace eng
