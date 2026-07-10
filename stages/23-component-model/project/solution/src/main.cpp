// ============================================================================
// Stage 23 mini-project — Pong expressed as entities + components + systems.
//                                                          (REFERENCE SOLUTION)
//
// Complete reference. The starter you implement is two levels up (../../src/systems.cpp and
// ../../include/ecs/entity.hpp). Build this standalone (it has its own CMakeLists.txt):
//   cmake -S project/solution -B build && cmake --build build && ./build/pong_ecs
//
// The same Pong you'd build with classes, but rebuilt the component way:
//   - two paddles and a ball are just `ecs::Entity` objects,
//   - each carries plain-data components (Transform, Velocity, Shape),
//   - behaviour lives in SYSTEMS (movement_system, bounce_in_bounds, aabb_overlap) that walk the
//     entity list — see project/src/systems.cpp, which is unit-tested with no window in sight.
//
// This file is the ONLY part that touches SFML: it reads input, calls the systems, and draws a
// rectangle per entity. Because CI here has no display, this is verified to BUILD and LINK; its
// on-screen behaviour is unverified-by-execution.
//
// Build & run:
//   cmake -S . -B build && cmake --build build
//   ./build/pong_ecs
// ============================================================================

#include <vector>

#include <SFML/Graphics.hpp>

#include "ecs/component.hpp"
#include "ecs/entity.hpp"
#include "game/systems.hpp"
#include "gmath/vec2.hpp"
#include "gmath/vec2_sfml.hpp"

namespace {

constexpr float WindowWidth = 640.0f;
constexpr float WindowHeight = 480.0f;
constexpr float PaddleSpeed = 400.0f;

// Small helper: build a paddle entity at a given x, vertically centred.
ecs::Entity make_paddle(float x) {
    ecs::Entity paddle;
    const gmath::Vec2f size{16.0f, 96.0f};
    paddle.add(ecs::Transform{gmath::Vec2f{x, WindowHeight / 2.0f - size.y / 2.0f}});
    paddle.add(ecs::Shape{size, 255, 255, 255, 255});
    // Paddles have no Velocity component: they move under player input, not the movement system.
    return paddle;
}

ecs::Entity make_ball() {
    ecs::Entity ball;
    const gmath::Vec2f size{14.0f, 14.0f};
    ball.add(ecs::Transform{gmath::Vec2f{WindowWidth / 2.0f, WindowHeight / 2.0f}});
    ball.add(ecs::Velocity{gmath::Vec2f{260.0f, 180.0f}});
    ball.add(ecs::Shape{size, 255, 220, 80, 255});
    return ball;
}

}  // namespace

int main() {
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned>(WindowWidth),
                                          static_cast<unsigned>(WindowHeight)),
                            "Stage 23 — Pong as components");
    window.setFramerateLimit(60);

    // The whole game world is a flat list of entities. Indices: 0 = left paddle, 1 = right, 2 = ball.
    std::vector<ecs::Entity> entities;
    entities.push_back(make_paddle(24.0f));
    entities.push_back(make_paddle(WindowWidth - 24.0f - 16.0f));
    entities.push_back(make_ball());

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const float dt = clock.restart().asSeconds();

        // --- input: nudge the paddles directly (real input mapping arrives in stage 24) --------
        auto& left = entities[0].get<ecs::Transform>();
        auto& right = entities[1].get<ecs::Transform>();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            left.pos.y -= PaddleSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            left.pos.y += PaddleSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            right.pos.y -= PaddleSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            right.pos.y += PaddleSpeed * dt;
        }

        // --- systems: exactly the code the tests exercise -------------------------------------
        game::movement_system(entities, dt);
        game::bounce_in_bounds(entities, gmath::Vec2f{WindowWidth, WindowHeight});

        // Ball vs. each paddle: if they overlap, reverse the ball's horizontal velocity.
        auto& ball_t = entities[2].get<ecs::Transform>();
        auto& ball_v = entities[2].get<ecs::Velocity>();
        const auto& ball_s = entities[2].get<ecs::Shape>();
        for (int i = 0; i < 2; ++i) {
            const auto& p_t = entities[i].get<ecs::Transform>();
            const auto& p_s = entities[i].get<ecs::Shape>();
            if (game::aabb_overlap(ball_t.pos, ball_s.size, p_t.pos, p_s.size)) {
                ball_v.vel.x = -ball_v.vel.x;
            }
        }

        // --- render: one system, expressed inline — draw a rectangle per drawable entity ------
        window.clear(sf::Color(20, 20, 30));
        for (const auto& entity : entities) {
            const auto* transform = entity.try_get<ecs::Transform>();
            const auto* shape = entity.try_get<ecs::Shape>();
            if (transform == nullptr || shape == nullptr) {
                continue;
            }
            sf::RectangleShape rect(gmath::to_sfml(shape->size));
            rect.setPosition(gmath::to_sfml(transform->pos));
            rect.setFillColor(sf::Color(shape->r, shape->g, shape->b, shape->a));
            window.draw(rect);
        }
        window.display();
    }

    return 0;
}
