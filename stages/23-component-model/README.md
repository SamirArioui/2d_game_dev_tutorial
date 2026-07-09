# Stage 23 — From objects to a component model

**Goal:** take the "everything is a class in a big inheritance tree" instinct you brought from
early OOP and replace it with the model real game engines use: entities that are **bags of
plain-data components**, with behaviour living in **systems**. You'll build a tiny, clear
component framework and re-express Pong's paddles and ball on top of it.

**New concepts:** why deep inheritance hierarchies break down in games · composition over
inheritance · **components** (plain-data structs) · an **entity** as a typed component bag
(`std::any` + `std::type_index`) · **systems** (functions that query entities) · **ECS vs
component-OOP**, and where **EnTT** fits. (*Python comparison:* mixins and `@dataclass` composition
vs. subclassing.)

This is the first stage of Phase H (architecture & engine). The `Entity`/component shape you build
here is a **shared contract**: stage 26 lifts it into `namespace eng`, and the capstone (stage 27)
is built on it. So we keep it small and legible on purpose.

---

## 1. Why not just use inheritance?

The natural first design for game objects is a class tree:

```
        GameObject
        /    |     \
   Paddle   Ball   Enemy
                     |
                 FlyingEnemy
```

It works right up until real requirements arrive. Now you want an enemy that also drops loot, a
paddle that can be temporarily controlled by AI, a pickup that flies *and* explodes. Behaviours
that don't fit the tree force you into bad choices:

- **The fragile base class.** Everyone shares `GameObject`, so adding "can take damage" there
  bloats objects that have no health (walls, the ball). Change the base and you risk breaking
  every subclass.
- **The diamond / "I need bits of two branches."** A `FlyingExplodingPickup` wants code from
  unrelated branches. Single inheritance can't express it; multiple inheritance invites the
  classic diamond ambiguity.
