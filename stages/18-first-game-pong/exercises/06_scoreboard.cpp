// Exercise 06 — a text scoreboard  (SFML; needs a display)
//
// Pong needs to show the score. That means sf::Font + sf::Text, plus turning
// numbers into a string. We reuse the graceful font-loading pattern from stage
// 17 so a missing font never crashes the program.
//
// Build:
//   cmake -S . -B build && cmake --build build --target 06_scoreboard
//   ./build/06_scoreboard      # LEFT/RIGHT arrows add a point to each side
//
// TODO:
//   1. In load_a_font(), return true on the first path that loads.
//   2. In the loop, update the score string as  "<left>   <right>"  using
//      std::to_string, and draw the text only when a font is available.

#include <SFML/Graphics.hpp>

#include <array>
#include <string>

bool load_a_font(sf::Font& font) {
    const std::array<const char*, 3> candidates{
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/arial.ttf",
    };
    // TODO: try each path; return true on the first success, else false.
    (void)font;
    (void)candidates;
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 160), "Ex06 - scoreboard");

    sf::Font font;
    const bool have_font = load_a_font(font);
    sf::Text text;
    if (have_font) {
        text.setFont(font);
        text.setCharacterSize(64);
        text.setFillColor(sf::Color::White);
        text.setPosition(120.0f, 40.0f);
    }

    int score_left = 0;
    int score_right = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    ++score_left;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    ++score_right;
                }
            }
        }

        // TODO: if have_font, set text to "<left>   <right>".
        (void)score_left;
        (void)score_right;

        window.clear(sf::Color(15, 15, 20));
        // TODO: if have_font, draw the text.
        (void)have_font;
        window.display();
    }
    return 0;
}
