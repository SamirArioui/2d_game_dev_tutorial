// Exercise 02 — Build the component bag (medium)
//
// The core of the whole stage: an Entity that stores components keyed by their C++ TYPE, using
// std::any (a box that holds any type) and std::type_index (a hashable handle for a type).
//
// Python comparison: `entity.transform = Transform(...)` then `hasattr(entity, "transform")` —
// only here the "attribute name" is a type, checked by the compiler.
//
// TODO: implement the three methods marked below so the asserts pass.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_component_bag.cpp -o ex && ./ex

#include <any>
#include <cassert>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <utility>

// Two plain-data components.
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
    // add<T>: store (or replace) the component of type T; return a reference to the stored copy.
    template <class T>
    T& add(T component) {
        // TODO: put `component` into components_ under the key std::type_index(typeid(T)),
        //       then return it via std::any_cast<T&>(...). (Move `component` in to avoid a copy.)
        (void)component;
        static T placeholder{};  // replace this
        return placeholder;
    }

    // has<T>: is there a component of type T?
    template <class T>
    bool has() const {
        // TODO: return whether the key std::type_index(typeid(T)) is present.
        return false;
    }

    // get<T>: reference to the component (assume it exists).
    template <class T>
    T& get() {
        // TODO: return std::any_cast<T&>(components_.at(std::type_index(typeid(T))));
        static T placeholder{};  // replace this
        return placeholder;
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

    // add again replaces (same key), it does not duplicate.
    e.add(Transform{9.0f, 9.0f});
    assert(e.get<Transform>().x == 9.0f);

    std::cout << "component bag works\n";
    return 0;
}
