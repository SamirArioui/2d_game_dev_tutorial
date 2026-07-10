#pragma once

// ============================================================================
// camera — the PURE math behind a 2D camera (stage 21). No SFML in here.
//
// SFML's camera is `sf::View`: a rectangle of the world (a center + a size)
// that gets stretched onto the window. Following a target, clamping to the
// level, converting between screen pixels and world coordinates, and parallax
// are all just arithmetic on that rectangle. We pull that arithmetic OUT into
// pure functions so it can be unit-tested with no window — the same discipline
// as the stage-20 tilemap parser.
//
// The functions below mirror what SFML does internally:
//   clamp_view_center  <-> keeping view.setCenter(...) inside the level
//   screen_to_world    <-> sf::RenderWindow::mapPixelToCoords
//   world_to_screen    <-> sf::RenderWindow::mapCoordsToPixel
//   parallax_offset    <-> how far a background layer has scrolled
// (All assume an axis-aligned, unrotated view whose viewport is the whole
// window — the common case.)
// ============================================================================

#include "game/gmath.hpp"

namespace game {

// Constrain a scalar to the inclusive range [lo, hi].
float clamp(float value, float lo, float hi);

// Keep a camera CENTER so that a view of size `view_size` stays fully inside a
// level of size `level_size` (all in world pixels). If the level is smaller
// than the view on an axis, the view is centered on the level instead (so you
// see the whole small level, letterboxed). This stops the camera from showing
// the void beyond the level edges.
gmath::Vec2f clamp_view_center(gmath::Vec2f desired_center, gmath::Vec2f view_size,
                               gmath::Vec2f level_size);

// Convert a SCREEN pixel to a WORLD coordinate, given the view's center and
// size and the window's pixel size. This is what a mouse click needs: the click
// arrives in pixels, but the thing it hits lives in world space.
//   world = (view_center - view_size/2) + (pixel / window_size) * view_size
gmath::Vec2f screen_to_world(gmath::Vec2f pixel, gmath::Vec2f view_center,
                             gmath::Vec2f view_size, gmath::Vec2f window_size);

// The exact inverse: WORLD coordinate -> SCREEN pixel. Useful for drawing a
// screen-space marker over a world object.
gmath::Vec2f world_to_screen(gmath::Vec2f world, gmath::Vec2f view_center,
                             gmath::Vec2f view_size, gmath::Vec2f window_size);

// Parallax: how far a background layer has scrolled, given the main camera
// center and a depth `factor` in [0, 1]. The value is `camera_center * factor`,
// which you can use either as the layer's view center OR (for a screen-space
// layer) as the offset of its texture rectangle.
//   factor 1.0 -> scrolls exactly with the world (same as the foreground)
//   factor 0.0 -> never scrolls (pinned, like the sky or a HUD)
//   factor 0.3 -> distant hills that drift slowly
// Distant layers use a smaller factor, so they scroll slower — the illusion of
// depth.
gmath::Vec2f parallax_offset(gmath::Vec2f camera_center, float factor);

}  // namespace game
