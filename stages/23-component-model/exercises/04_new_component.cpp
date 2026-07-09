// Exercise 04 — Extend without editing (medium)
//
// The payoff over inheritance: adding a brand-new capability means adding a NEW component + a NEW
// system, without touching Entity or any existing component. Compare that to a class hierarchy,
// where "some enemies can be poisoned" tempts you to edit a shared base class everyone depends on.
//
// TODO:
//   1. Define a plain-data component:  struct Health { int hp{0}; };
//   2. Write   void poison_system(std::vector<Entity>& es, int amount)   that subtracts `amount`
//      from the hp of every entity that HAS a Health component (skip the rest).
//   3. Note you did NOT edit Entity, Transform, or Velocity to make this work.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_new_component.cpp -o ex && ./ex

#include <any>
#include <cassert>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

// --- existing code you must NOT modify -------------------------------------------------------
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
// --- end of untouchable code -----------------------------------------------------------------

// TODO 1: define the Health component here.

// TODO 2: define poison_system here.

int main() {
    std::vector<Entity> entities;
    // reserve() so the vector never reallocates as we add the 2 entities below — otherwise the
    // `hero` reference would dangle after `rock` is emplaced (a classic C++ gotcha: growing a
    // vector can move its elements). Python lists never do this to you; C++ references can.
    entities.reserve(2);

    Entity& hero = entities.emplace_back();
    hero.add(Transform{0.0f, 0.0f});
    // hero.add(Health{100});   // <- uncomment once Health exists

    Entity& rock = entities.emplace_back();  // scenery: a Transform, but no Health
    rock.add(Transform{10.0f, 10.0f});

    // poison_system(entities, 30);           // <- uncomment once poison_system exists
    // assert(hero.get<Health>().hp == 70);
    // assert(!rock.has<Health>());

    std::cout << "extended without editing existing components\n";
    return 0;
}
