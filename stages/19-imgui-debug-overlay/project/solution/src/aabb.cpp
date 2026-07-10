// ============================================================================
// AABB overlap test + penetration (minimum translation vector).
// ============================================================================
#include "collision/aabb.hpp"

#include <algorithm>  // std::min, std::max

namespace collision {

std::optional<Hit> aabb_vs_aabb(const AABB& a, const AABB& b) {
    const float a_left = a.pos.x;
    const float a_right = a.pos.x + a.size.x;
    const float a_top = a.pos.y;
    const float a_bottom = a.pos.y + a.size.y;

    const float b_left = b.pos.x;
    const float b_right = b.pos.x + b.size.x;
    const float b_top = b.pos.y;
    const float b_bottom = b.pos.y + b.size.y;

    // Overlap length on each axis. If either is <= 0 the boxes don't overlap
    // (== 0 means they merely touch, which we treat as "not colliding").
    const float overlap_x = std::min(a_right, b_right) - std::max(a_left, b_left);
    const float overlap_y = std::min(a_bottom, b_bottom) - std::max(a_top, b_top);

    if (overlap_x <= 0.0f || overlap_y <= 0.0f) {
        return std::nullopt;
    }

    // Direction to push `a` OUT of `b`: away from b's center. If a's center is
    // left of b's, push a further left (negative x), and so on.
    const float a_center_x = a.pos.x + a.size.x * 0.5f;
    const float a_center_y = a.pos.y + a.size.y * 0.5f;
    const float b_center_x = b.pos.x + b.size.x * 0.5f;
    const float b_center_y = b.pos.y + b.size.y * 0.5f;

    const float dir_x = (a_center_x < b_center_x) ? -1.0f : 1.0f;
    const float dir_y = (a_center_y < b_center_y) ? -1.0f : 1.0f;

    // Penetration = signed overlap on each axis. The caller resolves along the
    // component with the smaller magnitude (the shallower axis).
    return Hit{gmath::Vec2f{overlap_x * dir_x, overlap_y * dir_y}};
}

}  // namespace collision
