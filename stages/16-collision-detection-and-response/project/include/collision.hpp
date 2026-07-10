#pragma once

// ===========================================================================
// collide — pure 2D collision detection & response, built on gmath::Vec2f.  (YOUR TASK)
//
// This is the STARTER. The structs (AABB / Circle / Hit), the aabb_center helper,
// and the gmath dependency are given. YOUR job is to implement the four functions
// marked `// TODO(stage 16): ...` — aabb_vs_aabb, circle_vs_circle, circle_vs_aabb,
// and reflect_velocity. Each ships as a placeholder return so the project compiles
// and the window app links, but ../tests/test_collision.cpp starts RED until you
// fill them in. A reference is in ../solution/include/collision.hpp — try it first.
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
    // TODO(stage 16): compute the overlap on each axis:
    //   overlap_x = min(a.right, b.right) - max(a.left, b.left);
    //   overlap_y = min(a.bottom, b.bottom) - max(a.top, b.top);
    // If either is <= 0 there's a gap -> return std::nullopt. Otherwise back out
    // along the axis of SMALLER overlap, signed by comparing centers (aabb_center)
    // so the penetration pushes A away from B. Return Hit{penetration}.
    (void)a;
    (void)b;
    return std::nullopt;  // placeholder: never detects a hit yet
}

// --- Circle vs Circle ------------------------------------------------------
//
// Two circles overlap iff the distance between centers is less than the sum of
// radii. We compare SQUARED distances first to avoid a sqrt when they miss. The
// penetration points from B's center toward A's center (so it pushes A away),
// with length = (r_a + r_b) - distance.
inline std::optional<Hit> circle_vs_circle(const Circle& a, const Circle& b) {
    // TODO(stage 16): delta = a.center - b.center (from B toward A). The circles
    // miss iff length_squared(delta) >= (a.radius + b.radius)² — early-out with
    // no sqrt. Otherwise depth = (rA + rB) - length(delta), and the penetration
    // is the unit delta * depth (push A away from B). Guard the concentric case
    // (dist ~0): pick an arbitrary axis, e.g. {depth, 0}.
    (void)a;
    (void)b;
    return std::nullopt;  // placeholder: never detects a hit yet
}

// --- Circle vs AABB --------------------------------------------------------
//
// Clamp the circle's center to the box to get the CLOSEST point on the box. If
// that point is farther than one radius from the center, they miss. Otherwise
// the penetration pushes the circle out along the line from the closest point to
// the center. The tricky case is the center being INSIDE the box (closest point
// == center, so there's no direction): we then eject through the nearest face.
inline std::optional<Hit> circle_vs_aabb(const Circle& c, const AABB& box) {
    // TODO(stage 16): find the closest point on the box to the circle's center by
    // clamping the center to [left,right] x [top,bottom] (gmath::clamp per axis).
    // diff = center - closest. Miss iff length_squared(diff) > radius². Otherwise:
    //   * center OUTSIDE the box (dist > ~0): push out along the unit diff by
    //     (radius - length(diff));
    //   * center INSIDE the box (dist ~0, closest == center): eject through the
    //     NEAREST face — compare distances to left/right/top/bottom and push out
    //     that way by (face distance + radius).
    (void)c;
    (void)box;
    return std::nullopt;  // placeholder: never detects a hit yet
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
    // TODO(stage 16): reflect v off a surface with outward unit normal `normal`.
    // Let vn = dot(v, normal). If vn >= 0 the object is moving away (or along) —
    // return v unchanged so it can't stick. Otherwise return
    //   v - normal * ((1 + restitution) * vn).
    // restitution in [0,1]: 1 = perfect bounce, 0 = kill the normal component.
    (void)normal;
    (void)restitution;
    return v;  // placeholder: no reflection yet
}

}  // namespace collide
