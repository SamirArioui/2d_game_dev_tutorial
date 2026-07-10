# Stage 11 — Smart pointers, move & the rule of 0/3/5

**Goal:** own heap memory *safely*, without ever writing `delete`. In stage 06 you saw raw `new`/
`delete` and how easy it is to leak or double-free. This stage replaces that with **smart pointers**
— objects that own memory and free it via RAII — and teaches the two ownership models (`unique_ptr`,
`shared_ptr`), how to *transfer* ownership with `std::move`, and the **rule of 0/3/5** that governs a
class owning a resource.

**New concepts:** `std::unique_ptr` (exclusive ownership) · `std::shared_ptr` (shared ownership +
reference counting) · `std::move` and move semantics · the rule of 0/3/5 · virtual destructors in a
`vector<unique_ptr<Base>>`.

**Python throughline:** Python objects live on the heap and CPython **reference-counts** them (plus
a cycle-collecting GC); memory management is invisible. C++ makes **ownership explicit** — you choose
*who* owns each object and *when* it dies. `shared_ptr` is literally Python's refcounting, but opt-in.

---

## 1. The problem smart pointers solve

Raw heap memory in C++ is a manual contract you must not break:

```cpp
Enemy* e = new Enemy("Goblin");   // you allocated it...
// ... 200 lines, three early returns, one exception later ...
delete e;                         // ...and YOU must free it, exactly once, on every path
```

Miss the `delete` → leak. `delete` twice → undefined behaviour. `delete` then use → dangling pointer
(stage 06's crashes). A **smart pointer** wraps the raw pointer in an object whose *destructor* does
the `delete` for you — RAII (stage 07/10) applied to memory. You get automatic, exactly-once cleanup
on every path, including exceptions.

> **Rule for this course from here on:** never write bare `new`/`delete`. Use `std::make_unique` /
> `std::make_shared`, or a container that owns its elements.

---

## 2. `std::unique_ptr` — one exclusive owner (your default)

`std::unique_ptr<T>` owns a single heap object. Exactly **one** unique_ptr owns it at a time; when
that unique_ptr dies, the object is deleted. Create one with `std::make_unique`:

```cpp
#include <memory>
auto goblin = std::make_unique<Enemy>("Goblin", 12);   // heap Enemy, owned by `goblin`
goblin->take_damage(5);                                 // use it exactly like a pointer
// no delete — when `goblin` goes out of scope, ~Enemy runs automatically
```

A `unique_ptr` **cannot be copied** — two owners would both delete the object. This is enforced at
compile time:

```cpp
auto copy = goblin;              // ❌ compile error: unique_ptr's copy is deleted
```

That restriction is the feature: the type system guarantees single ownership. To hand the object to
someone else, you **move** it (next section).

---

## 3. `std::move` and move semantics

To transfer a unique_ptr's ownership, use **`std::move`**:

```cpp
auto a = std::make_unique<Enemy>("Orc", 20);
auto b = std::move(a);    // ownership moves a -> b; `a` is now empty (nullptr)
```

`std::move` does **not** move anything by itself — it's a **cast** that marks its argument as "you
may pluck its guts out". The actual transfer is done by the type's **move constructor** / **move
assignment**. For a unique_ptr, the move just copies the raw pointer over and nulls the source, so
after the move `a` owns nothing and `b` owns the Orc. Moving is cheap; copying (where allowed) is a
deep copy.

```
Before:  a ●──▶ [Orc]        b (empty)
move:    a (empty)           b ●──▶ [Orc]
```

Python has no direct equivalent — names are just references, and rebinding one doesn't "empty" the
other. In C++, moving a unique_ptr genuinely transfers a unique resource.

> **Why moves matter for performance:** returning a big `std::vector` or `std::string` by value used
> to imply a copy. With move semantics the compiler moves it (steals the internal buffer) instead —
> so "return by value" is both safe and cheap. You've been benefiting from this since stage 09.

---

## 4. `std::shared_ptr` — shared ownership by reference count

Sometimes one object genuinely has **several** owners and should live until the *last* one is done —
e.g. a texture used by many sprites. `std::shared_ptr<T>` keeps a **reference count**: copying it
adds an owner, destroying a copy removes one, and the object is deleted when the count hits **zero**.

```cpp
auto tex = std::make_shared<Texture>("hero.png");   // use_count() == 1
auto also = tex;                                     // COPY is fine -> use_count() == 2
// Texture is freed only when BOTH tex and also are gone
```

This is exactly CPython's model — every object carries a refcount, incremented and decremented as
references come and go — except in C++ you **opt in** to it (`shared_ptr`) and pay its cost only
where you need shared ownership.

| | `unique_ptr` | `shared_ptr` |
|---|---|---|
| owners | exactly one | many (counted) |
| copyable? | no (move only) | yes (copy bumps the count) |
| overhead | none beyond a raw pointer | a heap control block + atomic refcount |
| use when | one clear owner (the default) | ownership is genuinely shared |

**Prefer `unique_ptr`.** Reach for `shared_ptr` only when you truly can't name a single owner. (And
beware *cycles*: two `shared_ptr`s pointing at each other never reach count zero — the leak
`std::weak_ptr` exists to break. Out of scope here, noted for later.)

