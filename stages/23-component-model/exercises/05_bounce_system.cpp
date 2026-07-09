// Exercise 05 — The bounce system (medium-hard)
//
// Put it together: a system that keeps a moving, sized entity inside a box by reflecting it off
// the top and bottom walls (the ball behaviour from the Pong mini-project, vertical axis only).
//
// TODO: finish bounce_y so that for every entity with Transform + Velocity + Shape:
//   - if it has crossed the TOP    (pos.y < 0):            clamp pos.y = 0        and flip vy sign
//   - if it has crossed the BOTTOM (pos.y + size_y > h):   clamp pos.y = h-size_y and flip vy sign
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_bounce_system.cpp -o ex && ./ex

#include <any>
#include <cassert>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

struct Transform {
    float x{0.0f};
    float y{0.0f};
};
struct Velocity {
    float vx{0.0f};
    float vy{0.0f};
};
struct Shape {
    float w{0.0f};
    float h{0.0f};
};

class Entity {
public:
    template <class T>
    T& add(T c) {
        std::any& s = components_[std::type_index(typeid(T))];
        s = std::move(c);
        return std::any_cast<T&>(s);
    }
    template <class T>
    T* try_get() {
        auto it = components_.find(std::type_index(typeid(T)));
        return it == components_.end() ? nullptr : std::any_cast<T>(&it->second);
    }
    template <class T>
    T& get() {
        return std::any_cast<T&>(components_.at(std::type_index(typeid(T))));
    }

private:
    std::unordered_map<std::type_index, std::any> components_;
};

void bounce_y(std::vector<Entity>& entities, float height) {
    for (auto& e : entities) {
        auto* t = e.try_get<Transform>();
        auto* v = e.try_get<Velocity>();
        auto* s = e.try_get<Shape>();
        if (t == nullptr || v == nullptr || s == nullptr) {
            continue;
        }
        // TODO: implement the top and bottom checks described above.
        (void)height;
    }
}

int main() {
    std::vector<Entity> entities;

    Entity& ball = entities.emplace_back();
    ball.add(Transform{0.0f, -5.0f});  // already above the top edge
    ball.add(Velocity{0.0f, -120.0f}); // heading further up
    ball.add(Shape{10.0f, 10.0f});

    bounce_y(entities, 480.0f);

    assert(ball.get<Transform>().y == 0.0f);      // clamped back inside
    assert(ball.get<Velocity>().vy == 120.0f);    // reversed

    std::cout << "bounce system works\n";
    return 0;
}
