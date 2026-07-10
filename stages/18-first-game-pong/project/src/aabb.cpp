// ============================================================================
// AABB overlap test + penetration (minimum translation vector).  (YOUR TASK)
//
// This is the STARTER — the same overlap test you built in stage 16, carried
// here so Pong is self-contained. Implement the body below so
// tests/test_collision.cpp goes GREEN and ball_hits_paddle() works.
//
// A complete reference is in ../solution/src/aabb.cpp — try it yourself first.
// ============================================================================
#include "collision/aabb.hpp"

#include <algorithm>  // std::min, std::max

namespace collision {

std::optional<Hit> aabb_vs_aabb(const AABB& a, const AABB& b) {
    // TODO(stage 18, from stage 16): return whether boxes `a` and `b` overlap,
    // and if so the penetration (minimum translation) vector.
    //   * each box spans [pos, pos + size] on x and y.
    //   * overlap_x = min(a_right, b_right) - max(a_left, b_left); overlap_y
    //     likewise. If EITHER is <= 0 the boxes don't overlap (== 0 = merely
    //     touching, which counts as NO collision) -> return std::nullopt.
    //   * otherwise push `a` away from b's centre: dir_x = (a_center_x <
    //     b_center_x) ? -1 : +1 (same for y), and return
    //     Hit{ {overlap_x * dir_x, overlap_y * dir_y} }.
    (void)a;
    (void)b;
    return std::nullopt;  // placeholder
}

}  // namespace collision
