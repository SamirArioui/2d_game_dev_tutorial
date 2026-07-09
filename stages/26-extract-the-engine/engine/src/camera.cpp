#include "eng/camera.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace eng {

Camera::Camera(const gmath::Vec2f& view_size)
    : view_(sf::FloatRect(0.0f, 0.0f, view_size.x, view_size.y)), view_size_(view_size),
      center_(view_size.x * 0.5f, view_size.y * 0.5f) {
    view_.setCenter(center_.x, center_.y);
}

gmath::Vec2f clamp_camera_center(const gmath::Vec2f& target, const gmath::Vec2f& view_size,
                                 const gmath::AABB& bounds) {
    const float half_w = view_size.x * 0.5f;
    const float half_h = view_size.y * 0.5f;

    gmath::Vec2f result = target;

    // If the world is narrower than the view, there is nothing to scroll to on
    // that axis — just centre the world. Otherwise clamp so neither view edge
    // crosses the world edge.
    if (bounds.size.x <= view_size.x) {
        result.x = bounds.center().x;
    } else {
        result.x = gmath::clamp(target.x, bounds.left() + half_w, bounds.right() - half_w);
    }
    if (bounds.size.y <= view_size.y) {
        result.y = bounds.center().y;
    } else {
        result.y = gmath::clamp(target.y, bounds.top() + half_h, bounds.bottom() - half_h);
    }
    return result;
}

void Camera::follow(const gmath::Vec2f& target) {
    center_ = has_bounds_ ? clamp_camera_center(target, view_size_, bounds_) : target;
    view_.setCenter(center_.x, center_.y);
}

void Camera::apply(sf::RenderTarget& target) const {
    target.setView(view_);
}

}  // namespace eng
