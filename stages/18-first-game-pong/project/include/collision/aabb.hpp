#pragma once

// ============================================================================
// Axis-Aligned Bounding Box collision (self-contained copy from stage 16).
//
// "Axis-aligned" means the box's edges are parallel to the X/Y axes — no
// rotation. That restriction makes the overlap test trivial and fast, which is
// exactly why Pong (and most 2D games' broad checks) use it. This is the SAME
// shape stage 16 introduced and unit-tested; we carry a copy so this stage is
// self-contained, and we re-test it here.
//
// SFML-free on purpose: collision is pure logic, so it stays testable headless.
// ============================================================================

#include <optional>

#include "gmath/vec2.hpp"

namespace collision {

// A box described by its TOP-LEFT corner `pos` and its `size` (width, height).
// (Top-left origin matches SFML's screen coordinates: +y points DOWN.)
struct AABB {
    gmath::Vec2f pos;   // top-left corner
    gmath::Vec2f size;  // width, height (both >= 0)
};

// The result of a collision: the MINIMUM translation that pushes `a` out of
// `b`. `penetration.x`/`.y` is how deep they overlap on each axis, signed so
// that adding it to `a.pos` separates them along the shallower axis. Callers
// resolve by moving along whichever component is smaller in magnitude.
struct Hit {
    gmath::Vec2f penetration;
};

// Do the two boxes overlap? If so, return a Hit with the penetration vector;
// otherwise std::nullopt (stage 10's "a value OR nothing"). Touching exactly
// edge-to-edge counts as NO overlap (penetration would be zero).
std::optional<Hit> aabb_vs_aabb(const AABB& a, const AABB& b);

}  // namespace collision
