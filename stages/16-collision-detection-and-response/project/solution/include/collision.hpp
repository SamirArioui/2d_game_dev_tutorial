#pragma once

// ===========================================================================
// collide — pure 2D collision detection & response, built on gmath::Vec2f.  (REFERENCE SOLUTION)
//
// This is the fully-implemented reference. The STARTER you fill in is two levels
// up at ../../include/collision.hpp, where the predicate bodies are TODO stubs.
//
// Detection answers "are these two shapes overlapping, and if so, by how much
// and in which direction?" Response answers "how should their velocities change
// as a result?" We split every routine into a small, window-free function so the
// whole thing is unit-tested (project/tests/) — the stage's central promise.
//
// The KEY idea shared by all three detectors is the PENETRATION vector (a.k.a.
// the minimum translation vector, MTV): the shortest shift that would separate
// the two shapes. Adding `hit.penetration` to the FIRST shape's position moves
// it just clear of the SECOND. Its direction is the contact NORMAL; its length
// is the overlap depth. Detection returns std::optional<Hit>: `nullopt` means no
// overlap (the Python-ish "return None"), a value means they hit.
// ===========================================================================

#include <algorithm>  // std::min, std::max
#include <optional>

#include "vec2.hpp"

namespace collide {

// An axis-aligned bounding box. `pos` is the TOP-LEFT corner (remember +y is
// down), `size` is {width, height}. "Axis-aligned" = never rotated, which is
// what makes the overlap test just four comparisons.
struct AABB {
    gmath::Vec2f pos;
    gmath::Vec2f size;
};

struct Circle {
    gmath::Vec2f center;
    float radius;
};

// The result of a collision: how to separate the first shape from the second.
struct Hit {
    gmath::Vec2f penetration;  // add this to shape-A's position to un-overlap it
};

// --- helpers ---------------------------------------------------------------
inline gmath::Vec2f aabb_center(const AABB& b) {
    return b.pos + b.size * 0.5f;
}

// --- AABB vs AABB ----------------------------------------------------------
//
// Two boxes overlap iff they overlap on BOTH axes. On each axis, the overlap
// depth is (smaller far edge) - (larger near edge); if either is <= 0 there's a
// gap and they miss. When they do overlap, the cheapest way out is along the
// axis of SMALLEST overlap (you back out the short way), so the penetration
// vector lies on that axis, signed so it pushes A away from B.
inline std::optional<Hit> aabb_vs_aabb(const AABB& a, const AABB& b) {
    const float a_left = a.pos.x, a_right = a.pos.x + a.size.x;
    const float a_top = a.pos.y, a_bottom = a.pos.y + a.size.y;
    const float b_left = b.pos.x, b_right = b.pos.x + b.size.x;
    const float b_top = b.pos.y, b_bottom = b.pos.y + b.size.y;

    const float overlap_x = std::min(a_right, b_right) - std::max(a_left, b_left);
    const float overlap_y = std::min(a_bottom, b_bottom) - std::max(a_top, b_top);

    if (overlap_x <= 0.0f || overlap_y <= 0.0f) {
        return std::nullopt;  // a gap on at least one axis -> no collision
    }

    const gmath::Vec2f ca = aabb_center(a);
    const gmath::Vec2f cb = aabb_center(b);

    if (overlap_x < overlap_y) {
        // Separate horizontally: push A left if it sits left of B, else right.
        const float sign = (ca.x < cb.x) ? -1.0f : 1.0f;
        return Hit{gmath::Vec2f{sign * overlap_x, 0.0f}};
    }
    // Separate vertically.
    const float sign = (ca.y < cb.y) ? -1.0f : 1.0f;
    return Hit{gmath::Vec2f{0.0f, sign * overlap_y}};
}

// --- Circle vs Circle ------------------------------------------------------
//
// Two circles overlap iff the distance between centers is less than the sum of
// radii. We compare SQUARED distances first to avoid a sqrt when they miss. The
// penetration points from B's center toward A's center (so it pushes A away),
// with length = (r_a + r_b) - distance.
inline std::optional<Hit> circle_vs_circle(const Circle& a, const Circle& b) {
    const gmath::Vec2f delta = a.center - b.center;  // from B toward A
    const float r = a.radius + b.radius;
    const float dist_sq = gmath::length_squared(delta);

    if (dist_sq >= r * r) {
        return std::nullopt;
    }

    const float dist = gmath::length(delta);
    const float depth = r - dist;

    if (dist > 1e-8f) {
        const gmath::Vec2f normal = delta * (1.0f / dist);  // unit, B -> A
        return Hit{normal * depth};
    }
    // Exactly concentric: no defined direction, so pick +x arbitrarily.
    return Hit{gmath::Vec2f{depth, 0.0f}};
}

// --- Circle vs AABB --------------------------------------------------------
//
// Clamp the circle's center to the box to get the CLOSEST point on the box. If
// that point is farther than one radius from the center, they miss. Otherwise
// the penetration pushes the circle out along the line from the closest point to
// the center. The tricky case is the center being INSIDE the box (closest point
// == center, so there's no direction): we then eject through the nearest face.
inline std::optional<Hit> circle_vs_aabb(const Circle& c, const AABB& box) {
    const float left = box.pos.x, right = box.pos.x + box.size.x;
    const float top = box.pos.y, bottom = box.pos.y + box.size.y;

    const gmath::Vec2f closest{gmath::clamp(c.center.x, left, right),
                               gmath::clamp(c.center.y, top, bottom)};
    const gmath::Vec2f diff = c.center - closest;  // closest-point -> center
    const float dist_sq = gmath::length_squared(diff);

    if (dist_sq > c.radius * c.radius) {
        return std::nullopt;
    }

    if (dist_sq > 1e-8f) {
        // Center outside the box: push out along the surface normal.
        const float dist = gmath::length(diff);
        const gmath::Vec2f normal = diff * (1.0f / dist);
        return Hit{normal * (c.radius - dist)};
    }

    // Center INSIDE the box: eject through whichever face is nearest.
    const float d_left = c.center.x - left;
    const float d_right = right - c.center.x;
    const float d_top = c.center.y - top;
    const float d_bottom = bottom - c.center.y;
    const float min_x = std::min(d_left, d_right);
    const float min_y = std::min(d_top, d_bottom);

    if (min_x < min_y) {
        return (d_left < d_right) ? Hit{gmath::Vec2f{-(d_left + c.radius), 0.0f}}
                                  : Hit{gmath::Vec2f{d_right + c.radius, 0.0f}};
    }
    return (d_top < d_bottom) ? Hit{gmath::Vec2f{0.0f, -(d_top + c.radius)}}
                              : Hit{gmath::Vec2f{0.0f, d_bottom + c.radius}};
}

// --- response: bounce with restitution -------------------------------------
//
// Reflect a velocity off a surface whose outward unit normal is `normal`.
// Restitution e in [0,1] is bounciness: 1 = perfect elastic bounce (no energy
// lost), 0 = the normal component is killed (slides/stops, no bounce).
//
//   v' = v - (1 + e) * dot(v, n) * n
//
// We only reflect when the object is moving INTO the surface (dot(v, n) < 0);
// if it's already moving away we leave it alone, so it can't get stuck jittering
// against the wall.
inline gmath::Vec2f reflect_velocity(const gmath::Vec2f& v, const gmath::Vec2f& normal,
                                     float restitution) {
    const float vn = gmath::dot(v, normal);
    if (vn >= 0.0f) {
        return v;  // moving away from (or along) the surface — nothing to do
    }
    return v - normal * ((1.0f + restitution) * vn);
}

}  // namespace collide
