#include "eng/tile_renderer.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace eng {

void TileRenderer::build(const Tilemap& map) {
    vertices_.clear();
    vertices_.setPrimitiveType(sf::Quads);

    const float ts = static_cast<float>(map.tile_size());
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            const int id = map.tile_at(x, y);
            auto color_it = colors_.find(id);
            if (color_it == colors_.end()) {
                continue;  // no colour assigned -> treat as empty, draw nothing
            }
            const sf::Color color = color_it->second;

            // Four corners of this tile's quad, in world pixels.
            const float px = static_cast<float>(x) * ts;
            const float py = static_cast<float>(y) * ts;
            vertices_.append(sf::Vertex(sf::Vector2f(px, py), color));
            vertices_.append(sf::Vertex(sf::Vector2f(px + ts, py), color));
            vertices_.append(sf::Vertex(sf::Vector2f(px + ts, py + ts), color));
            vertices_.append(sf::Vertex(sf::Vector2f(px, py + ts), color));
        }
    }
}

void TileRenderer::render(sf::RenderTarget& target) const {
    target.draw(vertices_);
}

}  // namespace eng
