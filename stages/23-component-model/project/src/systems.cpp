// ===========================================================================
// Implementation of the pure component systems — YOUR TASK.
//
// This is the STARTER: each system is stubbed with a TODO and a placeholder return so the project
// compiles and links, but the system tests in tests/test_ecs.cpp FAIL until you implement them.
// See systems.hpp for the "why" and the exact contract each function must satisfy.
//
// A complete reference is in ../solution/src/systems.cpp — try it yourself first.
// ===========================================================================

#include "game/systems.hpp"

#include "ecs/component.hpp"

namespace game {

void movement_system(std::vector<ecs::Entity>& entities, float dt) {
    // TODO(stage 23): for every entity that has BOTH a Transform and a Velocity, advance its
    // position by velocity * dt. Use try_get<T>() to fetch each component and SKIP any entity
    // that is missing either one — that "query" is the essence of a system:
    //     transform->pos += velocity->vel * dt;
}

void bounce_in_bounds(std::vector<ecs::Entity>& entities, const gmath::Vec2f& bounds) {
    // TODO(stage 23): for every entity that has a Transform, Velocity AND Shape, reflect it off the
    // walls of the box [0,0]..bounds (skip entities missing any of the three). On each axis: if the
    // entity crossed a wall, clamp its position back inside — using the Shape's size as its extent
    // on the far (right/bottom) walls — and flip that axis's velocity component.
}

bool aabb_overlap(const gmath::Vec2f& a_pos, const gmath::Vec2f& a_size, const gmath::Vec2f& b_pos,
                  const gmath::Vec2f& b_size) {
    // TODO(stage 23): return true iff the two axis-aligned boxes (top-left pos + size) overlap on
    // BOTH the x and y axes. Use strict '<' so boxes that only touch edges do NOT count as
    // overlapping.
    return false;  // placeholder
}

}  // namespace game
