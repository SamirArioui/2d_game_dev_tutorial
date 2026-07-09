// Exercise 01 — The plain-OOP starting point (solution)
#include <iostream>

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void update(float dt) = 0;
};

// Public inheritance: a Ball IS-A GameObject and must supply the update() the base declared.
class Ball : public GameObject {
public:
    Ball(float x, float vx) : x_(x), vx_(vx) {}

    void update(float dt) override { x_ += vx_ * dt; }

    float x() const { return x_; }

private:
    float x_{0.0f};
    float vx_{0.0f};
};

int main() {
    Ball ball(0.0f, 50.0f);
    ball.update(0.1f);
    ball.update(0.1f);
    std::cout << "ball x = " << ball.x() << "\n";  // 0 + 50*0.1 + 50*0.1 = 10
    return 0;
}
