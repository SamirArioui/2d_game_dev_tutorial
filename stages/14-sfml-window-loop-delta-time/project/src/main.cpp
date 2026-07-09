// ============================================================================
// Stage 14 mini-project — a bouncing ball under a fixed-timestep accumulator.
//
// This is the first SFML program in the course, and it wires together every
// idea from the stage:
//   - open a window and pump its event queue,
//   - the UPDATE / RENDER split (advance the world, THEN draw it),
//   - SFML's y-down, top-left-origin coordinate system,
//   - DELTA TIME measured with sf::Clock,
//   - a FIXED-timestep accumulator with render interpolation,
//   - a runtime TOGGLE (Space) between fixed and variable timestep.
//
// All the actual motion/timestep math lives in the window-free sim.hpp so it can
// be unit-tested (see project/tests/). main() is only glue + drawing.
//
// NOTE: real acceleration/gravity is stage 15 and real collision is stage 16.
// Here the ball moves at constant velocity and merely reflects off the edges.
//
// Build & run:
//   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
//   cmake --build build
//   ./build/ball_demo
// ============================================================================

#include <SFML/Graphics.hpp>

#include <string>

#include "sim.hpp"

namespace {

// Window size in pixels. SFML's origin (0,0) is the TOP-LEFT corner and +y goes
// DOWN, so (0,0) is the top-left and (Width, Height) is the bottom-right.
constexpr unsigned int Width = 800;
constexpr unsigned int Height = 600;

// The fixed timestep: the simulation advances in slices of this many seconds.
// 1/120 s == 120 physics ticks per second, independent of the display rate.
constexpr float FixedDt = 1.0f / 120.0f;

// Never run more than this many fixed steps in one frame (spiral-of-death cap).
constexpr int MaxSteps = 8;

// Never trust a frame longer than this (a stall shouldn't teleport the ball).
constexpr float MaxFrameDt = 0.25f;

}  // namespace

int main() {
    // --- create the window -------------------------------------------------
    // sf::VideoMode(w, h) is the drawable size; the last arg is the title.
    sf::RenderWindow window(sf::VideoMode(Width, Height), "Stage 14 - Bouncing Ball");
    // Cap the display to the monitor's refresh so we don't spin at 5000 FPS.
    window.setVerticalSyncEnabled(true);

    // --- the ball ----------------------------------------------------------
    // Start near the top-left, moving right and DOWN (+y is down in SFML!).
    sim::Ball ball{gmath::Vec2f{120.0f, 90.0f}, gmath::Vec2f{260.0f, 200.0f}, 24.0f};
    sim::Ball previous = ball;  // last fixed state, kept for render interpolation

    // The SFML shape we actually draw. setOrigin(radius, radius) moves the
    // shape's local origin to its center, so setPosition() places the CENTER —
    // matching how sim::Ball stores position.
    sf::CircleShape shape(ball.radius);
    shape.setOrigin(ball.radius, ball.radius);
    shape.setFillColor(sf::Color(120, 200, 255));

    // --- loop state --------------------------------------------------------
    sf::Clock clock;             // measures real time between frames (delta time)
    float accumulator = 0.0f;    // banked, not-yet-simulated real time
    bool use_fixed = true;       // Space toggles fixed <-> variable timestep

    while (window.isOpen()) {
        // === 1. EVENTS =====================================================
        // Drain every pending event this frame. In SFML 2.6 you poll into an
        // sf::Event and inspect event.type. (This is the closest thing games
        // have to Python's event-driven callbacks — but you pull, not push.)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::Space) {
                    use_fixed = !use_fixed;  // flip the timestep mode live
                    accumulator = 0.0f;      // drop banked time when switching
                }
            }
        }

        // === 2. DELTA TIME =================================================
        // restart() returns the time since the last restart AND resets the
        // clock — exactly one "how long was the last frame?" per iteration.
        float frame_dt = clock.restart().asSeconds();
        frame_dt = sim::clamp_frame_dt(frame_dt, MaxFrameDt);

        // === 3. UPDATE =====================================================
        float alpha = 1.0f;  // render-blend factor (1.0 == draw the latest state)
        if (use_fixed) {
            // FIXED: bank the frame's time, then run whole 1/120 s slices. Each
            // slice is identical regardless of frame rate, so the simulation is
            // deterministic and stable.
            int steps = sim::steps_to_run(accumulator, frame_dt, FixedDt, MaxSteps);
            for (int i = 0; i < steps; ++i) {
                previous = ball;                 // remember the pre-step state
                sim::integrate(ball, FixedDt);
                sim::reflect_in_bounds(ball, static_cast<float>(Width),
                                       static_cast<float>(Height));
            }
            // Leftover time we haven't simulated yet, as a fraction of a step.
            // Drawing at lerp(previous, ball, alpha) hides the mismatch between
            // a 120 Hz sim and a 60 Hz screen — no visible stutter.
            alpha = accumulator / FixedDt;
        } else {
            // VARIABLE: feed the raw frame time straight in. Simpler, but the
            // result now depends on frame rate, and a big dt can skip through a
            // wall. Toggle with Space to feel the difference.
            previous = ball;
            sim::integrate(ball, frame_dt);
            sim::reflect_in_bounds(ball, static_cast<float>(Width),
                                   static_cast<float>(Height));
            alpha = 1.0f;
        }

        // === 4. RENDER =====================================================
        // Draw the interpolated position (in fixed mode) or the exact one.
        gmath::Vec2f draw_pos = sim::render_lerp(previous.pos, ball.pos, alpha);
        shape.setPosition(draw_pos.x, draw_pos.y);

        window.clear(sf::Color(30, 30, 46));  // dark background
        window.draw(shape);
        window.display();  // swap buffers — show what we just drew

        // Show the current mode and frame time in the title bar. (A real HUD
        // with sf::Text arrives in stage 17.)
        std::string mode = use_fixed ? "FIXED 1/120s" : "VARIABLE";
        window.setTitle("Stage 14 - Ball  [Space: timestep = " + mode +
                        "]  dt=" + std::to_string(frame_dt) + "s");
    }

    return 0;
}
