// ============================================================================
// Stage 22 mini-project — an explosion burst and a thruster trail, both driven
// by ONE pooled particle buffer.
//
// The interesting part — the pool and the emitters — is the pure, unit-tested
// ParticlePool. This file is the SFML layer: it owns the window, drives a ship
// with the keyboard, calls the emitters, updates the pool, and draws it in one
// vertex-array pass. A single fixed-capacity pool feeds BOTH effects.
//
// NOTE: the bar for this stage is BUILD + LINK. No display on the build
// machine, so the visuals are unverified-by-execution.
// ============================================================================

#include <SFML/Graphics.hpp>

#include <random>

#include "game/gmath.hpp"
#include "game/particle_pool.hpp"
#include "particle_render.hpp"

using gmath::Vec2f;

int main() {
    const unsigned win_w = 800;
    const unsigned win_h = 600;
    sf::RenderWindow window(sf::VideoMode(win_w, win_h), "Stage 22 - Particles");
    window.setFramerateLimit(60);

    // ONE pool, sized once, shared by the explosion and the trail. When it's
    // full, new spawns are simply dropped — the capacity is a hard budget.
    game::ParticlePool pool(4000);

    // The RNG the emitters jitter with (stage 01). One generator, threaded
    // through every emit call this frame.
    std::mt19937 rng(std::random_device{}());

    // The ship: a little triangle we fly with WASD; its trail streams behind it.
    Vec2f ship_pos{static_cast<float>(win_w) * 0.5f, static_cast<float>(win_h) * 0.5f};
    Vec2f ship_vel{0.0f, 0.0f};
    const float ship_speed = 240.0f;

    sf::CircleShape ship(12.0f, 3);  // a 3-point "circle" is a triangle
    ship.setOrigin(12.0f, 12.0f);
    ship.setFillColor(sf::Color(220, 220, 240));

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Left click -> an explosion burst at the cursor (screen == world
            // here, since we never move the view).
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                const Vec2f at{static_cast<float>(event.mouseButton.x),
                               static_cast<float>(event.mouseButton.y)};
                game::emit_burst(pool, at, 200, rng);
            }
        }

        const float dt = clock.restart().asSeconds();

        // --- drive the ship ------------------------------------------------
        ship_vel = Vec2f{0.0f, 0.0f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            ship_vel.x -= ship_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            ship_vel.x += ship_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            ship_vel.y -= ship_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            ship_vel.y += ship_speed;
        }
        ship_pos += ship_vel * dt;

        // Emit a thruster trail behind the ship while it's actually moving.
        if (gmath::length(ship_vel) > 0.1f) {
            game::emit_trail(pool, ship_pos, ship_vel, 6, rng);
        }

        // --- advance every particle ----------------------------------------
        pool.update(dt);

        // --- draw ----------------------------------------------------------
        window.clear(sf::Color(10, 10, 18));
        sf::VertexArray particles = game::build_particle_vertices(pool, 4.0f);
        window.draw(particles);
        ship.setPosition(ship_pos.x, ship_pos.y);
        window.draw(ship);
        window.display();
    }

    return 0;
}
