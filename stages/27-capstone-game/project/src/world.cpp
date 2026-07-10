// ============================================================================
// Stage 27 capstone — GAME LOGIC (implementation) — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the two pure gameplay functions declared in
// world.hpp — resolve_move (axis-separated tile collision) and try_pickup (item
// collection) — using the engine's provided gmath + eng::Tilemap. NO SFML belongs
// here: these are the rules the tests in tests/ drive headless.
//
// The WorldItem type and the declarations you implement against are in world.hpp.
// The starter already compiles and links (each body has a placeholder), so both
// `capstone` and `capstone_tests` build — but the world tests are RED until you
// fill the TODOs in.
//
// A complete reference is in ../solution/src/world.cpp — try it yourself first.
// ============================================================================
#include "world.hpp"

#include <cmath>

namespace game {

gmath::Vec2f resolve_move(const eng::Tilemap& map, const gmath::AABB& body,
                          const gmath::Vec2f& delta) {
    // TODO(stage 27): move `body` by `delta`, resolving against solid tiles ONE
    // AXIS AT A TIME, and return the body's new top-left position.
    //   - A private "does this box overlap any solid tile?" helper keeps this
    //     clean: for every grid cell the box touches (map.tile_size()), treat
    //     off-map cells (tile_at < 0) AND map.is_solid_id(id) as solid. Use a tiny
    //     epsilon on the right/bottom edges so a flush box doesn't read the next cell.
    //   - Move on X first; if that overlaps a solid, snap flush: moving right, the
    //     right edge rests on the blocking column's left side; moving left, the
    //     left edge rests on its right side.
    //   - THEN move on Y and resolve the same way, INDEPENDENTLY. Resolving the axes
    //     separately is what lets the player slide along a wall instead of sticking.
    (void)map;
    (void)delta;
    return body.pos;  // placeholder: nothing resolved yet
}

std::vector<gmath::Vec2f> try_pickup(std::vector<WorldItem>& items, const gmath::AABB& player,
                                     Inventory& inv) {
    // TODO(stage 27): for each not-yet-collected item whose box() overlaps `player`
    // (gmath::aabb_vs_aabb), add one to `inv`, mark it collected, and record its
    // centre. Return the list of picked-up centres so the caller can spawn a
    // particle burst at each.
    (void)items;
    (void)player;
    (void)inv;
    return {};  // placeholder
}

}  // namespace game
