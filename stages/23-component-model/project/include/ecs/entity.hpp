#pragma once

// ===========================================================================
// Entity — a BAG OF COMPONENTS keyed by their C++ type.                    (YOUR TASK)
//
// This header is the STARTER: the class, every method SIGNATURE and the `components_` storage are
// given, but the method BODIES are stubbed with TODOs and placeholder returns. Implementing them
// is part of this stage's mini-project. The project still compiles and links, but the [entity]
// tests in tests/test_ecs.cpp FAIL until you fill these in. A complete reference is in
// ../../solution/include/ecs/entity.hpp — try it yourself first.
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
//   Store into the map with `components_[std::type_index(typeid(T))] = std::any(component);`,
//   look up with `.find(...)` / `.at(...)`, and pull the value back out with
//   `std::any_cast<T&>(slot)` (or `std::any_cast<T>(&slot)` for a pointer, nullptr on mismatch).
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
        // TODO(stage 23): store `component` in components_ keyed by std::type_index(typeid(T)),
        // REPLACING any existing component of type T, and return a reference to the stored copy.
        static T placeholder{};  // placeholder so the starter compiles; replace with the real store
        return placeholder;
    }

    // has<T>(): does this entity carry a component of type T? Python: `hasattr(entity, ...)`.
    template <class T>
    bool has() const {
        // TODO(stage 23): return true iff components_ has a key for std::type_index(typeid(T)).
        return false;  // placeholder
    }

    // get<T>(): reference to the component. Precondition: has<T>() — .at() throws
    // std::out_of_range otherwise. Use this when you KNOW the component is present.
    template <class T>
    T& get() {
        // TODO(stage 23): return a reference to the stored component of type T (components_.at(...)
        // throwing when it is absent is the intended behaviour).
        static T placeholder{};  // placeholder so the starter compiles
        return placeholder;
    }
    template <class T>
    const T& get() const {
        // TODO(stage 23): const overload of the above — read-only access.
        static T placeholder{};  // placeholder so the starter compiles
        return placeholder;
    }

    // try_get<T>(): pointer to the component, or nullptr if absent. This is the SYSTEM-FRIENDLY
    // accessor — a system asks "do you have what I need?" and skips the entity if not. Python:
    // `getattr(entity, "velocity", None)`.
    template <class T>
    T* try_get() {
        // TODO(stage 23): return a pointer to the stored component of type T, or nullptr if absent.
        return nullptr;  // placeholder
    }
    template <class T>
    const T* try_get() const {
        // TODO(stage 23): const overload of the above.
        return nullptr;  // placeholder
    }

    // remove<T>(): detach a component if present (no-op if it isn't).
    template <class T>
    void remove() {
        // TODO(stage 23): erase the component of type T from components_ (a no-op if not present).
    }

    std::size_t component_count() const {
        // TODO(stage 23): return how many components this entity currently holds.
        return 0;  // placeholder
    }

private:
    std::unordered_map<std::type_index, std::any> components_;
};

}  // namespace ecs
