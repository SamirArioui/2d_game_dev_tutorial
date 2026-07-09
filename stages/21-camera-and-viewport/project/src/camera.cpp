// ============================================================================
// camera implementation — pure arithmetic, no SFML.
// ============================================================================

#include "game/camera.hpp"

namespace game {

float clamp(float value, float lo, float hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

gmath::Vec2f clamp_view_center(gmath::Vec2f desired_center, gmath::Vec2f view_size,
                               gmath::Vec2f level_size) {
    const gmath::Vec2f half{view_size.x * 0.5f, view_size.y * 0.5f};

    gmath::Vec2f result = desired_center;

    // X axis: the center may range over [half, level - half]. If the level is
    // narrower than the view (level < 2*half), that range is empty, so we just
    // center on the level instead.
    if (level_size.x <= view_size.x) {
        result.x = level_size.x * 0.5f;
    } else {
        result.x = clamp(desired_center.x, half.x, level_size.x - half.x);
    }

    if (level_size.y <= view_size.y) {
        result.y = level_size.y * 0.5f;
    } else {
        result.y = clamp(desired_center.y, half.y, level_size.y - half.y);
    }

    return result;
}

gmath::Vec2f screen_to_world(gmath::Vec2f pixel, gmath::Vec2f view_center,
                             gmath::Vec2f view_size, gmath::Vec2f window_size) {
    // Top-left world corner of what the view shows.
    const gmath::Vec2f top_left{view_center.x - view_size.x * 0.5f,
                                view_center.y - view_size.y * 0.5f};
    // Fraction across the window (0..1), scaled up by the view's world size.
    return gmath::Vec2f{top_left.x + (pixel.x / window_size.x) * view_size.x,
                        top_left.y + (pixel.y / window_size.y) * view_size.y};
}

gmath::Vec2f world_to_screen(gmath::Vec2f world, gmath::Vec2f view_center,
                             gmath::Vec2f view_size, gmath::Vec2f window_size) {
    const gmath::Vec2f top_left{view_center.x - view_size.x * 0.5f,
                                view_center.y - view_size.y * 0.5f};
    return gmath::Vec2f{((world.x - top_left.x) / view_size.x) * window_size.x,
                        ((world.y - top_left.y) / view_size.y) * window_size.y};
}

gmath::Vec2f parallax_offset(gmath::Vec2f camera_center, float factor) {
    return gmath::Vec2f{camera_center.x * factor, camera_center.y * factor};
}

}  // namespace game
