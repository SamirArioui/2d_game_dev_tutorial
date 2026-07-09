// ============================================================================
// Stage 16 mini-project — mixed balls & rectangles bouncing inside a box.
//
// A handful of circles and axis-aligned boxes drift around the window. Every
// frame we:
//   1. move each entity (constant velocity * dt),
//   2. bounce each off the four container WALLS,
//   3. test every PAIR with the right predicate (circle/circle, aabb/aabb,
//      circle/aabb), and when they overlap, SEPARATE them (positional
//      correction by the penetration vector) and BOUNCE both velocities.
//
// Detection and response are the tested, window-free functions in collision.hpp
// (see project/tests/). This file is only the glue + drawing. The AABB-vs-AABB
// wall/paddle case here is exactly what Pong needs in stage 18.
//
// Build & run:
//   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
//   cmake --build build
//   ./build/collision_demo
// ============================================================================

#include <SFML/Graphics.hpp>

#include <optional>
#include <vector>

#include "collision.hpp"

namespace {

constexpr unsigned int Width = 800;
constexpr unsigned int Height = 600;
constexpr float Restitution = 1.0f;  // 1 = perfectly elastic; they bounce forever

// One movable thing. `pos` is the CENTER for both shapes; `extent` is the half-
// size (a circle uses extent.x as its radius). Storing a center for both makes
// movement and wall handling uniform.
struct Entity {
    bool is_circle;
    gmath::Vec2f pos;
    gmath::Vec2f vel;
    gmath::Vec2f extent;
    sf::Color color;
};

collide::Circle as_circle(const Entity& e) {
    return collide::Circle{e.pos, e.extent.x};
}

collide::AABB as_aabb(const Entity& e) {
    // AABB stores the TOP-LEFT corner, so shift the center by the half-size.
    return collide::AABB{e.pos - e.extent, e.extent * 2.0f};
}

// Dispatch to the right predicate. The returned penetration always moves `a`.
std::optional<collide::Hit> collide_pair(const Entity& a, const Entity& b) {
    if (a.is_circle && b.is_circle) {
        return collide::circle_vs_circle(as_circle(a), as_circle(b));
    }
    if (!a.is_circle && !b.is_circle) {
        return collide::aabb_vs_aabb(as_aabb(a), as_aabb(b));
    }
    if (a.is_circle) {  // a = circle, b = box: penetration already moves the circle (a)
        return collide::circle_vs_aabb(as_circle(a), as_aabb(b));
    }
    // a = box, b = circle: predicate moves the circle (b), so negate to move a.
    auto hit = collide::circle_vs_aabb(as_circle(b), as_aabb(a));
    if (hit) {
        hit->penetration = hit->penetration * -1.0f;
    }
    return hit;
}

// Bounce an entity off the container walls (inner box [0,W] x [0,H]).
void bounce_off_walls(Entity& e, float w, float h) {
    if (e.pos.x - e.extent.x < 0.0f) {
        e.pos.x = e.extent.x;
        e.vel = collide::reflect_velocity(e.vel, gmath::Vec2f{1.0f, 0.0f}, Restitution);
    } else if (e.pos.x + e.extent.x > w) {
        e.pos.x = w - e.extent.x;
        e.vel = collide::reflect_velocity(e.vel, gmath::Vec2f{-1.0f, 0.0f}, Restitution);
    }
    if (e.pos.y - e.extent.y < 0.0f) {
        e.pos.y = e.extent.y;
        e.vel = collide::reflect_velocity(e.vel, gmath::Vec2f{0.0f, 1.0f}, Restitution);
    } else if (e.pos.y + e.extent.y > h) {
        e.pos.y = h - e.extent.y;
        e.vel = collide::reflect_velocity(e.vel, gmath::Vec2f{0.0f, -1.0f}, Restitution);
    }
}

// Resolve a collision between two equal-"mass" entities: split them apart along
// the penetration vector and reflect each velocity about the contact normal.
// (A physically exact two-body response would weigh this by mass and use a
// single shared impulse — see the README's further-reading note.)
void resolve_pair(Entity& a, Entity& b, const collide::Hit& hit) {
    const gmath::Vec2f normal = gmath::normalize(hit.penetration);  // B -> A
    a.pos = a.pos + hit.penetration * 0.5f;   // push A out halfway ...
    b.pos = b.pos - hit.penetration * 0.5f;   // ... and B the other half
    a.vel = collide::reflect_velocity(a.vel, normal, Restitution);
    b.vel = collide::reflect_velocity(b.vel, normal * -1.0f, Restitution);
}

}  // namespace

int main() {
    sf::RenderWindow window(sf::VideoMode(Width, Height), "Stage 16 - Collisions");
    window.setVerticalSyncEnabled(true);

    // A mix of balls and boxes with assorted starting velocities.
    std::vector<Entity> entities = {
        {true, {150.0f, 150.0f}, {220.0f, 170.0f}, {28.0f, 28.0f}, sf::Color(120, 200, 255)},
        {true, {600.0f, 400.0f}, {-180.0f, 210.0f}, {40.0f, 40.0f}, sf::Color(255, 210, 120)},
        {true, {400.0f, 300.0f}, {160.0f, -240.0f}, {20.0f, 20.0f}, sf::Color(200, 140, 255)},
        {false, {300.0f, 480.0f}, {200.0f, -150.0f}, {35.0f, 25.0f}, sf::Color(140, 220, 160)},
        {false, {650.0f, 120.0f}, {-140.0f, 190.0f}, {30.0f, 45.0f}, sf::Color(255, 140, 150)},
    };

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed &&
                       event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) {
            dt = 0.05f;  // clamp a hitch so nothing tunnels through a wall
        }

        // 1. move + 2. wall bounce
        for (Entity& e : entities) {
            e.pos = e.pos + e.vel * dt;
            bounce_off_walls(e, static_cast<float>(Width), static_cast<float>(Height));
        }

        // 3. pairwise detection + response (every unordered pair once)
        for (std::size_t i = 0; i < entities.size(); ++i) {
            for (std::size_t j = i + 1; j < entities.size(); ++j) {
                if (auto hit = collide_pair(entities[i], entities[j])) {
                    resolve_pair(entities[i], entities[j], *hit);
                }
            }
        }

        // render
        window.clear(sf::Color(24, 24, 36));
        for (const Entity& e : entities) {
            if (e.is_circle) {
                sf::CircleShape shape(e.extent.x);
                shape.setOrigin(e.extent.x, e.extent.x);
                shape.setPosition(e.pos.x, e.pos.y);
                shape.setFillColor(e.color);
                window.draw(shape);
            } else {
                sf::RectangleShape shape(sf::Vector2f(e.extent.x * 2.0f, e.extent.y * 2.0f));
                shape.setOrigin(e.extent.x, e.extent.y);
                shape.setPosition(e.pos.x, e.pos.y);
                shape.setFillColor(e.color);
                window.draw(shape);
            }
        }
        window.display();
    }

    return 0;
}
