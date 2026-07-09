// Exercise 04 — solution  (SFML)
#include <SFML/Graphics.hpp>

#include <algorithm>  // std::clamp

int main() {
    sf::RenderWindow window(sf::VideoMode(320, 480), "Ex04 - input");
    window.setFramerateLimit(60);

    sf::RectangleShape paddle(sf::Vector2f(16.0f, 100.0f));
    paddle.setPosition(150.0f, 190.0f);
    const float speed = 400.0f;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const float dt = clock.restart().asSeconds();
        float y = paddle.getPosition().y;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            y -= speed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            y += speed * dt;
        }
        y = std::clamp(y, 0.0f, 480.0f - 100.0f);  // keep the paddle on screen

        paddle.setPosition(150.0f, y);

        window.clear(sf::Color(15, 15, 20));
        window.draw(paddle);
        window.display();
    }
    return 0;
}
