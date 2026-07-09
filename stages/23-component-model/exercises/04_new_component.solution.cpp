// Exercise 04 — Extend without editing (solution)
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
    bool has() const {
        return components_.find(std::type_index(typeid(T))) != components_.end();
    }
    template <class T>
    T& get() {
        return std::any_cast<T&>(components_.at(std::type_index(typeid(T))));
    }

private:
    std::unordered_map<std::type_index, std::any> components_;
};

// New capability = new component + new system. Nothing above changed.
struct Health {
    int hp{0};
};

void poison_system(std::vector<Entity>& entities, int amount) {
    for (auto& e : entities) {
        if (auto* h = e.try_get<Health>()) {  // only things that CAN take damage
            h->hp -= amount;
        }
    }
}

int main() {
    std::vector<Entity> entities;
    // reserve() so the vector never reallocates as we add the 2 entities below — otherwise the
    // `hero` reference would dangle after `rock` is emplaced (a classic C++ gotcha: growing a
    // vector can move its elements). Python lists never do this to you; C++ references can.
    entities.reserve(2);

    Entity& hero = entities.emplace_back();
    hero.add(Transform{0.0f, 0.0f});
    hero.add(Health{100});

    Entity& rock = entities.emplace_back();
    rock.add(Transform{10.0f, 10.0f});

    poison_system(entities, 30);
    assert(hero.get<Health>().hp == 70);
    assert(!rock.has<Health>());

    std::cout << "extended without editing existing components\n";
    return 0;
}
