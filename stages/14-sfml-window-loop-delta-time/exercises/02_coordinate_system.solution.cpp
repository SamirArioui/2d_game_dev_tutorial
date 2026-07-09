// Exercise 02 — SFML's coordinate system (solution)

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ex02 - coordinates");
    window.setVerticalSyncEnabled(true);

    // Position defaults to the shape's TOP-LEFT corner. (40, 40) is therefore
    // near the top-left of the window, because +y points DOWN.
    sf::CircleShape marker(15.0f);
    marker.setPosition(40.0f, 40.0f);
    marker.setFillColor(sf::Color(120, 255, 160));

    // setOrigin moves the shape's LOCAL origin. Setting it to half the size
    // makes setPosition() place the CENTRE instead of the corner — the idiom we
    // use everywhere so a shape's "position" matches a physics center.
    sf::RectangleShape center_box(sf::Vector2f(80.0f, 80.0f));
    center_box.setFillColor(sf::Color(200, 120, 255));
    center_box.setOrigin(40.0f, 40.0f);
    center_box.setPosition(400.0f, 300.0f);  // dead centre of an 800x600 window

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

        window.clear(sf::Color(30, 30, 46));
        window.draw(marker);      // appears top-left
        window.draw(center_box);  // appears centred
        window.display();
    }

    return 0;
}
