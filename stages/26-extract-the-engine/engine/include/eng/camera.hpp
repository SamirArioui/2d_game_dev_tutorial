#pragma once

// ============================================================================
// eng::Camera — a follow-and-clamp wrapper around sf::View.
//
// A "camera" in 2D is just which rectangle of the WORLD we currently show. SFML
// calls that an sf::View. This class adds the two behaviours every scrolling
// game wants: FOLLOW a target (centre the view on it) and CLAMP to the world
// bounds (so you never see past the edge of the map).
// ============================================================================

#include <SFML/Graphics/View.hpp>

#include "eng/collision.hpp"
#include "eng/math.hpp"

namespace sf {
class RenderTarget;
}

namespace eng {

class Camera {
public:
    // view_size is how many WORLD pixels are visible (usually the window size).
    explicit Camera(const gmath::Vec2f& view_size);

    // Restrict the camera centre so the visible rectangle stays inside `world`.
    void set_bounds(const gmath::AABB& world) {
        bounds_ = world;
        has_bounds_ = true;
    }

    // Centre on `target`, then clamp to bounds (if set). Call every frame.
    void follow(const gmath::Vec2f& target);

    gmath::Vec2f center() const { return center_; }
    const sf::View& view() const { return view_; }

    // Make this camera the active view on the given target (window). After this,
    // everything drawn is transformed by the view until the view is changed.
    void apply(sf::RenderTarget& target) const;

private:
    sf::View view_;
    gmath::Vec2f view_size_;
    gmath::Vec2f center_;
    gmath::AABB bounds_;
    bool has_bounds_{false};
};

// The pure clamp math, factored out of the SFML plumbing: given a desired
// centre, the view size and the world bounds, where does the camera actually
// end up? If the world is smaller than the view on an axis, we centre the world
// on that axis instead of clamping. (Pure gmath — no SFML in the body.)
gmath::Vec2f clamp_camera_center(const gmath::Vec2f& target, const gmath::Vec2f& view_size,
                                 const gmath::AABB& bounds);

}  // namespace eng
