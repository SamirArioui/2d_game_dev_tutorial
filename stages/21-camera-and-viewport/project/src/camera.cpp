// ============================================================================
// camera implementation — pure arithmetic, no SFML.   — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the pure camera maths below so the tests in
// tests/test_camera.cpp go from RED to GREEN and the demo (src/main.cpp) can
// follow/clamp its sf::View and place parallax + HUD correctly. Each function's
// exact contract — the formulas and the edge cases the tests check — is
// documented in include/game/camera.hpp; read it first.
//
// The starter already COMPILES and LINKS (each body has a placeholder return),
// but the tests fail until you fill in the TODOs. Everything else in this stage
// (the SFML sf::View / parallax / HUD boilerplate, the carried tilemap, the
// window) is complete — the *lesson* is the pure, testable camera arithmetic,
// so that is what you write.
//
// A complete reference is in ../solution/src/camera.cpp — try it yourself first.
//
// Build & test (from this stage's project/ folder):
//   cmake -S . -B build && cmake --build build
//   ctest --test-dir build --output-on-failure   # RED until you implement these
// ============================================================================

#include "game/camera.hpp"

namespace game {

float clamp(float value, float lo, float hi) {
    // TODO(stage 21): constrain `value` to the inclusive range [lo, hi] — return
    // lo if below it, hi if above it, else value unchanged. This is the atom the
    // other bounds work is built on.
    return value;  // placeholder
}

gmath::Vec2f clamp_view_center(gmath::Vec2f desired_center, gmath::Vec2f view_size,
                               gmath::Vec2f level_size) {
    // TODO(stage 21): keep the camera center so a `view_size` view stays fully
    // inside a `level_size` level. Per axis: the center may range over
    // [half, level - half] where half = view_size/2 — clamp() it there. BUT if
    // the level is smaller than the view on that axis (level <= view), that
    // range is empty, so center on the level instead (result = level/2).
    return {};  // placeholder
}

gmath::Vec2f screen_to_world(gmath::Vec2f pixel, gmath::Vec2f view_center,
                             gmath::Vec2f view_size, gmath::Vec2f window_size) {
    // TODO(stage 21): map a SCREEN pixel to a WORLD coordinate. The view's
    // top-left world corner is view_center - view_size/2; add the fraction
    // across the window (pixel / window_size) scaled by view_size:
    //   world = top_left + (pixel / window_size) * view_size   (per axis)
    return {};  // placeholder
}

gmath::Vec2f world_to_screen(gmath::Vec2f world, gmath::Vec2f view_center,
                             gmath::Vec2f view_size, gmath::Vec2f window_size) {
    // TODO(stage 21): the exact inverse of screen_to_world — map a WORLD coord
    // to a SCREEN pixel:
    //   pixel = ((world - top_left) / view_size) * window_size   (per axis)
    // with top_left = view_center - view_size/2. Must round-trip with
    // screen_to_world.
    return {};  // placeholder
}

gmath::Vec2f parallax_offset(gmath::Vec2f camera_center, float factor) {
    // TODO(stage 21): return camera_center * factor (per component). factor 1.0
    // scrolls with the world, 0.0 stays pinned, values between scroll slower for
    // distant layers.
    return {};  // placeholder
}

}  // namespace game
