// Exercise 03 — solution  (PURE, no SFML)
#include <cassert>
#include <iostream>

enum class Scorer { None, Left, Right };

Scorer check_scoring(float ball_x, float ball_w, float court_w) {
    if (ball_x + ball_w < 0.0f) {
        return Scorer::Right;  // ball fully past the LEFT edge
    }
    if (ball_x > court_w) {
        return Scorer::Left;  // ball past the RIGHT edge
    }
    return Scorer::None;
}

float serve_velocity_x(bool to_left, float speed) { return to_left ? -speed : speed; }

int main() {
    assert(check_scoring(-20.0f, 14.0f, 800.0f) == Scorer::Right);
    assert(check_scoring(820.0f, 14.0f, 800.0f) == Scorer::Left);
    assert(check_scoring(400.0f, 14.0f, 800.0f) == Scorer::None);

    assert(serve_velocity_x(true, 420.0f) < 0.0f);
    assert(serve_velocity_x(false, 420.0f) > 0.0f);

    std::cout << "03_scoring_and_serve: all assertions passed\n";
    return 0;
}
