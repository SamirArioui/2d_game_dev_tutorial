// Exercise 05 — draw text, and fail gracefully on a missing font  (SFML)
//
// sf::Text needs an sf::Font, and a font is loaded from a .ttf file at runtime.
// The pitfall: if the file is not where you think (the working-directory trap),
// loadFromFile() returns false and — if you ignore that — you draw with a font
// that was never loaded. Always CHECK the bool and degrade gracefully; never
// hard-crash on a missing asset.
//
// Build (needs a display to run):
//   cmake -S . -B build && cmake --build build --target 05_text_label
//   ./build/05_text_label
//
// TODO:
//   1. In load_a_font(), loop over the candidate paths and return true on the
//      first font.loadFromFile(path) that succeeds; print a warning and return
//      false if none work.
//   2. In main(), only setFont/draw the text when a font actually loaded.

#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>

bool load_a_font(sf::Font& font) {
    const std::array<const char*, 3> candidates{
        "/System/Library/Fonts/Supplemental/Arial.ttf",     // macOS
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",  // Linux
        "C:/Windows/Fonts/arial.ttf",                       // Windows
    };
    // TODO: try each path; return true on first success, else warn + return false.
    (void)font;
    (void)candidates;
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 120), "Ex05 - text");

    sf::Font font;
    const bool have_font = load_a_font(font);

    sf::Text text;
    // TODO: if have_font, set the font, size, color, string, and position.
    (void)text;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(20, 20, 30));
        // TODO: if have_font, window.draw(text);
        (void)have_font;
        window.display();
    }
    return 0;
}