---

## 5. The rule of 0 / 3 / 5

A class has up to **five special member functions** the compiler can generate: destructor, copy
constructor, copy assignment, move constructor, move assignment. The rule tells you how many to
write:

- **Rule of 0** — *own nothing raw.* Hold members that already manage themselves (`std::string`,
  `std::vector`, `std::unique_ptr`, `std::shared_ptr`) and write **none** of the five; the compiler's
  defaults are correct. **This is what you should do almost always.**
- **Rule of 3** — if a class owns a raw resource so that it needs a custom **destructor**, it almost
  certainly also needs a custom **copy constructor** and **copy assignment** (else the default
  shallow copy duplicates the raw pointer and double-frees). Pre-C++11.
- **Rule of 5** — C++11 adds the **move constructor** and **move assignment** for cheap ownership
  transfer. If you're writing the other three, add these two so your type moves efficiently.

```cpp
struct Rule0 {                 // ✅ owns nothing raw -> writes zero special members
    std::string name;
    std::unique_ptr<Enemy> boss;
    std::vector<int> scores;
};                             // copy/move/destroy all handled correctly by defaults
```

The exercise and mini-project build a raw-owning class (`ScopedResource`) whose five members each
**log**, so you can watch which one fires for `x = y;` vs `x = std::move(y);` vs leaving a scope.
Seeing it once is what makes it click — after that you'll happily follow the rule of 0 and let
`unique_ptr` do this work for you.

---

## 6. The entity pattern: `vector<unique_ptr<Base>>`

Games keep their entities in a list. In C++ the idiomatic list of polymorphic, owned objects is:

```cpp
std::vector<std::unique_ptr<Character>> entities;
entities.push_back(std::make_unique<Enemy>("Goblin", 12, 4, sprite));
for (const auto& e : entities) { e->attack(); }   // virtual dispatch (stage 07)
```

Two things make this work:

- **`unique_ptr<Character>` holds a derived `Enemy`** without *slicing*. A `std::vector<Character>`
  would store Characters by value and chop off the Enemy part; a pointer to the base keeps the whole
  object and enables virtual dispatch.
