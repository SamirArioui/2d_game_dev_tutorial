// Exercise 01 — The plain-OOP starting point (easy)
//
// Before we refactor to components, feel how the "classic" OOP approach works — and where it
// starts to pinch. Here every game object inherits from a GameObject base and overrides update().
//
// Python comparison: this is the textbook `class Enemy(GameObject):` inheritance tree.
//
// TODO:
//   1. Make `Ball` inherit from GameObject (public inheritance).
//   2. Override update(dt) so the ball advances: x_ += vx_ * dt.
//   3. Print the ball's x after two updates.
//
// The stage README argues why this hierarchy gets awkward as objects grow (a flying, exploding,
// scored pickup would need bits of many unrelated base classes). Exercise 02 shows the fix.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_plain_oop.cpp -o ex && ./ex

#include <iostream>

// A base class with a virtual update(). Subclasses override it. (Stage 07's inheritance.)
class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void update(float dt) = 0;  // pure virtual: every GameObject MUST define update
};

class Ball /* TODO: inherit from GameObject */ {
public:
    Ball(float x, float vx) : x_(x), vx_(vx) {}

    // TODO: override update(dt) to advance x_ by vx_ * dt.

    float x() const { return x_; }

private:
    float x_{0.0f};
    float vx_{0.0f};
};

int main() {
    Ball ball(0.0f, 50.0f);
    // TODO: call ball.update(0.1f) twice, then print ball.x() — expect 10.
    std::cout << "ball x = " << ball.x() << "\n";
    return 0;
}
