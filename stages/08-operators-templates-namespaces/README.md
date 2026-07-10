# Stage 08 — Operator overloading, templates & namespaces

**Goal:** build the small math library the rest of the course rests on — `gmath::Vec2<T>` — and
in the process learn the three C++ features that make it possible: **operator overloading** (so
`a + b` works on your own types), **templates** (write code once, get a type-checked version for
every type), and **namespaces** (so your names don't collide with anyone else's).

**New concepts:** operator overloading (`operator+`, `operator-`, `operator*`, `operator==`,
`operator<<`) · **function templates** and **class templates** (`template <class T>`, argument
deduction) · **namespaces** (`namespace`, qualified names, `using`).

> Each maps onto something you already know from Python: operator overloading is *dunder methods*
> (`__add__`, `__eq__`, `__repr__`), templates are the compile-time, statically-typed cousin of
> *duck typing*, and namespaces play the "who owns this name" role of *modules*.

---

## 1. Operator overloading — dunder methods, renamed

In Python you make `+` work on your class by defining `__add__`:

```python
class Vec2:
    def __add__(self, other):        # a + b  calls  a.__add__(b)
        return Vec2(self.x + other.x, self.y + other.y)
    def __eq__(self, other): ...     # ==
    def __repr__(self): ...          # what print() shows
```

C++ is the same idea with different spelling: you define a function literally named
`operator+` (or `operator==`, `operator<<`, …). Writing `a + b` calls it.

```cpp
struct Vec2f {
    float x, y;
    // a + b  calls  a.operator+(b). const method (reads *this), other by const& (no copy),
    // returns a NEW value.
    Vec2f operator+(const Vec2f& other) const { return Vec2f{x + other.x, y + other.y}; }
};
```

| Python dunder | C++ operator | This stage |
|---------------|-------------|------------|
| `__add__` | `operator+` | Vec2 + / exercise 01 |
| `__sub__` | `operator-` | Vec2 - |
| `__mul__` | `operator*` | Vec2 * scalar |
| `__eq__` | `operator==` | Vec2 == / exercise 02 |
| `__repr__` / `__str__` | `operator<<` (free function) | exercise 03 |

Two style rules this stage follows:

- **Symmetric read-only operators are `const` methods taking `const&`** and returning a *new*
  value (`operator+` doesn't modify either operand).
- **`operator<<` is a *free* function, not a member** — its left operand is the stream
  (`std::ostream&`), not your type, so it can't be a member of your type. It returns the stream by
  reference so `cout << a << b` chains (exercise 03).

> **Don't overload for cleverness.** Operators should mean the obvious thing. We deliberately do
> *not* define `Vec2 * Vec2` — there's no single obvious meaning (dot? component-wise?), so a
> reader couldn't guess it. `Vec2 * scalar` is unambiguous, so that's the only `*` we define.

---

## 2. Templates — duck typing, checked at compile time

Python functions are generic for free: `max_of(a, b)` works on anything with `<`, decided at run
time. C++ gets the same reuse *without* giving up static typing, using **templates**. `T` is a
type parameter; the compiler stamps out a separate, fully type-checked copy for each type you use.

### Function templates

```cpp
template <class T>          // "for some type T ..."
T max_of(T a, T b) {
    return (a < b) ? b : a; // the only requirement on T is that it supports operator<
}

max_of(3, 9);      // compiler DEDUCES T = int    -> stamps out max_of<int>
max_of(2.5, 1.5);  // compiler DEDUCES T = double -> stamps out max_of<double>
```

You don't write `max_of<int>(...)`; the compiler figures `T` out from the arguments (*template
argument deduction*). If you call it with a type that has no `<`, you get a **compile-time** error
at the point the operator is used — the duck-typing failure Python would only find at run time.

### Class templates

The same idea for a whole class. `Vec2<T>` is one definition serving `Vec2<float>`, `Vec2<int>`,
`Vec2<double>`, …:

```cpp
template <class T>
struct Vec2 { T x, y; /* operators ... */ };

using Vec2f = Vec2<float>;   // a type ALIAS: Vec2f and Vec2<float> are the same type
```

Exercise 05 has you write a `Range<T>` class template; the mini-project is the real `Vec2<T>`.

### Why templates live in headers

A template isn't code until it's *instantiated* with a concrete type. The compiler therefore
needs to see the **full definition** wherever you use it — so templates go in headers (`.hpp`),
not split into a `.cpp` like ordinary functions (stage 04). That's why `vec2.hpp` is
"header-only": there is no `vec2.cpp`.

---

## 3. Namespaces — modules for names

As a program grows, names collide: two libraries might both define `reset()` or `Vec2`. A
**namespace** groups related names under a prefix so they don't clash:

```cpp
namespace gmath {
    template <class T> struct Vec2 { ... };
    template <class T> T clamp(T v, T lo, T hi) { ... };
}

gmath::Vec2<float> p;        // qualified name, like gmath.Vec2 after `import gmath`
auto q = gmath::clamp(5, 0, 10);
```

Ways to use a name from a namespace, and their Python analogues:

