// Exercise 01 — one particle's lifetime (easy)
//
// A particle is mostly two things: it MOVES (position += velocity * dt) and it
// AGES (life -= dt). When its life hits zero it's dead. Here you write that
// per-particle update for a single "spark".
//
// Build & run (from this exercises/ folder):
//   clang++ -std=c++17 -Wall -Wextra 01_lifetime.cpp -o ex && ./ex
//
// TODO:
//   1. Move the spark by velocity * dt, subtract dt from its life, and return
//      true if it's still alive (life > 0) or false if it just died.

#include <cassert>
#include <iostream>

struct Spark {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float life = 0.0f;  // seconds remaining
};

bool update_spark(Spark& s, float dt) {
    // TODO 1: integrate motion, age the spark, return whether it's still alive.
    (void)s;
    (void)dt;
    return true;  // <-- replace
}

int main() {
    Spark s;
    s.x = 0.0f;
    s.y = 0.0f;
    s.vx = 10.0f;
    s.vy = 0.0f;
    s.life = 1.0f;

    bool alive = update_spark(s, 0.5f);  // half a second
    assert(alive);
    assert(s.x == 5.0f);  // moved 10 px/s * 0.5 s
    assert(s.life == 0.5f);

    alive = update_spark(s, 0.6f);  // total 1.1s > 1.0s life
    assert(!alive);

    std::cout << "all checks passed\n";
    return 0;
}
