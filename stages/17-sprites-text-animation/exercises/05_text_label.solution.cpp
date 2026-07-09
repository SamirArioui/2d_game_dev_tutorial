// Exercise 05 — solution  (SFML)
#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>

bool load_a_font(sf::Font& font) {
    const std::array<const char*, 3> candidates{
        "/System/Library/Fonts/Supplemental/Arial.ttf",     // macOS
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",  // Linux
        "C:/Windows/Fonts/arial.ttf",                       // Windows
    };
    for (const char* path : candidates) {
        if (font.loadFromFile(path)) {
            std::cout << "Loaded font: " << path << '\n';
            return true;
        }
    }
    std::cerr << "WARNING: no font found; running without a label.\n";
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 120), "Ex05 - text");

    sf::Font font;
    const bool have_font = load_a_font(font);

    sf::Text text;
    if (have_font) {
        text.setFont(font);
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::White);
        text.setString("Hello, sf::Text!");
        text.setPosition(20.0f, 40.0f);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color(20, 20, 30));
        if (have_font) {
            window.draw(text);
        }
        window.display();
    }
    return 0;
}
