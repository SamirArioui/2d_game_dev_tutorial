#pragma once

// ===========================================================================
// Systems — the BEHAVIOUR half of the component model.
//
// A system is a plain function that walks a collection of entities, picks out the ones carrying
// the components it cares about, and acts on their data. Nothing here knows about SFML or a
// window, so all of it is unit-tested in tests/test_ecs.cpp.
//
// Python comparison: instead of `ball.update()` calling a method on each object (behaviour
// scattered across a class hierarchy), a system is one place that says "for every entity with a
// Transform AND a Velocity, move it" — behaviour grouped by what it does, not by what it's on.
// ===========================================================================

#include <vector>

#include "ecs/entity.hpp"
#include "gmath/vec2.hpp"

namespace game {

// For every entity that has BOTH a Transform and a Velocity, advance position by velocity * dt.
// Entities missing either component are skipped — that "query" is the essence of a system.
void movement_system(std::vector<ecs::Entity>& entities, float dt);

// Reflect any moving, sized entity off the walls of the axis-aligned box [0,0]..bounds, using its
// Shape as its extent. Flips the relevant velocity component and clamps the entity back inside.
void bounce_in_bounds(std::vector<ecs::Entity>& entities, const gmath::Vec2f& bounds);

// Do two axis-aligned boxes (top-left position + size) overlap? A pure predicate, reused by the
// paddle/ball collision in the demo and pinned down by tests.
bool aabb_overlap(const gmath::Vec2f& a_pos, const gmath::Vec2f& a_size, const gmath::Vec2f& b_pos,
                  const gmath::Vec2f& b_size);

}  // namespace game
