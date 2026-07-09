// Exercise 03 — Frame-rate-independent movement with delta time (solution)

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ex03 - delta time");
    window.setVerticalSyncEnabled(true);

    sf::RectangleShape box(sf::Vector2f(50.0f, 50.0f));
    box.setFillColor(sf::Color(255, 200, 120));
    box.setPosition(0.0f, 275.0f);

    const float speed = 200.0f;  // pixels PER SECOND, not per frame

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

        // restart() gives the time since the previous call AND resets the clock,
        // so this is exactly "how long did the last frame take?".
        float dt = clock.restart().asSeconds();

        float x = box.getPosition().x + speed * dt;  // move by px/sec * sec
        if (x > 800.0f) {
            x = -50.0f;  // wrap around; -50 = the box's width, so it slides in
        }
        box.setPosition(x, box.getPosition().y);

        window.clear(sf::Color(30, 30, 46));
        window.draw(box);
        window.display();
    }

    return 0;
}
