// ============================================================================
// particle_pool implementation — pooling, motion integration, emitters. No SFML.  — YOUR TASK
// ============================================================================
//
// This is the STARTER. Implement the pooling rules and the two emitters below
// so the pool tests in tests/test_pool.cpp go from RED to GREEN and the demo
// (src/main.cpp) can burst + trail particles. Each function's contract — the
// invariants the tests pin down — is documented in
// include/game/particle_pool.hpp; read it first.
//
// The buffer allocation (the constructor) is done for you. The starter already
// COMPILES and LINKS (each body has a placeholder return), but the tests fail
// until you fill in the TODOs. Everything else in this stage (the SFML
// vertex-array renderer with its life-based alpha FADE, the demo, the headers)
// is complete — the *lesson* is the pure, testable pool logic, so that is what
// you write.
//
// A complete reference is in ../solution/src/particle_pool.cpp — try it first.
//
// Build & test (from this stage's project/ folder):
//   cmake -S . -B build && cmake --build build
//   ctest --test-dir build --output-on-failure   # RED until you implement these
// ============================================================================

#include "game/particle_pool.hpp"

#include <cmath>  // std::cos, std::sin

namespace game {

ParticlePool::ParticlePool(std::size_t capacity) {
    // Allocate the entire buffer ONCE. This is the only allocation the pool
    // ever does; from here on we recycle these slots forever. (Done for you.)
    particles_.resize(capacity);
}

bool ParticlePool::spawn(const Particle& proto) {
    // TODO(stage 22): revive one DEAD slot with `proto` and return true. If the
    // pool is full (alive_ >= capacity) do nothing and return false — capacity
    // is fixed, never grow. Find a slot where !alive, copy proto into it, force
    // its `alive` flag true (whatever proto said), and increment alive_.
    return false;  // placeholder
}

void ParticlePool::update(float dt) {
    // TODO(stage 22): advance every ALIVE particle by dt: integrate its motion
    // (position += velocity * dt, stage-15 style), age it (life -= dt), and when
    // life runs out (<= 0) recycle the slot — mark it dead and decrement alive_
    // so a later spawn can reuse it. (Skip slots that are already dead.)
    (void)dt;
}

std::size_t emit_burst(ParticlePool& pool, gmath::Vec2f origin, std::size_t count,
                       std::mt19937& rng) {
    // TODO(stage 22): spawn up to `count` particles flung out from `origin` in
    // ALL directions (random angle 0..2*pi via a uniform_real_distribution,
    // random speed, random lifetime) with warm colours. Set each Particle's
    // position=origin, velocity={cos(a)*s, sin(a)*s}, life=max_life. Call
    // pool.spawn(p) for each; STOP early when it returns false (pool full).
    // Return how many actually spawned.
    return 0;  // placeholder
}

std::size_t emit_trail(ParticlePool& pool, gmath::Vec2f origin, gmath::Vec2f ship_velocity,
                       std::size_t count, std::mt19937& rng) {
    // TODO(stage 22): spawn up to `count` exhaust particles at `origin` moving
    // roughly OPPOSITE `ship_velocity` (normalize the negated velocity; fall
    // back to {0,1} if the ship is still), plus a little random jitter, with
    // cool colours and short lifetimes. Use gmath::length / gmath::normalize for
    // the direction. Call pool.spawn(p) each time; stop when it returns false.
    // Return how many actually spawned. (The tests check dot(velocity, ship_vel)
    // < 0 — i.e. the exhaust really does point backward.)
    return 0;  // placeholder
}

}  // namespace game
