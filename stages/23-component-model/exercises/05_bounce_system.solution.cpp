// Exercise 05 — The bounce system (solution)
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
        if (t->y < 0.0f) {
            t->y = 0.0f;
            v->vy = -v->vy;
        } else if (t->y + s->h > height) {
            t->y = height - s->h;
            v->vy = -v->vy;
        }
    }
}

int main() {
    std::vector<Entity> entities;

    Entity& ball = entities.emplace_back();
    ball.add(Transform{0.0f, -5.0f});
    ball.add(Velocity{0.0f, -120.0f});
    ball.add(Shape{10.0f, 10.0f});

    bounce_y(entities, 480.0f);

    assert(ball.get<Transform>().y == 0.0f);
    assert(ball.get<Velocity>().vy == 120.0f);

    std::cout << "bounce system works\n";
    return 0;
}
