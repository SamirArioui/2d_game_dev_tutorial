// Exercise 03 — Write a system (solution)
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

void movement_system(std::vector<Entity>& entities, float dt) {
    for (auto& e : entities) {
        auto* t = e.try_get<Transform>();
        auto* v = e.try_get<Velocity>();
        if (t != nullptr && v != nullptr) {  // the query: needs BOTH
            t->x += v->vx * dt;
            t->y += v->vy * dt;
        }
    }
}

int main() {
    std::vector<Entity> entities;
    entities.reserve(2);  // keep references valid as the vector grows (see exercise 04)

    Entity& mover = entities.emplace_back();
    mover.add(Transform{0.0f, 0.0f});
    mover.add(Velocity{10.0f, -4.0f});

    Entity& stuck = entities.emplace_back();
    stuck.add(Transform{100.0f, 100.0f});

    movement_system(entities, 0.5f);

    assert(entities[0].get<Transform>().x == 5.0f);
    assert(entities[0].get<Transform>().y == -2.0f);
    assert(entities[1].get<Transform>().x == 100.0f);

    std::cout << "movement system works\n";
    return 0;
}
