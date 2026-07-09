// Exercise 05 — an explosion emitter (medium-hard)
//
// An emitter is just a helper that spawns a SHAPE of particles into a pool. A
// burst flings `count` particles outward in random directions. Write one, using
// <random> (std::mt19937 from stage 01) for the angle and speed, and have it
// respect the pool's capacity by stopping when spawn() refuses.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/particle_pool.cpp 05_burst_emitter.cpp -o ex && ./ex
//
// TODO:
//   1. For each of `count`, build a Particle at `origin` with a velocity of
//      (cos(angle), sin(angle)) * speed for a random angle in [0, 2*pi) and a
//      random speed. Give it a positive life. spawn() it; if that returns
//      false, break. Return how many you actually spawned.

#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

#include "game/particle_pool.hpp"

using game::Particle;
using game::ParticlePool;
using gmath::Vec2f;

std::size_t spawn_burst(ParticlePool& pool, Vec2f origin, std::size_t count, std::mt19937& rng) {
    std::uniform_real_distribution<float> angle(0.0f, 6.2831853f);
    std::uniform_real_distribution<float> speed(40.0f, 160.0f);
    std::size_t spawned = 0;
    // TODO 1: spawn up to `count` outward-flung particles; stop if the pool fills.
    (void)pool;
    (void)origin;
    (void)angle;
    (void)speed;
    (void)rng;
    return spawned;
}

int main() {
    std::mt19937 rng(123);

    ParticlePool pool(50);
    const std::size_t made = spawn_burst(pool, Vec2f{100.0f, 100.0f}, 30, rng);
    assert(made == 30u);
    assert(pool.alive_count() == 30u);

    // Ask for more than fits: it must cap at the remaining slots (20).
    const std::size_t more = spawn_burst(pool, Vec2f{0, 0}, 1000, rng);
    assert(more == 20u);
    assert(pool.alive_count() == 50u);

    // Every live particle has a positive lifetime.
    for (const Particle& p : pool.particles()) {
        if (p.alive) {
            assert(p.life > 0.0f);
        }
    }

    std::cout << "all checks passed\n";
    return 0;
}
