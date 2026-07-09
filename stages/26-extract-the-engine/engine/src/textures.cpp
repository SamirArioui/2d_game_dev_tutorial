#include "eng/textures.hpp"

#include <SFML/Graphics/Image.hpp>

namespace eng {

sf::Texture make_solid_texture(unsigned width, unsigned height, sf::Color color) {
    sf::Image image;
    image.create(width, height, color);  // fill every pixel with `color`
    sf::Texture texture;
    texture.loadFromImage(image);  // upload CPU pixels -> GPU texture
    return texture;
}

sf::Texture make_circle_texture(unsigned diameter, sf::Color color) {
    sf::Image image;
    image.create(diameter, diameter, sf::Color::Transparent);

    const float radius = diameter * 0.5f;
    const float cx = radius - 0.5f;
    const float cy = radius - 0.5f;
    for (unsigned y = 0; y < diameter; ++y) {
        for (unsigned x = 0; x < diameter; ++x) {
            const float dx = static_cast<float>(x) - cx;
            const float dy = static_cast<float>(y) - cy;
            // Inside the circle -> opaque colour; outside -> stays transparent.
            if (dx * dx + dy * dy <= radius * radius) {
                image.setPixel(x, y, color);
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

sf::Texture make_checker_texture(unsigned width, unsigned height, unsigned cell, sf::Color a,
                                 sf::Color b) {
    sf::Image image;
    image.create(width, height, a);
    const unsigned safe_cell = cell == 0 ? 1 : cell;
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            // Alternate colour every `cell` pixels in a checker pattern.
            const bool second = ((x / safe_cell) + (y / safe_cell)) % 2 == 1;
            image.setPixel(x, y, second ? b : a);
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

}  // namespace eng
