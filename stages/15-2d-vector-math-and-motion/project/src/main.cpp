// ============================================================================
// Stage 15 mini-project — an aimed projectile + an eased moving platform.
//
// Two things move, each showing off one half of the stage:
//
//   * A PROJECTILE launched from a fixed origin, AIMED at a target with
//     aim_velocity(origin, target, speed) == normalize(target - origin) * speed,
//     then falling under GRAVITY via semi-implicit Euler integration.
//     Left-click anywhere to re-aim and re-launch at the cursor.
//
//   * A PLATFORM that glides between two points using an EASED tween:
//     lerp(a, b, smoothstep(ping_pong(t))) — so it accelerates away, decelerates
//     in, pauses, and returns, forever.
//
// All the maths lives in the pure, tested headers vec2.hpp / motion.hpp. This
// file only reads input, calls those functions, and draws — converting to SFML
// coordinates through the ONE bridge point, gmath::to_sfml().
//
// Build & run:
//   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
//   cmake --build build
//   ./build/motion_demo
// ============================================================================

#include <SFML/Graphics.hpp>

#include "motion.hpp"
#include "vec2.hpp"
#include "vec2_sfml.hpp"

namespace {

constexpr unsigned int Width = 800;
constexpr unsigned int Height = 600;

// Gravity points DOWN, which is +y in SFML screen space. Units are pixels/s².
const gmath::Vec2f Gravity{0.0f, 900.0f};

// Where projectiles are fired from (bottom-left) and how fast they leave.
const gmath::Vec2f LaunchOrigin{60.0f, static_cast<float>(Height) - 60.0f};
constexpr float LaunchSpeed = 720.0f;

// Launch a fresh projectile from the origin, aimed at `target`.
motion::Body launch(const gmath::Vec2f& target) {
    return motion::Body{LaunchOrigin, motion::aim_velocity(LaunchOrigin, target, LaunchSpeed)};
}

// True once the projectile has clearly left the play area (so we relaunch it).
bool off_screen(const gmath::Vec2f& p) {
    return p.y > static_cast<float>(Height) + 50.0f || p.x < -50.0f ||
           p.x > static_cast<float>(Width) + 50.0f;
}

}  // namespace

int main() {
    sf::RenderWindow window(sf::VideoMode(Width, Height), "Stage 15 - Motion");
    window.setVerticalSyncEnabled(true);

    // --- the projectile ----------------------------------------------------
    gmath::Vec2f target{700.0f, 180.0f};   // default aim point (top-right)
    motion::Body projectile = launch(target);

    sf::CircleShape ball(10.0f);
    ball.setOrigin(10.0f, 10.0f);
    ball.setFillColor(sf::Color(255, 210, 120));

    // A little marker showing where we're aiming.
    sf::CircleShape target_marker(6.0f);
    target_marker.setOrigin(6.0f, 6.0f);
    target_marker.setFillColor(sf::Color(255, 90, 90));

    // --- the eased platform ------------------------------------------------
    const gmath::Vec2f platform_a{400.0f, 130.0f};
    const gmath::Vec2f platform_b{400.0f, 470.0f};
    sf::RectangleShape platform(sf::Vector2f(120.0f, 20.0f));
    platform.setOrigin(60.0f, 10.0f);
    platform.setFillColor(sf::Color(140, 220, 160));

    sf::Clock clock;
    float elapsed = 0.0f;  // total time, drives the platform tween

    while (window.isOpen()) {
        // === events ========================================================
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed &&
                       event.key.code == sf::Keyboard::Escape) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed &&
                       event.mouseButton.button == sf::Mouse::Left) {
                // Read the click in SFML coords and bring it into gmath via the
                // bridge, then re-aim and re-launch.
                target = gmath::from_sfml(sf::Vector2f(static_cast<float>(event.mouseButton.x),
                                                       static_cast<float>(event.mouseButton.y)));
                projectile = launch(target);
            }
        }

        // === update ========================================================
        float dt = clock.restart().asSeconds();
        if (dt > 0.05f) {
            dt = 0.05f;  // clamp a hitch so nothing tunnels (stage 14 habit)
        }
        elapsed += dt;

        // Projectile: gravity + semi-implicit Euler. Relaunch when it exits.
        motion::integrate_semi_implicit(projectile, Gravity, dt);
        if (off_screen(projectile.pos)) {
            projectile = launch(target);
        }

        // Platform: eased ping-pong tween between the two endpoints.
        float phase = motion::ping_pong(elapsed * 0.35f);  // 0->1->0 triangle
        float eased = motion::smoothstep(phase);           // gentle start/stop
        gmath::Vec2f platform_pos = gmath::lerp(platform_a, platform_b, eased);

        // === render ========================================================
        // to_sfml() is the ONLY place gmath meets SFML — the bridge in action.
        ball.setPosition(gmath::to_sfml(projectile.pos));
        target_marker.setPosition(gmath::to_sfml(target));
        platform.setPosition(gmath::to_sfml(platform_pos));

        window.clear(sf::Color(28, 28, 42));
        window.draw(platform);
        window.draw(target_marker);
        window.draw(ball);
        window.display();
    }

    return 0;
}
