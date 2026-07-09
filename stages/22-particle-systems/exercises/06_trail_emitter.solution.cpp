// Exercise 06 — solution
#include <cassert>
#include <iostream>
#include <random>

#include "game/particle_pool.hpp"

using game::Particle;
using game::ParticlePool;
using gmath::Vec2f;

std::size_t spawn_trail(ParticlePool& pool, Vec2f origin, Vec2f ship_velocity, std::size_t count) {
    const Vec2f back = gmath::normalize(Vec2f{-ship_velocity.x, -ship_velocity.y});
    const float speed = 100.0f;
    std::size_t spawned = 0;
    for (std::size_t i = 0; i < count; ++i) {
        Particle p;
        p.position = origin;
        p.velocity = Vec2f{back.x * speed, back.y * speed};
        p.max_life = 0.4f;
        p.life = p.max_life;
        if (!pool.spawn(p)) {
            break;
        }
        ++spawned;
    }
    return spawned;
}

int main() {
    ParticlePool pool(32);
    const Vec2f ship_vel{0.0f, -200.0f};

    const std::size_t made = spawn_trail(pool, Vec2f{400.0f, 300.0f}, ship_vel, 10);
    assert(made == 10u);

    for (const Particle& p : pool.particles()) {
        if (p.alive) {
            assert(gmath::dot(p.velocity, ship_vel) < 0.0f);
        }
    }

    std::cout << "all checks passed\n";
    return 0;
}
