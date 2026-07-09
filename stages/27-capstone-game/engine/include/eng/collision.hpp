#pragma once

// ============================================================================
// eng collision — axis-aligned bounding boxes and the one collision test the
// whole engine needs. Also SFML-free (namespace `gmath`), also unit-tested.
//
// An AABB is the simplest useful collider: a rectangle whose sides stay parallel
// to the X/Y axes (no rotation). That restriction makes the overlap test a
// handful of comparisons — fast enough to run on every entity every frame.
// ============================================================================

#include <algorithm>

#include "eng/math.hpp"

namespace gmath {

// Axis-aligned box, stored as a top-left corner + size (the SFML convention,
// so it maps straight onto sf::FloatRect and sprite positions).
struct AABB {
    Vec2f pos;   // top-left corner
    Vec2f size;  // width, height

    float left() const { return pos.x; }
    float top() const { return pos.y; }
    float right() const { return pos.x + size.x; }
    float bottom() const { return pos.y + size.y; }

    Vec2f center() const { return {pos.x + size.x * 0.5f, pos.y + size.y * 0.5f}; }

    bool contains(const Vec2f& p) const {
        return p.x >= left() && p.x <= right() && p.y >= top() && p.y <= bottom();
    }
};

// Result of a collision query. `penetration` is the MINIMUM translation vector:
// add it to box `a`'s position and `a` no longer overlaps `b`. It is (0,0) when
// there is no collision.
struct Hit {
    bool collided{false};
    Vec2f penetration{};
};

// Overlap test for two AABBs. Returns the minimum-translation vector so callers
// can both DETECT and RESOLVE the collision from one call.
//
// The idea: compute how much the boxes overlap on X and on Y. If either overlap
// is <= 0 they are apart. Otherwise we push out along the axis of SMALLER
// overlap (the shortest way out), with a sign that moves `a` away from `b`.
inline Hit aabb_vs_aabb(const AABB& a, const AABB& b) {
    const float overlap_x = std::min(a.right(), b.right()) - std::max(a.left(), b.left());
    const float overlap_y = std::min(a.bottom(), b.bottom()) - std::max(a.top(), b.top());

    if (overlap_x <= 0.0f || overlap_y <= 0.0f) {
        return Hit{false, Vec2f{}};
    }

    if (overlap_x < overlap_y) {
        // Resolve horizontally. Push left if a's centre is left of b's, else right.
        const float sign = (a.center().x < b.center().x) ? -1.0f : 1.0f;
        return Hit{true, Vec2f{sign * overlap_x, 0.0f}};
    }
    // Resolve vertically.
    const float sign = (a.center().y < b.center().y) ? -1.0f : 1.0f;
    return Hit{true, Vec2f{0.0f, sign * overlap_y}};
}

}  // namespace gmath
