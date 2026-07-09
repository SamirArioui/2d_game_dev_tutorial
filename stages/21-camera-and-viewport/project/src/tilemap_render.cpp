// ============================================================================
// tilemap_render implementation — procedural tileset + vertex-array builder.
// ============================================================================

#include "tilemap_render.hpp"

#include <array>

namespace game {

sf::Texture make_tileset_texture(int tile_size, int tile_count) {
    // A palette to colour each tile id. Index 0 is the "empty" colour (never
    // actually drawn), 1.. are the solid tiles.
    const std::array<sf::Color, 4> palette = {
        sf::Color(30, 30, 40),     // 0: empty / background (unused when drawing)
        sf::Color(70, 140, 70),    // 1: grass wall
        sf::Color(120, 110, 100),  // 2: stone
        sf::Color(150, 110, 60),   // 3: dirt
    };

    // Draw into an sf::Image (CPU-side pixels), then upload to a Texture (GPU).
    sf::Image image;
    image.create(static_cast<unsigned>(tile_size * tile_count),
                 static_cast<unsigned>(tile_size), sf::Color::Transparent);

    for (int id = 0; id < tile_count; ++id) {
        const sf::Color fill = palette[static_cast<std::size_t>(id) % palette.size()];
        const int ox = id * tile_size;  // this cell's left edge in the strip
        for (int y = 0; y < tile_size; ++y) {
            for (int x = 0; x < tile_size; ++x) {
                // A 1px darker border makes the grid readable at a glance.
                const bool border = (x == 0 || y == 0 || x == tile_size - 1 || y == tile_size - 1);
                const sf::Color c = border ? sf::Color(fill.r / 2, fill.g / 2, fill.b / 2) : fill;
                image.setPixel(static_cast<unsigned>(ox + x), static_cast<unsigned>(y), c);
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

sf::VertexArray build_tilemap_vertices(const Tilemap& map, int tile_count) {
    // sf::Quads: every 4 consecutive vertices describe one textured rectangle.
    sf::VertexArray vertices(sf::Quads);
    const float ts = static_cast<float>(map.tile_size());

    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            const int id = map.tile_at(x, y);
            if (id == 0) {
                continue;  // empty tiles cost nothing — we simply don't emit them
            }

            // World-space corners of this cell.
            const float px = static_cast<float>(x) * ts;
            const float py = static_cast<float>(y) * ts;

            // Texture-space corners: the id's cell inside the tileset strip.
            const int clamped = id % tile_count;  // stay inside the strip
            const float tx = static_cast<float>(clamped) * ts;

            sf::Vertex tl, tr, br, bl;
            tl.position = {px, py};
            tr.position = {px + ts, py};
            br.position = {px + ts, py + ts};
            bl.position = {px, py + ts};

            tl.texCoords = {tx, 0.0f};
            tr.texCoords = {tx + ts, 0.0f};
            br.texCoords = {tx + ts, ts};
            bl.texCoords = {tx, ts};

            // Quads are wound TL -> TR -> BR -> BL.
            vertices.append(tl);
            vertices.append(tr);
            vertices.append(br);
            vertices.append(bl);
        }
    }

    return vertices;
}

}  // namespace game
