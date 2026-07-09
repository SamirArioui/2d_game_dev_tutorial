// Exercise 02 — solution
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
    while (pool.spawn(proto)) {
        ++spawned;
    }

    std::cout << "spawned " << spawned << " into a pool of " << pool.capacity() << "\n";
    assert(spawned == 5);
    assert(pool.alive_count() == 5u);
    assert(pool.capacity() == 5u);
    assert(pool.particles().size() == 5u);
    assert(!pool.spawn(proto));

    std::cout << "all checks passed\n";
    return 0;
}
