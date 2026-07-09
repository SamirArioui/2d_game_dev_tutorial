// Exercise 03 — solution
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

    for (int i = 0; i < 3; ++i) {
        assert(pool.spawn(proto));
    }
    assert(pool.alive_count() == 3u);

    pool.update(2.0f);  // 1.0 - 2.0 <= 0 for every particle -> all die

    assert(pool.alive_count() == 0u);
    assert(pool.capacity() == 3u);
    assert(pool.particles().size() == 3u);

    assert(pool.spawn(proto));
    assert(pool.spawn(proto));
    assert(pool.alive_count() == 2u);
    assert(pool.particles().size() == 3u);

    std::cout << "all checks passed\n";
    return 0;
}
