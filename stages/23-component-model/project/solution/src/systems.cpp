// ===========================================================================
// Implementation of the pure component systems. See systems.hpp for the "why".
// ===========================================================================

#include "game/systems.hpp"

#include "ecs/component.hpp"

namespace game {

void movement_system(std::vector<ecs::Entity>& entities, float dt) {
    for (auto& entity : entities) {
        // The system's "query": it only acts on entities that have BOTH pieces of data it needs.
        auto* transform = entity.try_get<ecs::Transform>();
        auto* velocity = entity.try_get<ecs::Velocity>();
        if (transform != nullptr && velocity != nullptr) {
            transform->pos += velocity->vel * dt;
        }
    }
}

void bounce_in_bounds(std::vector<ecs::Entity>& entities, const gmath::Vec2f& bounds) {
    for (auto& entity : entities) {
        auto* transform = entity.try_get<ecs::Transform>();
        auto* velocity = entity.try_get<ecs::Velocity>();
        auto* shape = entity.try_get<ecs::Shape>();
        if (transform == nullptr || velocity == nullptr || shape == nullptr) {
            continue;  // needs a place, a speed, AND a size to bounce
        }

        // Left / right walls: clamp back inside and flip horizontal velocity.
        if (transform->pos.x < 0.0f) {
            transform->pos.x = 0.0f;
            velocity->vel.x = -velocity->vel.x;
        } else if (transform->pos.x + shape->size.x > bounds.x) {
            transform->pos.x = bounds.x - shape->size.x;
            velocity->vel.x = -velocity->vel.x;
        }

        // Top / bottom walls: same idea, vertical axis.
        if (transform->pos.y < 0.0f) {
            transform->pos.y = 0.0f;
            velocity->vel.y = -velocity->vel.y;
        } else if (transform->pos.y + shape->size.y > bounds.y) {
            transform->pos.y = bounds.y - shape->size.y;
            velocity->vel.y = -velocity->vel.y;
        }
    }
}

bool aabb_overlap(const gmath::Vec2f& a_pos, const gmath::Vec2f& a_size, const gmath::Vec2f& b_pos,
                  const gmath::Vec2f& b_size) {
    // Two axis-aligned boxes overlap iff they overlap on BOTH axes. On each axis, box A's near
    // edge must be before box B's far edge and vice-versa.
    const bool overlap_x = a_pos.x < b_pos.x + b_size.x && b_pos.x < a_pos.x + a_size.x;
    const bool overlap_y = a_pos.y < b_pos.y + b_size.y && b_pos.y < a_pos.y + a_size.y;
    return overlap_x && overlap_y;
}

}  // namespace game
