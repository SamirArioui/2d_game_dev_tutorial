// Exercise 03 — Frame-rate-independent movement with delta time (medium)
//
// The #1 beginner game bug: moving something by a fixed number of pixels PER
// FRAME. On a 144 Hz monitor it then flies twice as fast as on 72 Hz. The fix
// is DELTA TIME: measure how long the last frame took, and move by
// speed(px/sec) * dt(sec). Distance per real second is then the same everywhere.
//
// PYTHON NOTE: like multiplying by `time.perf_counter()` deltas in a loop, but
// here it is THE core idiom, not an afterthought — every moving thing uses dt.
//
// TODO:
//   1. Each frame, measure dt with clock.restart().asSeconds().
//   2. Move `box` by speed * dt (NOT a constant per-frame amount).
//   3. Wrap it back to the left edge when it leaves the right side.
//
// Build:
//   cmake --build build --target 03_frame_rate_independent
//   ./build/03_frame_rate_independent

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ex03 - delta time");
    window.setVerticalSyncEnabled(true);

    sf::RectangleShape box(sf::Vector2f(50.0f, 50.0f));
    box.setFillColor(sf::Color(255, 200, 120));
    box.setPosition(0.0f, 275.0f);

    const float speed = 200.0f;  // pixels PER SECOND

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

        // TODO 1: get this frame's delta time in seconds.
        float dt = 0.0f;  // <-- replace with clock.restart().asSeconds();

        // TODO 2: advance x by speed * dt (frame-rate independent).
        // TODO 3: if x is past 800, wrap back to -50 (just off the left edge).
        (void)dt;
        (void)speed;

        window.clear(sf::Color(30, 30, 46));
        window.draw(box);
        window.display();
    }

    return 0;
}
