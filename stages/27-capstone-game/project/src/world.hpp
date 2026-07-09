#pragma once

// ============================================================================
// Capstone GAME LOGIC — pure, SFML-free, unit-tested.
//
// Two things the game needs that are pure functions of data:
//   1. resolve_move : slide an AABB body by a delta, but stop it at solid tiles
//                     (axis-separated tile collision).
//   2. try_pickup   : collect any items the player overlaps into the inventory.
//
// Both take plain data (a Tilemap, AABBs, vectors) and return plain data, so the
// tests drive them with no window — the same discipline as the Pong rules.
// ============================================================================

#include <string>
#include <vector>

#include "eng/collision.hpp"
#include "eng/math.hpp"
#include "eng/tilemap.hpp"
#include "inventory.hpp"

namespace game {

// An item sitting in the world until picked up.
struct WorldItem {
    std::string name;
    int value{0};
    gmath::Vec2f pos;       // top-left of the item's little box
    float size{20.0f};
    bool collected{false};

    gmath::AABB box() const { return gmath::AABB{pos, {size, size}}; }
};

// Move `body` by `delta`, resolving against solid tiles one axis at a time, and
// return the body's new TOP-LEFT position. Moving into a wall snaps the body flush
// against it instead of passing through. Axis separation (X then Y) is what lets
// you slide along a wall instead of sticking to it.
gmath::Vec2f resolve_move(const eng::Tilemap& map, const gmath::AABB& body,
                          const gmath::Vec2f& delta);

// Collect every not-yet-collected item whose box overlaps `player`, adding one of
// each to `inv` and marking it collected. Returns the world positions of the
// items picked up this call (so the caller can spawn a particle burst at each).
std::vector<gmath::Vec2f> try_pickup(std::vector<WorldItem>& items, const gmath::AABB& player,
                                     Inventory& inv);

}  // namespace game
