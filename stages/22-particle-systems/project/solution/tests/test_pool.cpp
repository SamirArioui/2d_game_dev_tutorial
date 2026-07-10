// ============================================================================
// Tests for the ParticlePool — pure logic, no SFML, no window.
//
// The pooling INVARIANTS are exactly what we must guarantee:
//   * spawning caps at capacity (the buffer never grows),
//   * dead particles are recycled (freed slots get reused),
//   * lifetimes expire on update,
//   * motion integrates correctly,
//   * the emitters spawn the right count and shape.
// All of that is pure arithmetic + bookkeeping, so Catch2 nails it down without
// ever opening a window.
// ============================================================================

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <random>

#include "game/particle_pool.hpp"

using game::emit_burst;
using game::emit_trail;
using game::Particle;
using game::ParticlePool;
using gmath::Vec2f;

// A helper to build a simple, long-lived particle proto for the mechanics tests.
static Particle make_proto(Vec2f pos, Vec2f vel, float life) {
    Particle p;
    p.position = pos;
    p.velocity = vel;
    p.life = life;
    p.max_life = life;
    return p;
}

TEST_CASE("a fresh pool has fixed capacity and no live particles", "[pool]") {
    ParticlePool pool(8);
    REQUIRE(pool.capacity() == 8u);
    REQUIRE(pool.alive_count() == 0u);
    REQUIRE(pool.particles().size() == 8u);  // buffer allocated up front
}

TEST_CASE("spawning caps at capacity; the buffer never grows", "[pool][cap]") {
    ParticlePool pool(4);
    const Particle proto = make_proto(Vec2f{0, 0}, Vec2f{0, 0}, 10.0f);

    // First 4 spawns succeed...
    for (int i = 0; i < 4; ++i) {
        REQUIRE(pool.spawn(proto));
    }
    REQUIRE(pool.alive_count() == 4u);

    // ...the 5th and 6th are refused (return false), and NOTHING grows.
    REQUIRE_FALSE(pool.spawn(proto));
    REQUIRE_FALSE(pool.spawn(proto));
    REQUIRE(pool.alive_count() == 4u);
    REQUIRE(pool.capacity() == 4u);
    REQUIRE(pool.particles().size() == 4u);
}

TEST_CASE("dead particles are recycled", "[pool][recycle]") {
    ParticlePool pool(3);
    const Particle proto = make_proto(Vec2f{0, 0}, Vec2f{0, 0}, 1.0f);

    // Fill it, then age everything past its lifetime so all slots die.
    for (int i = 0; i < 3; ++i) {
        REQUIRE(pool.spawn(proto));
    }
    REQUIRE(pool.alive_count() == 3u);

    pool.update(2.0f);  // life 1.0 - 2.0 <= 0 -> all dead
    REQUIRE(pool.alive_count() == 0u);
    REQUIRE(pool.capacity() == 3u);            // capacity unchanged
    REQUIRE(pool.particles().size() == 3u);    // still exactly 3 slots, reused

    // The freed slots can be spawned into again.
    REQUIRE(pool.spawn(proto));
    REQUIRE(pool.spawn(proto));
    REQUIRE(pool.alive_count() == 2u);
    REQUIRE(pool.particles().size() == 3u);    // never grew
}

TEST_CASE("lifetimes expire on update", "[pool][lifetime]") {
    ParticlePool pool(2);
    pool.spawn(make_proto(Vec2f{0, 0}, Vec2f{0, 0}, 1.0f));

    SECTION("still alive before its life runs out") {
        pool.update(0.5f);
        REQUIRE(pool.alive_count() == 1u);
    }
    SECTION("dead once cumulative dt exceeds its life") {
        pool.update(0.5f);
        pool.update(0.6f);  // total 1.1 > 1.0
        REQUIRE(pool.alive_count() == 0u);
    }
}

TEST_CASE("update integrates position from velocity", "[pool][motion]") {
    ParticlePool pool(1);
    pool.spawn(make_proto(Vec2f{0.0f, 0.0f}, Vec2f{10.0f, -4.0f}, 100.0f));

    pool.update(0.5f);  // pos += vel * 0.5

    // Only one particle is alive; find it and check where it moved to.
    for (const Particle& p : pool.particles()) {
        if (p.alive) {
            REQUIRE(p.position.x == Catch::Approx(5.0f));
            REQUIRE(p.position.y == Catch::Approx(-2.0f));
        }
    }
}

TEST_CASE("emit_burst caps at free slots and makes live particles", "[pool][burst]") {
    ParticlePool pool(3);
    std::mt19937 rng(42);

    // Asking for 10 into a 3-slot pool spawns exactly 3.
    const std::size_t made = emit_burst(pool, Vec2f{100.0f, 100.0f}, 10, rng);
    REQUIRE(made == 3u);
    REQUIRE(pool.alive_count() == 3u);

    // Each spawned particle is alive and has a positive lifetime.
    int checked = 0;
    for (const Particle& p : pool.particles()) {
        if (p.alive) {
            CHECK(p.life > 0.0f);
            CHECK(p.max_life > 0.0f);
            ++checked;
        }
    }
    REQUIRE(checked == 3);

    // A full pool accepts no more.
    REQUIRE(emit_burst(pool, Vec2f{0, 0}, 5, rng) == 0u);
}

TEST_CASE("emit_trail streams opposite the ship velocity", "[pool][trail]") {
    ParticlePool pool(32);
    std::mt19937 rng(7);

    const Vec2f ship_vel{200.0f, 0.0f};  // moving right...
    const std::size_t made = emit_trail(pool, Vec2f{0, 0}, ship_vel, 16, rng);
    REQUIRE(made == 16u);

    // ...so exhaust should carry a component pointing LEFT (opposite): the dot
    // product of each particle's velocity with the ship velocity is negative.
    for (const Particle& p : pool.particles()) {
        if (p.alive) {
            CHECK(gmath::dot(p.velocity, ship_vel) < 0.0f);
        }
    }
}
