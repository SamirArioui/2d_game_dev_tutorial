// Exercise 06 — solution  (SFML)
#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>
#include <string>

bool load_a_font(sf::Font& font) {
    const std::array<const char*, 3> candidates{
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:/Windows/Fonts/arial.ttf",
    };
    for (const char* path : candidates) {
        if (font.loadFromFile(path)) {
            return true;
        }
    }
    std::cerr << "WARNING: no font found; scoreboard hidden.\n";
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

        if (have_font) {
            text.setString(std::to_string(score_left) + "   " + std::to_string(score_right));
        }

        window.clear(sf::Color(15, 15, 20));
        if (have_font) {
            window.draw(text);
        }
        window.display();
    }
    return 0;
}
