// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)
#include "world.hpp"

#include <cmath>

namespace game {

namespace {

// A tiny epsilon so a body sitting FLUSH against a tile boundary does not count
// the neighbouring tile as overlapping (floating point would otherwise make
// "exactly touching" ambiguous).
constexpr float kEps = 0.001f;

bool cell_solid(const eng::Tilemap& map, int cx, int cy) {
    const int id = map.tile_at(cx, cy);
    if (id < 0) {
        return true;  // off the map = solid wall
    }
    return map.is_solid_id(id);
}

// Does the box overlap ANY solid tile? Check every grid cell the box touches.
bool overlaps_solid(const eng::Tilemap& map, const gmath::AABB& b) {
    const float ts = static_cast<float>(map.tile_size());
    const int x0 = static_cast<int>(std::floor(b.left() / ts));
    const int x1 = static_cast<int>(std::floor((b.right() - kEps) / ts));
    const int y0 = static_cast<int>(std::floor(b.top() / ts));
    const int y1 = static_cast<int>(std::floor((b.bottom() - kEps) / ts));
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            if (cell_solid(map, x, y)) {
                return true;
            }
        }
    }
    return false;
}

}  // namespace

gmath::Vec2f resolve_move(const eng::Tilemap& map, const gmath::AABB& body,
                          const gmath::Vec2f& delta) {
    const float ts = static_cast<float>(map.tile_size());
    gmath::AABB b = body;

    // --- Move on X, then resolve. Snapping flush against the blocking column is
    //     what makes the body STOP at a wall rather than tunnel through it. ---
    b.pos.x += delta.x;
    if (overlaps_solid(map, b)) {
        if (delta.x > 0.0f) {
            const int col = static_cast<int>(std::floor((b.right() - kEps) / ts));
            b.pos.x = static_cast<float>(col) * ts - b.size.x;  // right edge -> col's left side
        } else if (delta.x < 0.0f) {
            const int col = static_cast<int>(std::floor(b.left() / ts));
            b.pos.x = static_cast<float>(col + 1) * ts;  // left edge -> col's right side
        }
    }

    // --- Now move on Y and resolve independently. Doing the axes SEPARATELY is
    //     what lets you slide along a wall instead of catching on it. ---
    b.pos.y += delta.y;
    if (overlaps_solid(map, b)) {
        if (delta.y > 0.0f) {
            const int row = static_cast<int>(std::floor((b.bottom() - kEps) / ts));
            b.pos.y = static_cast<float>(row) * ts - b.size.y;
        } else if (delta.y < 0.0f) {
            const int row = static_cast<int>(std::floor(b.top() / ts));
            b.pos.y = static_cast<float>(row + 1) * ts;
        }
    }

    return b.pos;
}

std::vector<gmath::Vec2f> try_pickup(std::vector<WorldItem>& items, const gmath::AABB& player,
                                     Inventory& inv) {
    std::vector<gmath::Vec2f> picked;
    for (WorldItem& item : items) {
        if (item.collected) {
            continue;
        }
        if (gmath::aabb_vs_aabb(player, item.box()).collided) {
            inv.add(Item{item.name, item.value});
            item.collected = true;
            picked.push_back(item.box().center());
        }
    }
    return picked;
}

}  // namespace game
