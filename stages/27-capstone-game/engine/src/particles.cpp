#include "eng/particles.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace eng {

namespace {

// Blend two RGBA colours by t in [0,1] (t=0 -> a, t=1 -> b). Used to fade a
// particle from its bright core colour to its transparent end colour over life.
sf::Color blend(const Rgba& a, const Rgba& b, float t) {
    const auto mix = [t](unsigned char lo, unsigned char hi) {
        return static_cast<sf::Uint8>(gmath::lerp(lo, hi, t));
    };
    return sf::Color{mix(a.r, b.r), mix(a.g, b.g), mix(a.b, b.b), mix(a.a, b.a)};
}

}  // namespace

void ParticleSystem::render(sf::RenderTarget& target) const {
    // One draw call for the whole system: pack every live particle into a single
    // vertex array of points. Batching like this is why a pool of hundreds of
    // particles still costs almost nothing to draw.
    sf::VertexArray points(sf::Points);
    for (const Particle& p : pool_) {
        if (!p.alive()) {
            continue;
        }
        // fade: 0 when just spawned, 1 at end of life.
        const float t = 1.0f - (p.life / p.max_life);
        points.append(sf::Vertex(sf::Vector2f(p.pos.x, p.pos.y), blend(start_, end_, t)));
    }
    target.draw(points);
}

}  // namespace eng
