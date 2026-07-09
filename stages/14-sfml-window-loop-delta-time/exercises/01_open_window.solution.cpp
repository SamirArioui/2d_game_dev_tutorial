// Exercise 01 — Open a window and run the game loop (solution)

#include <SFML/Graphics.hpp>

int main() {
    // sf::VideoMode(width, height) is the drawable area, in pixels.
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ex01");
    window.setVerticalSyncEnabled(true);  // don't render faster than the monitor

    while (window.isOpen()) {
        // Drain EVERY pending event this frame, or the OS thinks we hung.
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // the user clicked the window's X
            } else if (event.type == sf::Event::KeyPressed &&
                       event.key.code == sf::Keyboard::Escape) {
                window.close();  // Escape quits too
            }
        }

        // The classic three-step frame: clear -> draw -> display.
        window.clear(sf::Color(30, 30, 46));  // wipe last frame to a dark colour
        // (nothing to draw yet)
        window.display();                      // swap buffers, show the frame
    }

    return 0;
}
