// Exercise 04 — real-time keyboard input  (SFML; needs a display)
//
// Games read input two ways. EVENTS (window.pollEvent) are good for one-shot
// things ("the window was closed", "a key was just pressed"). But for smooth
// movement you want the CURRENT state of a key every frame — that's
// sf::Keyboard::isKeyPressed, which is not tied to the event queue at all.
//
// Build:
//   cmake -S . -B build && cmake --build build --target 04_keyboard_input
//   ./build/04_keyboard_input
//
// TODO:
//   In the loop, move the paddle with W (up) and S (down) at `speed` px/sec,
//   using isKeyPressed. Then clamp paddle.y to stay on screen [0, 480-height].

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(320, 480), "Ex04 - input");
    window.setFramerateLimit(60);

    sf::RectangleShape paddle(sf::Vector2f(16.0f, 100.0f));
    paddle.setPosition(150.0f, 190.0f);
    const float speed = 400.0f;  // px/sec

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

        // TODO: if W is pressed, y -= speed*dt; if S is pressed, y += speed*dt.
        // TODO: clamp y to [0, 480 - paddle height]  (height is 100).
        (void)dt;
        (void)speed;

        paddle.setPosition(150.0f, y);

        window.clear(sf::Color(15, 15, 20));
        window.draw(paddle);
        window.display();
    }
    return 0;
}