- **The base needs a `virtual` destructor.** Deleting an `Enemy` through a `Character*` only runs
  `~Enemy` (then `~Character`) if `~Character` is `virtual`. Forget it and you get undefined
  behaviour. (You'll see both destructors fire in exercise 05.)

When the vector is destroyed, every `unique_ptr` deletes its entity — no cleanup loop required.

---

## Building and running

This stage uses **CMake**. From this folder:

```bash
cmake -S . -B build          # configure into ./build
cmake --build build          # compile the project + every exercise solution
./build/stage11_entities     # run the mini-project
```

Exercise solutions build as `s11_<name>` (e.g. `./build/s11_04_rule_of_five`). Or compile one by
hand:

```bash
clang++ -std=c++17 -Wall -Wextra exercises/04_rule_of_five.solution.cpp -o ex && ./ex
```

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference solution. Ordered easy → hard.

1. **`01_unique_ptr`** *(easy)* — `make_unique`, access with `->`, and watch the destructor run with
   no manual `delete`.
2. **`02_move_unique`** *(medium)* — transfer ownership with `std::move`; observe the source go
   empty; see why a copy is a compile error.
3. **`03_shared_ptr`** *(medium)* — shared ownership; watch `use_count()` rise and fall as copies
   enter and leave scopes.
4. **`04_rule_of_five`** *(medium–hard)* — a raw-owning class that logs every special member. The
   destructor + copy operations (rule of 3) are given; **you add the two move operations** to reach
   the rule of 5, then watch "copy" become "move".
5. **`05_polymorphic_container`** *(medium–hard)* — a `std::vector<std::unique_ptr<Character>>` with a
   derived `Enemy`, virtual dispatch, and a virtual destructor.

Try each yourself before peeking at the solution.

---

## Mini-project — `unique_ptr` entities sharing a `shared_ptr` resource

**Your task.** The starters in [`project/src/`](project/src/) have `TODO`s — you implement the demo
in [`main.cpp`](project/src/main.cpp) and the rule-of-5 class in
[`scoped_resource.cpp`](project/src/scoped_resource.cpp). (`ScopedResource` is declared in
`scoped_resource.hpp`; the `Character` / `Enemy` / `SpriteStub` types are provided for you in
`entity.hpp`.) It brings the whole stage together on the course's `Character`/`Enemy` types:

- entities live in a **`std::vector<std::unique_ptr<Character>>`** — the vector exclusively owns each
  one, and `describe()` dispatches virtually (`Character` follows the **rule of 0**: it holds a
  `std::string`, ints, and a `shared_ptr`, so it declares none of the five special members);
- several enemies **share one `std::shared_ptr<SpriteStub>`** — the demo prints `use_count()` climbing
  to 4 as goblins reference it and falling back to 1 when the entities are released;
- a named `unique_ptr` is handed into the vector with **`std::move`** (source left empty);
- **`ScopedResource`** is a **rule-of-5** class owning a raw handle, with every special member
  **logging**, so copy/move/destroy are observable.

Once your implementation is complete it prints (abridged):

```
=== Shared sprite (shared_ptr) ===
goblin.png owners: 1 (just the loader)
...
goblin.png owners now: 4 (loader + Goblin + Orc + Slime)
  Aria (hero, hp 30, atk 12, sprite 'hero.png')
  Goblin (enemy, hp 12, atk 4, sprite 'goblin.png')
  ...
goblin.png owners after clear: 1 (back to just the loader)

=== Rule of 5, observed (ScopedResource) ===
  [ScopedResource] ctor 'sound' (handle 100)
  [ScopedResource] copy ctor 'sound' (handle 100)
  [ScopedResource] move ctor 'sound' (handle 100)
  ...
  [ScopedResource] move assign 'sound' (handle 100)
  leaving scope -> destructors run in reverse (d, c, b, a):
  ...
```

```bash
cmake -S . -B build && cmake --build build
./build/stage11_entities
```

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone and
compare, but try it yourself first:

```bash
cmake -S project/solution -B build-solution && cmake --build build-solution
./build-solution/stage11_entities
```

This is the ownership model the engine stages build on: the world owns entities via
`unique_ptr`, and shared assets (textures, fonts, sounds) are handed around as `shared_ptr` — which
is exactly what the resource manager in stage 25 formalizes.

---

## Checklist before moving on

- [ ] I never write bare `new`/`delete` anymore; I reach for `make_unique`/`make_shared`.
- [ ] I can explain why a `unique_ptr` can't be copied, only moved, and what `std::move` actually does.
- [ ] I can read `shared_ptr::use_count()` and predict when the object is finally freed.
- [ ] I can state the rule of 0/3/5 and know rule-of-0 is the goal (let members manage themselves).
- [ ] I know a `vector<unique_ptr<Base>>` needs a **virtual destructor** on `Base`, and why.
- [ ] I implemented the mini-project (`project/src/main.cpp` + `scoped_resource.cpp`) and watched the
      shared count and the rule-of-5 log.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference on `<memory>`; the
C++ Core Guidelines section on resource management (R.*) is excellent and short.

Next up: [Stage 12 — Multi-target CMake + FetchContent](../12-cmake-multitarget/).
