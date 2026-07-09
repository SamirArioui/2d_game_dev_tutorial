// Exercise 01 — solution
#include <cassert>
#include <iostream>

struct Spark {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float life = 0.0f;
};

bool update_spark(Spark& s, float dt) {
    s.x += s.vx * dt;
    s.y += s.vy * dt;
    s.life -= dt;
    return s.life > 0.0f;
}

int main() {
    Spark s;
    s.x = 0.0f;
    s.y = 0.0f;
    s.vx = 10.0f;
    s.vy = 0.0f;
    s.life = 1.0f;

    bool alive = update_spark(s, 0.5f);
    assert(alive);
    assert(s.x == 5.0f);
    assert(s.life == 0.5f);

    alive = update_spark(s, 0.6f);
    assert(!alive);

    std::cout << "all checks passed\n";
    return 0;
}
