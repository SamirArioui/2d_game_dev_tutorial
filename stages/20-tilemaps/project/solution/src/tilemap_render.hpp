#pragma once

// ============================================================================
// tilemap_render — the SFML half of stage 20 (this is where the window lives).
//
// Kept in its OWN translation unit, separate from the pure Tilemap, for the
// reason the course keeps hammering: rendering pulls in SFML and can't be
// unit-tested without a display, so we quarantine it away from the logic.
// ============================================================================

#include <SFML/Graphics.hpp>

#include "game/tilemap.hpp"

namespace game {

// Generate a tiny tileset TEXTURE procedurally (no asset files on disk, per the
// stage's rule). It's a horizontal strip of `tile_size` x `tile_size` cells:
//   cell 0 -> a dark, flat "floor" look (used for empty id 0, though we skip
//             drawing those), cells 1.. -> distinct solid colours with a border.
// Returns the texture by value; the caller owns it and must outlive any sprite
// or vertex array that references it.
sf::Texture make_tileset_texture(int tile_size, int tile_count);

// Build ONE sf::VertexArray of quads for the whole map — the efficient way to
// draw a tilemap. Instead of N separate sprites (N draw calls), we push 4
// vertices per non-empty tile into a single array and draw it in ONE call, with
// each quad's texture coordinates pointing at its id's cell in the tileset.
sf::VertexArray build_tilemap_vertices(const Tilemap& map, int tile_count);

}  // namespace game