- **The "gorilla/banana" problem** (Joe Armstrong's line): you wanted the banana (one behaviour)
  but inheritance hands you the gorilla holding it and the entire jungle (the whole base chain).

Games hit this *hard* because game objects are combinatorial: any mix of "moves", "is drawn", "has
health", "collides", "is controlled by the player" can occur. Inheritance models **is-a**;
game objects are better described by **has-a**.

### Python comparison

You've felt this in Python too. The Pythonic answers are **composition** and **mixins**:

```python
# Inheritance (rigid): behaviour is baked into the type hierarchy.
class FlyingExplodingPickup(FlyingThing, ExplodingThing, Pickup):  # MRO headaches
    ...

# Composition (flexible): the object HAS capabilities as attributes/data.
@dataclass
class Entity:
    transform: Transform | None = None
    velocity:  Velocity  | None = None
    health:    Health    | None = None
```

"Favour composition over inheritance" is the same advice in both languages. This stage is the C++,
game-shaped version of it.

---

## 2. The component model

Three ideas, each deliberately simple:

1. **Component = plain data.** A component is a `struct` with fields and *no behaviour*:
   `Transform { pos }`, `Velocity { vel }`, `Shape { size, colour }`. See
   [`project/include/ecs/component.hpp`](project/include/ecs/component.hpp).
2. **Entity = a bag of components, keyed by type.** An `Entity` can hold at most one component of
   each type, and you attach/query them by their C++ type:
   `entity.add(Velocity{...})`, `entity.has<Velocity>()`, `entity.get<Transform>()`. See
   [`project/include/ecs/entity.hpp`](project/include/ecs/entity.hpp).
3. **System = a function that acts on the data.** A system walks the entities, picks the ones that
   have the components it needs, and does one job: `movement_system` moves everything with a
   `Transform` **and** a `Velocity`. See [`project/include/game/systems.hpp`](project/include/game/systems.hpp).

Behaviour is grouped by **what it does** (one movement system), not scattered across **what it's
on** (an `update()` in every class). Adding a capability = adding a component + a system, touching
nothing that already works (you'll prove this in exercise 04).

### How the entity stores components

```cpp
// entity.hpp (trimmed — read the file for the full comments)
class Entity {
    std::unordered_map<std::type_index, std::any> components_;
public:
    template <class T> T&   add(T c);      // entity.add(Velocity{...})
    template <class T> bool has() const;   // entity.has<Velocity>()
    template <class T> T&   get();          // entity.get<Transform>()   (must exist)
    template <class T> T*   try_get();      // nullptr if absent   <-- systems use this
};
```

Two standard-library tools make this work without a shared base class:

- **`std::any`** — a box that can hold a value of *any* type (C++17). It's the closest C++ has to
  "a Python variable can hold anything". We store each component inside one.
- **`std::type_index`** — a hashable, comparable handle for a type, produced by `typeid(T)`. We use
  it as the map key, so each component *type* gets one slot.

`entity.add(Velocity{...})` reads like `entity.velocity = Velocity(...)` in Python, and
`entity.has<Velocity>()` like `hasattr(entity, "velocity")` — except the "attribute name" is a
**type checked by the compiler**, so a typo is a build error, not a runtime `AttributeError`.

> **`const`-correctness (a course thread).** `has()`, `get() const`, and `try_get() const` are
> `const` members — asking an entity what it has never changes it. `movement_system` takes
> `std::vector<Entity>&` (non-const: it mutates), while a render pass takes `const&`.

---

## 3. Systems and the "query"

A system's defining move is the **query**: *skip any entity that doesn't have the components I
need.* `try_get<T>()` returns a pointer (or `nullptr`), which is exactly the right shape:

```cpp
void movement_system(std::vector<Entity>& entities, float dt) {
    for (auto& e : entities) {
        auto* t = e.try_get<Transform>();
        auto* v = e.try_get<Velocity>();
        if (t && v) {              // the query: needs BOTH
            t->pos += v->vel * dt; // integrate position (dt is seconds, a float)
        }
    }
}
```

A paddle (Transform + Shape, no Velocity) is silently skipped by `movement_system` — it doesn't
move on its own. The ball (Transform + Velocity + Shape) is moved *and* bounced. No `if
(isPaddle)` type checks anywhere; membership is decided purely by which components an entity
carries.

---

## 4. ECS vs component-OOP (and where EnTT fits)

What you're building is **component-OOP**: components live *inside* each entity object, and you
iterate a `vector<Entity>`. It's simple, the ownership is obvious, and it's plenty for a Pong or a
small platformer.

A full **ECS** (Entity-Component-System) rearranges the memory:

- an **entity** is just an integer **id** — it owns nothing;
- **components** are stored in tightly packed arrays, *one array per component type*;
- **systems** iterate those arrays directly.

Why bother? **Cache locality and scale.** Iterating 100,000 `Velocity` values packed back-to-back
is dramatically faster than chasing 100,000 heap-allocated entities, because the CPU cache loves
contiguous memory. Full ECS also enables slick archetype queries ("every entity with A and B").

The trade-off is complexity: id lifetimes, sparse sets, archetypes. You do **not** need that to
learn the ideas, and premature ECS obscures the very concepts (component, system, query) it's
built on. So we start with component-OOP, which teaches the same vocabulary with a fraction of the
machinery.

When you *do* outgrow it, the production-grade C++ answer is **[EnTT](https://github.com/skypjack/entt)** —
a fast, header-only ECS used in real engines (and by Minecraft). Its `registry.emplace<Velocity>(e)`
/ `registry.view<Transform, Velocity>()` API is a direct, industrial-strength descendant of the
`add<T>` / query pattern you're writing here by hand. Learning the small version first makes EnTT
read like an optimisation of something you already understand.

| | component-OOP (this stage) | full ECS (EnTT) |
|---|---|---|
| entity is | an object that owns its components | an integer id |
| components stored | inside each entity (`std::any` bag) | packed arrays, one per type |
| best for | clarity, small/medium games | scale, cache performance |
| you write | a `vector<Entity>` + system functions | `registry.view<...>()` |

---

## 5. Keeping logic testable (the recurring rule)

Notice what `component.hpp`, `entity.hpp`, and `systems.hpp` have in common: **no SFML**. Colours
are stored as raw RGBA bytes, positions as `gmath::Vec2f`. Only [`project/src/main.cpp`](project/src/main.cpp)
includes SFML, and all it does is read input and draw a rectangle per entity.

That separation is why [`project/tests/test_ecs.cpp`](project/tests/test_ecs.cpp) can assert on the
whole model with no window: component storage, the movement query, the bounce reflection, and AABB
overlap are all pure functions of their inputs. Rendering (which needs a display) stays
unverified-by-execution; the *logic* is nailed down by tests. This is the same discipline the
roadmap has pushed since stage 13.

---

## Building and running

The mini-project has its own [`project/CMakeLists.txt`](project/CMakeLists.txt). It builds a pure
logic library, a Catch2 test runner, and the SFML app. **The first configure clones Catch2 (and,
for the app, SFML 2.6.1) from GitHub, so it needs network access.**

```bash
cd project

# --- Fast path: build & run ONLY the tests (no SFML, no display needed) ---
cmake -S . -B build -DBUILD_APP=OFF
cmake --build build
ctest --test-dir build --output-on-failure        # 5 tests, all green

# --- Full path: also build the SFML demo (first run clones SFML) ---
cmake -S . -B build
cmake --build build
./build/pong_ecs                                   # W/S = left paddle, Up/Down = right paddle
```

The exercises are a separate, SFML-free project:

```bash
cd exercises
cmake -S . -B build && cmake --build build
ctest --test-dir build --output-on-failure         # runs the reference solutions
# ...or compile one by hand:
clang++ -std=c++17 -Wall -Wextra 02_component_bag.cpp -o ex && ./ex
```

> **Troubleshooting.** If linking fails, add `-DCMAKE_CXX_COMPILER=/usr/bin/clang++`. On macOS, if
> the SFML configure reports *"Could NOT find OpenGL"*, your `xcrun` SDK isn't set — pass
> `-DCMAKE_OSX_SYSROOT="$(xcrun --show-sdk-path)"` (or an explicit
> `/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk`).

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; `NN_name.solution.cpp`
is the reference. Ordered easy → hard.

1. **`01_plain_oop`** *(easy)* — finish a small `GameObject`/`Ball` inheritance tree. Feel the
   shape we're about to leave behind.
2. **`02_component_bag`** *(medium)* — implement `add` / `has` / `get` on an `Entity` using
   `std::any` + `std::type_index`. This is the heart of the stage.
3. **`03_movement_system`** *(medium)* — write a system's **query**: act only on entities that have
   *both* a `Transform` and a `Velocity`.
4. **`04_new_component`** *(medium)* — add a brand-new `Health` component and a `poison_system`
   **without editing** `Entity` or any existing component — the extensibility payoff. (Also meets
   the classic *vector-reallocation invalidates references* gotcha and fixes it with `reserve`.)
5. **`05_bounce_system`** *(medium–hard)* — implement wall-reflection, the ball behaviour from the
   mini-project.

---

## Mini-project — Pong as entities + components + systems

In [`project/`](project/). The same Pong you'd build with classes, rebuilt the component way:

- two paddles and a ball are plain `ecs::Entity` objects (see `make_paddle` / `make_ball` in
  [`main.cpp`](project/src/main.cpp));
- each carries `Transform` / `Velocity` / `Shape` components;
- `movement_system`, `bounce_in_bounds`, and `aabb_overlap` (in
  [`systems.cpp`](project/src/systems.cpp)) drive them — the exact functions the tests exercise.

It **combines every concept in the stage**: plain-data components, a typed entity bag, systems with
queries, and the logic/rendering split. The paddles have no `Velocity`, so the movement system
ignores them; the ball has all three components, so it moves, bounces off the walls, and reverses
off a paddle on overlap. Input handling is intentionally minimal here — mapped, semantic input is
the whole point of **stage 24**.

Build + link is verified; the on-screen behaviour is unverified-by-execution (CI has no display).

---

## Checklist before moving on

- [ ] I can explain why deep inheritance trees get painful for game objects.
- [ ] I can state the three parts of the model: component (data), entity (typed bag), system
      (function with a query).
- [ ] I implemented `add`/`has`/`get` and understand how `std::any` + `std::type_index` store
      components by type.
- [ ] I added a new capability by adding a component + a system, editing nothing existing.
- [ ] I can describe how a full ECS differs (packed arrays, entity = id) and name EnTT as the
      grown-up version.
- [ ] My logic is SFML-free and unit-tested; only rendering touches the window.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — and EnTT's wiki for the ECS
end-state.

Next up: [Stage 24 — Scenes, state machine, input & events](../24-scenes-state-input-events/).
