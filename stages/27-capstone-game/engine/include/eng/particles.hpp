#pragma once

// ============================================================================
// eng::ParticleSystem — a POOLED particle emitter.
//
// Particles (sparks, dust, pickup bursts) are created and destroyed constantly.
// Allocating/freeing each one would thrash the heap, so we use a POOL: a fixed
// vector of Particle slots allocated ONCE. Emitting reuses dead slots; a dead
// particle is just one whose `life` has run out. Nothing is ever new/deleted at
// runtime.
//
// The pool logic (emit / update / count) is defined INLINE here and is SFML-FREE
// (colour is a plain RGBA struct, sf::RenderTarget is only forward-declared), so
// it unit-tests without a window. Only render() — defined in particles.cpp —
// touches SFML.
// ============================================================================

#include <cstddef>
#include <random>
#include <vector>

#include "eng/math.hpp"

namespace sf {
class RenderTarget;
}

namespace eng {

// SFML-free colour so the pool header pulls in no graphics headers.
struct Rgba {
    unsigned char r{255};
    unsigned char g{255};
    unsigned char b{255};
    unsigned char a{255};
};

struct Particle {
    gmath::Vec2f pos;
    gmath::Vec2f vel;
    float life{0.0f};      // seconds remaining; <= 0 means the slot is free
    float max_life{1.0f};  // original lifetime, used to compute the fade factor

    bool alive() const { return life > 0.0f; }
};

class ParticleSystem {
public:
    explicit ParticleSystem(std::size_t capacity = 512) : pool_(capacity), rng_(std::random_device{}()) {}

    // Spawn up to `count` particles at `pos`, flying outward in random directions
    // at roughly `speed` px/s, each living `lifetime` seconds. If the pool is
    // full the extra particles are silently dropped (a pool has a hard ceiling —
    // that is the point).
    void emit_burst(const gmath::Vec2f& pos, int count, float speed, float lifetime) {
        std::uniform_real_distribution<float> angle(0.0f, 6.2831853f);      // 0..2pi
        std::uniform_real_distribution<float> speed_jitter(0.4f, 1.0f);
        for (int i = 0; i < count; ++i) {
            Particle* slot = find_dead_slot();
            if (slot == nullptr) {
                return;  // pool exhausted
            }
            const float a = angle(rng_);
            const float s = speed * speed_jitter(rng_);
            slot->pos = pos;
            slot->vel = gmath::Vec2f{std::cos(a) * s, std::sin(a) * s};
            slot->life = lifetime;
            slot->max_life = lifetime;
        }
    }

    // Advance every live particle. Dead ones are skipped; their slots stay free
    // for the next emit. A little drag makes bursts feel like they settle.
    void update(float dt) {
        for (Particle& p : pool_) {
            if (!p.alive()) {
                continue;
            }
            p.life -= dt;
            p.pos += p.vel * dt;
            p.vel *= 0.98f;  // simple drag
        }
    }

    // Draw every live particle as a fading dot. Defined in particles.cpp because
    // it needs SFML; declared here so the rest of the class stays header-only.
    void render(sf::RenderTarget& target) const;

    void set_color(const Rgba& start, const Rgba& end) {
        start_ = start;
        end_ = end;
    }

    std::size_t alive_count() const {
        std::size_t n = 0;
        for (const Particle& p : pool_) {
            if (p.alive()) {
                ++n;
            }
        }
        return n;
    }
    std::size_t capacity() const { return pool_.size(); }

    const std::vector<Particle>& particles() const { return pool_; }

private:
    Particle* find_dead_slot() {
        for (Particle& p : pool_) {
            if (!p.alive()) {
                return &p;
            }
        }
        return nullptr;
    }

    std::vector<Particle> pool_;
    Rgba start_{255, 220, 120, 255};  // bright core
    Rgba end_{255, 90, 30, 0};        // fades to transparent orange
    std::mt19937 rng_;
};

}  // namespace eng
