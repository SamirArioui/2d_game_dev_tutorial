// Exercise 06 — Particle pool (hard)
//
// ParticleSystem keeps a FIXED pool of slots. Emitting reuses dead slots; a
// particle dies when its life hits zero. Prove the pool fills on emit and drains
// after enough time passes. (The pool logic is SFML-free; we never call render.)

#include <catch2/catch_test_macros.hpp>

#include "eng/particles.hpp"

using eng::ParticleSystem;
using gmath::Vec2f;

TEST_CASE("pool fills on emit and drains as particles expire", "[ex06]") {
    ParticleSystem particles(64);  // capacity of 64 slots

    // TODO: a brand-new system has how many LIVE particles?
    REQUIRE(particles.alive_count() == 999);  // <-- fix

    // Emit 10 particles, each living 1 second.
    particles.emit_burst(Vec2f{0.0f, 0.0f}, 10, 50.0f, 1.0f);

    // TODO: how many are alive right after the burst?
    REQUIRE(particles.alive_count() == 0);  // <-- fix

    // Advance half a second: still alive (lifetime was 1s).
    particles.update(0.5f);
    REQUIRE(particles.alive_count() == 10);

    // Advance past the end of their life: all should be dead, slots free again.
    particles.update(0.6f);
    // TODO: how many are alive now?
    REQUIRE(particles.alive_count() == 10);  // <-- fix
}

TEST_CASE("emitting more than capacity drops the extras", "[ex06]") {
    ParticleSystem particles(8);
    particles.emit_burst(Vec2f{0.0f, 0.0f}, 100, 50.0f, 1.0f);  // ask for 100

    // TODO: the pool can hold at most its capacity. How many are alive?
    REQUIRE(particles.alive_count() == 100);  // <-- fix
}
