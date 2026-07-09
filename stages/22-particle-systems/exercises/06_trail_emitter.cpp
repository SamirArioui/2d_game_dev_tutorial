// Exercise 06 — a thruster-trail emitter (medium-hard)
//
// A trail is directional: exhaust streams OUT THE BACK of a moving ship, i.e.
// opposite its velocity. Normalize the reversed velocity to get the "back"
// direction, then spawn particles moving that way. The test checks the physics:
// each particle's velocity, dotted with the ship's velocity, is negative
// (they point the other way).
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/particle_pool.cpp 06_trail_emitter.cpp -o ex && ./ex
//
// TODO:
//   1. Compute `back` = normalize(-ship_velocity). For each of `count`, spawn a
//      Particle at `origin` whose velocity is back * speed (a fixed 100 is fine)
//      with a positive life. Stop if the pool fills. Return the count spawned.

#include <cassert>
#include <iostream>
#include <random>

#include "game/particle_pool.hpp"

using game::Particle;
using game::ParticlePool;
using gmath::Vec2f;

std::size_t spawn_trail(ParticlePool& pool, Vec2f origin, Vec2f ship_velocity, std::size_t count) {
    std::size_t spawned = 0;
    // TODO 1: back = normalize(-ship_velocity); spawn `count` particles moving
    //         along `back`, stopping if the pool fills.
    (void)pool;
    (void)origin;
    (void)ship_velocity;
    (void)count;
    return spawned;
}

int main() {
    ParticlePool pool(32);
    const Vec2f ship_vel{0.0f, -200.0f};  // flying UP...

    const std::size_t made = spawn_trail(pool, Vec2f{400.0f, 300.0f}, ship_vel, 10);
    assert(made == 10u);

    // ...so exhaust should carry DOWNWARD momentum: dot with the ship velocity
    // is negative for every spawned particle.
    for (const Particle& p : pool.particles()) {
        if (p.alive) {
            assert(gmath::dot(p.velocity, ship_vel) < 0.0f);
        }
    }

    std::cout << "all checks passed\n";
    return 0;
}
