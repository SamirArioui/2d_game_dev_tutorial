// Exercise 05 — solution
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
    for (std::size_t i = 0; i < count; ++i) {
        const float a = angle(rng);
        const float s = speed(rng);
        Particle p;
        p.position = origin;
        p.velocity = Vec2f{std::cos(a) * s, std::sin(a) * s};
        p.max_life = 0.8f;
        p.life = p.max_life;
        if (!pool.spawn(p)) {
            break;
        }
        ++spawned;
    }
    return spawned;
}

int main() {
    std::mt19937 rng(123);

    ParticlePool pool(50);
    const std::size_t made = spawn_burst(pool, Vec2f{100.0f, 100.0f}, 30, rng);
    assert(made == 30u);
    assert(pool.alive_count() == 30u);

    const std::size_t more = spawn_burst(pool, Vec2f{0, 0}, 1000, rng);
    assert(more == 20u);
    assert(pool.alive_count() == 50u);

    for (const Particle& p : pool.particles()) {
        if (p.alive) {
            assert(p.life > 0.0f);
        }
    }

    std::cout << "all checks passed\n";
    return 0;
}
