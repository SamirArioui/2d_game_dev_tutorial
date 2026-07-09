// Exercise 02 — spawning caps at capacity (medium)
//
// The defining rule of a pool: it NEVER grows. Once every slot is alive, spawn
// must refuse (return false) rather than allocate more. Here you drive a real
// ParticlePool and confirm the cap.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/particle_pool.cpp 02_spawn_cap.cpp -o ex && ./ex
//
// TODO:
//   1. Spawn `proto` in a loop until spawn() returns false, counting how many
//      succeeded. Store that count in `spawned`.

#include <cassert>
#include <iostream>

#include "game/particle_pool.hpp"

using game::Particle;
using game::ParticlePool;

int main() {
    ParticlePool pool(5);

    Particle proto;
    proto.life = 10.0f;
    proto.max_life = 10.0f;

    int spawned = 0;
    // TODO 1: keep spawning until the pool refuses; count the successes.

    std::cout << "spawned " << spawned << " into a pool of " << pool.capacity() << "\n";
    assert(spawned == 5);
    assert(pool.alive_count() == 5u);
    assert(pool.capacity() == 5u);            // never grew
    assert(pool.particles().size() == 5u);
    assert(!pool.spawn(proto));               // still full

    std::cout << "all checks passed\n";
    return 0;
}
