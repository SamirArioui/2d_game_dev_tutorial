// Exercise 02 — SFML's coordinate system (easy-medium)
//
// SFML's screen space is NOT the maths classroom. The origin (0,0) is the
// TOP-LEFT corner, +x goes right (as usual) but +y goes DOWN. So a bigger y is
// LOWER on screen. This trips up everyone once; drawing a few shapes at known
// spots makes it stick.
//
// PYTHON NOTE: same convention as most GUI/image libraries (tkinter, PIL,
// pygame): row 0 is the top. Unlike a matplotlib plot, where y grows upward.
//
// TODO:
//   1. Place `marker` at the TOP-LEFT area (small x, small y) and give it a
//      colour — confirm it draws near the top-left, not the bottom-left.
//   2. Use setOrigin so a shape's position refers to its CENTRE, not its corner.
//   3. Put `center_box` in the exact middle of an 800x600 window.
//
// Build:
//   cmake --build build --target 02_coordinate_system && ./build/02_coordinate_system

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ex02 - coordinates");
    window.setVerticalSyncEnabled(true);

    // A small marker. By default a shape's position is its TOP-LEFT corner.
    sf::CircleShape marker(15.0f);
    // TODO 1: set its position near the top-left (e.g. 40, 40) and a fill colour.

    // A box we want centred on its own position.
    sf::RectangleShape center_box(sf::Vector2f(80.0f, 80.0f));
    center_box.setFillColor(sf::Color(200, 120, 255));
    // TODO 2: setOrigin to half its size (40, 40) so position == its centre.
    // TODO 3: setPosition to the window centre (400, 300).

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
        window.draw(marker);
        window.draw(center_box);
        window.display();
    }

    return 0;
}
