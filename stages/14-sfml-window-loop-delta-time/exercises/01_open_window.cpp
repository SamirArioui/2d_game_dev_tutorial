// Exercise 01 — Open a window and run the game loop (easy)
//
// Every SFML program is the same skeleton: create a window, then loop until it
// closes. Each iteration you (1) drain the event queue, (2) update, (3) draw.
// This exercise is just the skeleton — no moving parts yet.
//
// PYTHON NOTE: standard Python has no real-time loop. The closest feel is a
// `while True:` that pulls events and redraws — but here the OS, not you, owns
// the window, so you must poll its event queue every frame or the window
// appears frozen (the OS thinks your app hung).
//
// TODO:
//   1. Create an 800x600 window titled "Ex01".
//   2. In the loop, close the window on the Closed event AND on Escape.
//   3. Clear to a colour, then display() every frame.
//
// Build (from exercises/):
//   cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
//   cmake --build build --target 01_open_window
//   ./build/01_open_window

#include <SFML/Graphics.hpp>

int main() {
    // TODO 1: replace the 200x200 placeholder with an 800x600 window.
    sf::RenderWindow window(sf::VideoMode(200, 200), "Ex01");
    window.setVerticalSyncEnabled(true);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // TODO 2: also close when Escape is pressed.
            // Hint: event.type == sf::Event::KeyPressed and
            //       event.key.code == sf::Keyboard::Escape
        }

        // TODO 3: clear the window to a colour, then display() it.
        window.display();
    }

    return 0;
}
