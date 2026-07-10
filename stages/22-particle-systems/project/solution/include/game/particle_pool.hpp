#pragma once

// ============================================================================
// particle_pool — a POOLED, fixed-capacity particle buffer (stage 22).
//
// A particle system spawns lots of tiny, short-lived things (sparks, smoke,
// exhaust) and throws them away a fraction of a second later. The naive way is
// to allocate a new object per spawn and delete it on death — hundreds of
// times per frame. That churns the allocator and, in a GC language, feeds the
// garbage collector constantly.
//
// POOLING avoids all of that: allocate a FIXED array once, up front, and then
// RECYCLE slots. "Spawn" means "find a dead slot and revive it"; "die" means
// "mark the slot dead" so a later spawn can reuse it. No per-frame allocation,
// ever. The buffer never grows.
//
// Python comparison — pooling vs the garbage collector:
//   # naive: a new object each spawn; the GC must later collect all the dead
//   particles = [Particle() for _ in range(n)]     # allocates n objects
//   # pooled: one fixed list, reused forever; nothing for the GC to collect
//   pool = [Particle() for _ in range(CAP)]        # once
//   slot = find_dead(pool); slot.reset(...)        # reuse
// C++ has no GC, so the cost is malloc/free pressure and cache misses instead —
// pooling fixes both. It's a technique you'll reuse for bullets, enemies, any
// spawn-heavy system.
//
// This class is PURE: no SFML, no window. That's what lets us unit-test the
// pooling rules (caps at capacity, recycles dead slots, lifetimes expire).
// ============================================================================

#include <cstddef>  // std::size_t
#include <cstdint>  // std::uint8_t
#include <random>   // std::mt19937
#include <vector>

#include "game/gmath.hpp"

namespace game {

// One particle: plain data. Colour is stored as three bytes so the pool needn't
// know about SFML; the renderer turns (r,g,b) + a life-based alpha into pixels.
struct Particle {
    gmath::Vec2f position{0.0f, 0.0f};
    gmath::Vec2f velocity{0.0f, 0.0f};  // pixels per second
    float life = 0.0f;                  // seconds remaining
    float max_life = 1.0f;              // total lifetime (for the fade ratio)
    std::uint8_t r = 255;
    std::uint8_t g = 255;
    std::uint8_t b = 255;
    bool alive = false;
};

class ParticlePool {
public:
    // Allocate the whole buffer once. Every slot starts dead.
    explicit ParticlePool(std::size_t capacity);

    std::size_t capacity() const { return particles_.size(); }
    std::size_t alive_count() const { return alive_; }

    // Revive one dead slot with `proto` and return true. If the pool is FULL,
    // do nothing and return false — capacity is fixed, we never allocate more.
    // (The proto's `alive` flag is ignored; spawn always marks the slot alive.)
    bool spawn(const Particle& proto);

    // Advance every alive particle by `dt` seconds: integrate its motion
    // (position += velocity * dt, reusing stage-15 motion), age it
    // (life -= dt), and recycle any whose life has run out (mark dead, freeing
    // the slot for a future spawn).
    void update(float dt);

    // Read-only view for the renderer: iterate and skip slots where !alive.
    const std::vector<Particle>& particles() const { return particles_; }

private:
    std::vector<Particle> particles_;  // fixed size == capacity; slots recycled
    std::size_t alive_ = 0;            // maintained so alive_count() is O(1)
};

// --- Emitters: pure helpers that spawn a shape of particles into a pool ------
// Both take an mt19937 by reference (stage 01's RNG) so the caller controls
// seeding — which also makes them reproducible in tests. Each returns how many
// particles it actually spawned (fewer than asked if the pool filled up).

// Explosion burst: `count` particles flung out from `origin` in all directions,
// with randomized speed and lifetime. Warm colours (orange/yellow).
std::size_t emit_burst(ParticlePool& pool, gmath::Vec2f origin, std::size_t count,
                       std::mt19937& rng);

// Thruster trail: `count` particles at `origin` moving roughly OPPOSITE to
// `ship_velocity` (so they stream out behind the ship), with a little jitter.
// Cool colours (blue/cyan) and short lifetimes.
std::size_t emit_trail(ParticlePool& pool, gmath::Vec2f origin, gmath::Vec2f ship_velocity,
                       std::size_t count, std::mt19937& rng);

}  // namespace game
