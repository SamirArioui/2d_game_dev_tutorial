// Exercise 06 — Particle pool (solution)
#include <catch2/catch_test_macros.hpp>

#include "eng/particles.hpp"

using eng::ParticleSystem;
using gmath::Vec2f;

TEST_CASE("pool fills on emit and drains as particles expire", "[ex06]") {
    ParticleSystem particles(64);

    REQUIRE(particles.alive_count() == 0);  // empty to start

    particles.emit_burst(Vec2f{0.0f, 0.0f}, 10, 50.0f, 1.0f);
    REQUIRE(particles.alive_count() == 10);

    particles.update(0.5f);
    REQUIRE(particles.alive_count() == 10);  // still within their 1s lifetime

    particles.update(0.6f);                  // total 1.1s > lifetime
    REQUIRE(particles.alive_count() == 0);   // all expired, slots freed
}

TEST_CASE("emitting more than capacity drops the extras", "[ex06]") {
    ParticleSystem particles(8);
    particles.emit_burst(Vec2f{0.0f, 0.0f}, 100, 50.0f, 1.0f);

    REQUIRE(particles.alive_count() == 8);  // capped at capacity
}
