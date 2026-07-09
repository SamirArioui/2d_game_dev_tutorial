// Exercise 03 — scoring and serving  (PURE, no SFML)
//
// Two rules that end and restart a rally:
//   * who scored? The left paddle guards the LEFT edge; if the ball leaves past
//     the left edge, the RIGHT player scores (and vice versa).
//   * serve: put the ball back at centre, moving toward the side you choose.
//
//   clang++ -std=c++17 -Wall -Wextra 03_scoring_and_serve.cpp -o ex && ./ex
//
// TODO:
//   1. check_scoring(): the ball's right edge is ball_x + ball_w.
//        - if the whole ball is past the LEFT edge (right edge < 0)  -> Right
//        - if the ball's left edge is past the RIGHT edge (ball_x > court_w) -> Left
//        - otherwise -> None
//   2. serve_velocity_x(): return -speed to serve left, +speed to serve right.

#include <cassert>
#include <iostream>

enum class Scorer { None, Left, Right };

Scorer check_scoring(float ball_x, float ball_w, float court_w) {
    // TODO: implement the three cases above.
    (void)ball_x; (void)ball_w; (void)court_w;
    return Scorer::None;
}

float serve_velocity_x(bool to_left, float speed) {
    // TODO: negative to serve left, positive to serve right.
    (void)to_left;
    return speed;
}

int main() {
    // court 800 wide, ball 14 wide
    assert(check_scoring(-20.0f, 14.0f, 800.0f) == Scorer::Right);  // off the left
    assert(check_scoring(820.0f, 14.0f, 800.0f) == Scorer::Left);   // off the right
    assert(check_scoring(400.0f, 14.0f, 800.0f) == Scorer::None);   // in play

    assert(serve_velocity_x(true, 420.0f) < 0.0f);
    assert(serve_velocity_x(false, 420.0f) > 0.0f);

    std::cout << "03_scoring_and_serve: all assertions passed\n";
    return 0;
}
