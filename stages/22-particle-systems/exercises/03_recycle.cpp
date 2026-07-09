// Exercise 03 — recycle dead slots (medium)
//
// Pooling only pays off if dead slots come back. After a particle's life runs
// out, update() marks its slot dead, and a later spawn should REUSE that slot —
// with no growth in the underlying buffer. Prove that here.
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra -I ../project/include \
//       ../project/src/particle_pool.cpp 03_recycle.cpp -o ex && ./ex
//
// TODO:
//   1. Call pool.update(dt) with a dt large enough to kill every particle
//      (their life is 1.0), so all slots become free again.

#include <cassert>
#include <iostream>

#include "game/particle_pool.hpp"

using game::Particle;
using game::ParticlePool;

int main() {
    ParticlePool pool(3);

    Particle proto;
    proto.life = 1.0f;
    proto.max_life = 1.0f;

    // Fill the pool.
    for (int i = 0; i < 3; ++i) {
        assert(pool.spawn(proto));
    }
    assert(pool.alive_count() == 3u);

    // TODO 1: age every particle past its 1.0s life so all three die.

    assert(pool.alive_count() == 0u);          // all recycled
    assert(pool.capacity() == 3u);             // capacity unchanged
    assert(pool.particles().size() == 3u);     // buffer did NOT grow

    // The freed slots accept new spawns.
    assert(pool.spawn(proto));
    assert(pool.spawn(proto));
    assert(pool.alive_count() == 2u);
    assert(pool.particles().size() == 3u);     // still exactly 3 slots

    std::cout << "all checks passed\n";
    return 0;
}
