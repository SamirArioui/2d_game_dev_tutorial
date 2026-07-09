#pragma once

// ============================================================================
// particle_render — the SFML half of stage 22. Turns the pure pool into pixels.
//
// Kept separate from ParticlePool for the usual reason: it pulls in SFML and
// can't be unit-tested without a display, so it stays out of the tested logic.
// ============================================================================

#include <SFML/Graphics.hpp>

#include "game/particle_pool.hpp"

namespace game {

// Build ONE sf::VertexArray of small quads for every ALIVE particle — the same
// "one draw call, not N sprites" idea as the tilemap. Each particle becomes a
// `size`-pixel square whose colour is its (r,g,b) with an ALPHA that fades with
// its remaining life (life / max_life), so particles gently disappear.
sf::VertexArray build_particle_vertices(const ParticlePool& pool, float size);

}  // namespace game