| C++ | Python | Notes |
|-----|--------|-------|
| `gmath::clamp(...)` | `gmath.clamp(...)` | fully qualified — always unambiguous |
| `using gmath::clamp;` then `clamp(...)` | `from gmath import clamp` | brings in one name |
| `using namespace gmath;` | `from gmath import *` | brings in everything — **never in a header** (it leaks into every file that includes it; see `STYLE.md`) |

Unlike a Python module, a C++ namespace doesn't correspond to a file and doesn't control
compilation — it purely organises *names*. The whole course's engine code will live in
`namespace eng` (stage 26); the math lives in `gmath` starting now.

---

## 4. Building and running

This is a **CMake** stage. `vec2.hpp` is header-only, so the mini-project is a single `.cpp` that
includes it:

```bash
cmake -S . -B build
cmake --build build

./build/vec2_demo           # the mini-project

# each exercise solution is a target too:
./build/ex_04_function_template
```

> **Compiler note:** if your default compiler struggles, point CMake at a specific one, e.g.
> `cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++`.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference solution. Ordered easy → hard:

1. **`01_operator_plus`** *(easy)* — overload `operator+` for a `Color` struct (Python's
   `__add__`).
2. **`02_equality`** *(medium)* — overload `operator==`, then define `operator!=` in terms of it
   so they can't disagree (`__eq__`).
3. **`03_stream_output`** *(medium)* — overload `operator<<` as a *free* function so
   `std::cout << v` prints your type, and return the stream so calls chain (`__repr__`).
4. **`04_function_template`** *(medium)* — write `max_of<T>` and `clamp<T>`; see the compiler
   deduce `T` and stamp out an `int` and a `double` version of each.
5. **`05_class_template`** *(medium–hard)* — write a `Range<T>` class template with `const`
   methods, used as `Range<int>` and `Range<double>`.
6. **`06_namespaces`** *(medium)* — two `volume()` functions in different namespaces; call them
   qualified, then pull one in with a `using` declaration.

Try each yourself before peeking at the solution.

---

## Your task — `gmath::Vec2<T>` and `gmath::clamp<T>`

[`project/include/vec2.hpp`](project/include/vec2.hpp) is a **starter**: the declarations are
given (the `Vec2<T>` struct with `T x, y;`, every operator/method **signature**, the `Vec2f`
alias, and the `clamp<T>` signature) but the bodies are stubbed with `TODO`s — implementing them
is your task. The driver [`project/src/main.cpp`](project/src/main.cpp) is complete and you do not
edit it; it exercises what you implement. This is the **canonical `gmath` math library** the
graphics and physics stages build on, so its shape is fixed:

- **`gmath::Vec2<T>`** — a class template with members `T x, y;`, the operators `operator+`,
  `operator-`, `operator*` (by scalar), and `operator==`, plus the alias `using Vec2f =
  Vec2<float>;`.
- **`gmath::clamp<T>`** — a function template `T clamp(T v, T lo, T hi)`, a general scalar utility
  (not a `Vec2` method).

The library is kept deliberately minimal — **stage 15 adds `dot`/`length`/`normalize`/`lerp`** and
bridges it to `sf::Vector2f`, and **stage 13 unit-tests it**. `main.cpp` exercises `Vec2f`
arithmetic and calls the one `clamp` template on `int`, `float`, and `double`.

Build & run your version (it compiles from the start; the numbers are wrong until you implement
`vec2.hpp`):

```bash
cmake -S . -B build && cmake --build build
./build/vec2_demo
```

Expected output once implemented:

```
player   = (1, 2)
velocity = (3, -0.5)
next     = (7, 1)
back == player? yes

clamp(15, 0, 10)        = 10
clamp(-3.5f, 0.f, 1.f)  = 0
clamp(0.42, 0.0, 1.0)   = 0.42
```

(`next = player + velocity * dt` with `dt = 2`, and `back = next - velocity * dt` returns exactly
to `player`, so `==` is `yes`. The same `clamp` template serves all three numeric types.)

Stuck? A complete reference is in [`project/solution/`](project/solution/) — it is self-contained
with its own `CMakeLists.txt`. Try it yourself first, then compare:

```bash
cmake -S project/solution -B build-solution && cmake --build build-solution
./build-solution/vec2_demo
```

---

## Checklist before moving on

- [ ] I can overload an operator (`+`, `==`) as a member and `operator<<` as a free function, and
      I know why `<<` can't be a member.
- [ ] I can write a **function template** and a **class template**, and explain how the compiler
      deduces `T` and instantiates a checked copy per type.
- [ ] I know why templates live in headers.
- [ ] I can put code in a `namespace` and access it three ways (qualified, `using` declaration,
      `using namespace`), and I know why the last one doesn't belong in a header.
- [ ] I implemented the mini-project starter (`project/include/vec2.hpp`) so my `gmath::Vec2<T>`
      matches the course contract (the four operators + `Vec2f` alias) and I did **not** add extra
      vector operations — those come in stage 15.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference on operator overloading,
templates, and namespaces; "Back to Basics: Templates" on CppCon. Paste `Vec2<T>` into godbolt.org
and watch a `Vec2<int>` and a `Vec2<float>` get generated as separate types.

Next up: [Stage 09 — STL, lambdas & `std::function`](../09-stl-and-lambdas/), where the standard
library's containers and algorithms let you stop hand-rolling data structures like the stage-06
linked list.
