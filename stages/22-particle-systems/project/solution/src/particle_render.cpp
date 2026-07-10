// ============================================================================
// particle_render implementation — fade-by-life vertex array.
// ============================================================================

#include "particle_render.hpp"

namespace game {

sf::VertexArray build_particle_vertices(const ParticlePool& pool, float size) {
    sf::VertexArray vertices(sf::Quads);
    const float half = size * 0.5f;

    for (const Particle& p : pool.particles()) {
        if (!p.alive) {
            continue;  // dead slots contribute nothing
        }

        // Fade out over the particle's life: alpha 255 when fresh -> 0 at death.
        float ratio = (p.max_life > 0.0f) ? (p.life / p.max_life) : 0.0f;
        if (ratio < 0.0f) {
            ratio = 0.0f;
        }
        if (ratio > 1.0f) {
            ratio = 1.0f;
        }
        const auto alpha = static_cast<sf::Uint8>(ratio * 255.0f);
        const sf::Color color(p.r, p.g, p.b, alpha);

        const float x = p.position.x;
        const float y = p.position.y;

        // Four corners of the little square, all sharing the faded colour.
        vertices.append(sf::Vertex({x - half, y - half}, color));
        vertices.append(sf::Vertex({x + half, y - half}, color));
        vertices.append(sf::Vertex({x + half, y + half}, color));
        vertices.append(sf::Vertex({x - half, y + half}, color));
    }

    return vertices;
}

}  // namespace game
