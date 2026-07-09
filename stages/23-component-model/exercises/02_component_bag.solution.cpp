// Exercise 02 — Build the component bag (solution)
#include <any>
#include <cassert>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <utility>

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
    T& add(T component) {
        std::any& slot = components_[std::type_index(typeid(T))];
        slot = std::move(component);
        return std::any_cast<T&>(slot);
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

int main() {
    Entity e;
    assert(!e.has<Transform>());

    e.add(Transform{3.0f, 4.0f});
    e.add(Velocity{1.0f, 0.0f});

    assert(e.has<Transform>());
    assert(e.has<Velocity>());
    assert(e.get<Transform>().x == 3.0f);

    e.add(Transform{9.0f, 9.0f});
    assert(e.get<Transform>().x == 9.0f);

    std::cout << "component bag works\n";
    return 0;
}
