#pragma once

// ============================================================================
// eng::TileRenderer — draw a Tilemap as one batched vertex array.
//
// The Tilemap itself is pure data (tilemap.hpp). Rendering is kept separate so
// the data stays testable. A naive renderer would draw one sf::RectangleShape
// per tile — hundreds of draw calls. Instead we build ONE sf::VertexArray of
// quads (four vertices per visible tile) and draw it in a single call. That is
// the standard tilemap-rendering trick.
//
// Colours are assigned per tile id in code (procedural — no tileset image), so
// this ships no binary assets.
// ============================================================================

#include <unordered_map>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "eng/tilemap.hpp"

namespace sf {
class RenderTarget;
}

namespace eng {

class TileRenderer {
public:
    // Assign a colour to a tile id. Ids with no colour set (e.g. floor id 0) are
    // treated as empty and skipped, letting the background show through.
    void set_color(int tile_id, sf::Color color) { colors_[tile_id] = color; }

    // (Re)build the vertex array from the map. Call once after loading, or again
    // if the map changes. Cheap enough to call rarely, not every frame.
    void build(const Tilemap& map);

    void render(sf::RenderTarget& target) const;

private:
    sf::VertexArray vertices_{sf::Quads};
    std::unordered_map<int, sf::Color> colors_;
};

}  // namespace eng
