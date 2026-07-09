# Coding style & conventions

All code in this course follows one consistent style so the examples read the same from stage 00
to stage 27. You don't need to memorize this — the **tools enforce most of it automatically**
(introduced formally in [stage 05](stages/05-cmake-and-debugging-basics/)):

- **`clang-format`** auto-formats layout (indentation, braces, spacing). It's the C++ equivalent
  of Python's `black`. Config: [`.clang-format`](.clang-format).
- **`clang-tidy`** is a linter / static analyzer that flags bug-prone or non-idiomatic code. It's
  the C++ equivalent of `flake8` / `pylint`. Config: [`.clang-tidy`](.clang-tidy).

Run them from a stage's build directory once you're on stage 05+:

```bash
clang-format -i src/*.cpp src/*.hpp      # reformat in place
clang-tidy src/*.cpp -- -std=c++17       # lint (needs the compile flags after --)
```

---

## Naming

| Kind | Convention | Example |
|------|-----------|---------|
| Types (`class`, `struct`, `enum class`) | `PascalCase` | `Character`, `Vec2`, `DamageType` |
| Functions & methods | `snake_case` | `take_damage`, `roll_dice` |
| Local variables & parameters | `snake_case` | `int hit_points`, `const Vec2& target` |
| Private/protected data members | `snake_case_` (trailing underscore) | `int hp_;`, `std::string name_;` |
| Constants / `constexpr` | `PascalCase` or `kCamelCase` | `constexpr int MaxHp = 100;` |
| Namespaces | lowercase | `namespace eng { ... }` |
| Files | `snake_case` | `math_utils.hpp`, `game_state.cpp` |

> Trailing-underscore members (`hp_`) make it obvious at a glance whether you're touching a
> member or a local, and avoid clashing with a constructor parameter of the same name:
> `Character(int hp) : hp_(hp) {}`.

---

## Formatting (enforced by `.clang-format`)

- **Indent:** 4 spaces, no tabs.
- **Braces:** opening brace on the same line (`if (x) {`), always brace control-flow bodies —
  even one-liners.
- **Line length:** ~100 columns.
- **Pointers/references:** bind to the type — `int* p`, `const Vec2& v` (not `int *p`).

---

## Conventions this course insists on

These are *habits*, taught stage by stage, that a linter alone won't instill:

- **`const`-correctness** — mark anything that shouldn't change `const`: local values, parameters
  passed by reference (`const T&`), and member functions that don't mutate (`int hp() const`).
  Reinforced across stages 01 → 02 → 07 → 08+.
- **Pass big objects by `const&`, small ones by value** — copying a `std::string` or a container
  on every call is wasteful; copying an `int` is free. (Stage 02.)
- **RAII over manual cleanup** — let destructors release resources (files, memory, textures)
  instead of remembering to close/free by hand. (Stages 07, 10, 11.)
- **Prefer `enum class` to bare `enum`** — scoped, typed, no implicit int conversions. (Stage 03.)
- **Include what you use** — every file `#include`s the headers for what it references, and uses
  `#pragma once` at the top of every header. (Stage 04.)
- **No `using namespace std;` in headers** — it leaks into every file that includes them. In
  `.cpp` files it's tolerated for brevity in this course; real engine code qualifies (`std::`).

---

## Comments

- Explain **why**, not **what** — the code already says what it does.
- Each stage's teaching code is deliberately commented more heavily than production code would be,
  because the comments *are* part of the lesson.
