// Exercise 02 — bounce the ball off a paddle  (PURE, no SFML)
//
// A Pong bounce is more than "reverse X". Where the ball hits the paddle should
// steer it: hit near the top, it goes up; hit near the bottom, it goes down.
// We pass that contact point as `offset` in [-1, +1] (-1 = top, 0 = centre,
// +1 = bottom) so the geometry is somebody else's problem here.
//
//   clang++ -std=c++17 -Wall -Wextra 02_paddle_bounce.cpp -o ex && ./ex
//
// TODO:
//   Implement bounce(). Reverse the horizontal velocity, and set the vertical
//   velocity from the offset. A simple, testable rule:
//       new.x = -in.x
//       new.y = offset * abs(in.x)      (so top -> up, bottom -> down)

#include <cassert>
#include <cmath>
#include <iostream>

struct Vel {
    float x, y;
};

Vel bounce(Vel in, float offset) {
    // TODO: return the reflected velocity with spin from `offset`.
    (void)offset;
    return in;
}

int main() {
    // ball moving left, dead-centre hit -> moving right, no vertical spin
    Vel a = bounce(Vel{-300.0f, 0.0f}, 0.0f);
    assert(a.x > 0.0f);
    assert(std::abs(a.y) < 1e-3f);

    // hit low (offset > 0) -> downward (+y)
    Vel b = bounce(Vel{-300.0f, 0.0f}, 0.5f);
    assert(b.x > 0.0f && b.y > 0.0f);

    // ball moving right, hit high (offset < 0) -> upward (-y) and now moving left
    Vel c = bounce(Vel{300.0f, 0.0f}, -0.5f);
    assert(c.x < 0.0f && c.y < 0.0f);

    std::cout << "02_paddle_bounce: all assertions passed\n";
    return 0;
}
