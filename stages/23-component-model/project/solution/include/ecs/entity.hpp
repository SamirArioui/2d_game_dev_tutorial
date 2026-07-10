#pragma once

// ===========================================================================
// Entity — a BAG OF COMPONENTS keyed by their C++ type.
//
// Python mental model: in Python you bolt attributes onto an object at run time —
//     ball.transform = Transform(); ball.velocity = Velocity()
// and later ask `hasattr(ball, "velocity")`. This Entity does exactly that, except the
// "attribute name" is a C++ TYPE (Transform, Velocity, ...) instead of a string, so the compiler
// type-checks every access for you.
//
// How it works under the hood (two standard-library tools you may not have met yet):
//   - std::any        — a type-erased box that can hold a value of ANY type (C++17's closest
//                       thing to "a Python variable can hold anything").
//   - std::type_index — a small, hashable, comparable handle for a type, produced from typeid(T).
//                       We use it as the map key, so each component type appears at most once.
//
// This is "component-OOP", not a full ECS: components live INSIDE each entity. It is deliberately
// simple and clear — the capstone game (stage 27) builds on this exact shape, and stage 26 lifts
// it into `namespace eng`.
// ===========================================================================

#include <any>
#include <cstddef>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace ecs {

class Entity {
public:
    // add<T>(component): attach (or replace) the component of type T; returns a reference to the
    // stored copy so you can keep tweaking it. Python: `entity.transform = Transform(...)`.
    template <class T>
    T& add(T component) {
        std::any& slot = components_[std::type_index(typeid(T))];
        slot = std::move(component);
        return std::any_cast<T&>(slot);
    }

    // has<T>(): does this entity carry a component of type T? Python: `hasattr(entity, ...)`.
    template <class T>
    bool has() const {
        return components_.find(std::type_index(typeid(T))) != components_.end();
    }

    // get<T>(): reference to the component. Precondition: has<T>() — .at() throws
    // std::out_of_range otherwise. Use this when you KNOW the component is present.
    template <class T>
    T& get() {
        return std::any_cast<T&>(components_.at(std::type_index(typeid(T))));
    }
    template <class T>
    const T& get() const {
        return std::any_cast<const T&>(components_.at(std::type_index(typeid(T))));
    }

    // try_get<T>(): pointer to the component, or nullptr if absent. This is the SYSTEM-FRIENDLY
    // accessor — a system asks "do you have what I need?" and skips the entity if not. Python:
    // `getattr(entity, "velocity", None)`.
    template <class T>
    T* try_get() {
        auto it = components_.find(std::type_index(typeid(T)));
        return it == components_.end() ? nullptr : std::any_cast<T>(&it->second);
    }
    template <class T>
    const T* try_get() const {
        auto it = components_.find(std::type_index(typeid(T)));
        return it == components_.end() ? nullptr : std::any_cast<T>(&it->second);
    }

    // remove<T>(): detach a component if present (no-op if it isn't).
    template <class T>
    void remove() {
        components_.erase(std::type_index(typeid(T)));
    }

    std::size_t component_count() const { return components_.size(); }

private:
    std::unordered_map<std::type_index, std::any> components_;
};

}  // namespace ecs
