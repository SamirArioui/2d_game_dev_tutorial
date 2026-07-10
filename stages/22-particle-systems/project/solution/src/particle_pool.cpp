// ============================================================================
// particle_pool implementation — pooling, motion integration, emitters. No SFML.  (REFERENCE SOLUTION)
// ============================================================================
// This is the complete reference implementation of the pool + emitters. The
// starter you fill in is one level up at ../../src/particle_pool.cpp (everything
// else in this folder is identical to the starter). Build this folder
// standalone to see all tests pass:
//   cmake -S . -B build && cmake --build build && ctest --test-dir build
// ============================================================================

#include "game/particle_pool.hpp"

#include <cmath>  // std::cos, std::sin

namespace game {

ParticlePool::ParticlePool(std::size_t capacity) {
    // Allocate the entire buffer ONCE. This is the only allocation the pool
    // ever does; from here on we recycle these slots forever.
    particles_.resize(capacity);
}

bool ParticlePool::spawn(const Particle& proto) {
    if (alive_ >= particles_.size()) {
        return false;  // full — refuse rather than grow
    }
    // Find a dead slot to recycle. A linear scan is fine for the small pools we
    // use and keeps the idea obvious; a production system would keep a free-list
    // of dead indices to make this O(1).
    for (Particle& p : particles_) {
        if (!p.alive) {
            p = proto;       // copy the prototype's fields into the slot
            p.alive = true;  // spawn always revives, whatever proto said
            ++alive_;
            return true;
        }
    }
    return false;  // unreachable while alive_ is correct, but safe
}

void ParticlePool::update(float dt) {
    for (Particle& p : particles_) {
        if (!p.alive) {
            continue;
        }
        // Motion integration (stage 15): move by velocity * dt.
        p.position += p.velocity * dt;
        // Age it.
        p.life -= dt;
        if (p.life <= 0.0f) {
            p.alive = false;  // recycle: this slot is now free to reuse
            --alive_;
        }
    }
}

std::size_t emit_burst(ParticlePool& pool, gmath::Vec2f origin, std::size_t count,
                       std::mt19937& rng) {
    std::uniform_real_distribution<float> angle(0.0f, 6.2831853f);  // 0..2*pi
    std::uniform_real_distribution<float> speed(40.0f, 160.0f);
    std::uniform_real_distribution<float> life(0.4f, 1.0f);
    std::uniform_int_distribution<int> green(120, 200);  // orange..yellow

    std::size_t spawned = 0;
    for (std::size_t i = 0; i < count; ++i) {
        const float a = angle(rng);
        const float s = speed(rng);

        Particle p;
        p.position = origin;
        p.velocity = gmath::Vec2f{std::cos(a) * s, std::sin(a) * s};
        p.max_life = life(rng);
        p.life = p.max_life;
        p.r = 255;
        p.g = static_cast<std::uint8_t>(green(rng));
        p.b = 40;

        if (!pool.spawn(p)) {
            break;  // pool full — stop early
        }
        ++spawned;
    }
    return spawned;
}

std::size_t emit_trail(ParticlePool& pool, gmath::Vec2f origin, gmath::Vec2f ship_velocity,
                       std::size_t count, std::mt19937& rng) {
    // Exhaust streams OPPOSITE the direction of travel. If the ship is still,
    // fall back to "downward" so a parked ship still puffs.
    gmath::Vec2f back = gmath::Vec2f{-ship_velocity.x, -ship_velocity.y};
    if (gmath::length(back) < 0.001f) {
        back = gmath::Vec2f{0.0f, 1.0f};
    }
    back = gmath::normalize(back);

    std::uniform_real_distribution<float> exhaust_speed(60.0f, 120.0f);
    std::uniform_real_distribution<float> jitter(-25.0f, 25.0f);
    std::uniform_real_distribution<float> life(0.2f, 0.5f);
    std::uniform_int_distribution<int> blue(200, 255);

    std::size_t spawned = 0;
    for (std::size_t i = 0; i < count; ++i) {
        const float s = exhaust_speed(rng);

        Particle p;
        p.position = origin;
        // Mostly backward, plus a small perpendicular-ish jitter for spread.
        p.velocity = gmath::Vec2f{back.x * s + jitter(rng), back.y * s + jitter(rng)};
        p.max_life = life(rng);
        p.life = p.max_life;
        p.r = 60;
        p.g = 140;
        p.b = static_cast<std::uint8_t>(blue(rng));

        if (!pool.spawn(p)) {
            break;
        }
        ++spawned;
    }
    return spawned;
}

}  // namespace game
