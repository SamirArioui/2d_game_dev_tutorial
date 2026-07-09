// Exercise 04 — solution  (SFML)
#include <SFML/Graphics.hpp>

#include <iostream>

sf::Image make_checkerboard(unsigned size, unsigned tile, sf::Color light) {
    sf::Image image;
    image.create(size, size, sf::Color::Black);
    for (unsigned y = 0; y < size; ++y) {
        for (unsigned x = 0; x < size; ++x) {
            // Alternate light/dark per tile: the parity of (tileX + tileY).
            if (((x / tile) + (y / tile)) % 2 == 0) {
                image.setPixel(x, y, light);
            }
        }
    }
    return image;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(256, 256), "Ex04 - procedural texture");

    sf::Texture texture;
    sf::Image board = make_checkerboard(64, 8, sf::Color(200, 200, 60));
    if (!texture.loadFromImage(board)) {
        std::cerr << "Failed to upload image to texture\n";
        return 1;
    }

    sf::Sprite sprite(texture);
    sprite.setScale(4.0f, 4.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
    return 0;
}
