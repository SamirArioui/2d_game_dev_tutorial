# Roadmap — C++ → 2D Games → Game Engine

A gradual, fundamentals-first path from "I know Python" to "I built a small 2D game engine",
using **C++** and **SFML**. Every stage is a folder under [`stages/`](stages/) with:

- a detailed `README.md` (the *why*, the memory/compilation model, and Python comparisons),
- `exercises/` — 4–6 drills of gradual difficulty, each with a reference `.solution.cpp`,
- a `project/` — one mini-project that combines **all** the concepts of that stage.

New here? Read [`SETUP.md`](SETUP.md) first (install a compiler + CMake), skim
[`STYLE.md`](STYLE.md) (coding conventions), then start at
[`stages/00-hello-cpp/`](stages/00-hello-cpp/).

**28 stages (00–27), eight phases (A–H).** Stages 00–04 compile by hand with `clang++`; 05 onward use
CMake; 13 onward can run unit tests with Catch2 + `ctest`; 14 onward pull SFML (and Dear ImGui at
19) via CMake `FetchContent`.

---

## The throughline

The mini-projects tell one continuous story so each stage builds on the last:

> character sheet → dice battle → game-math lib → inventory → save/load → **Pong** →
> 2D-world tech (tilemaps / camera / particles) → engine → **capstone game** (revives inventory + save/load)

---

## Cross-cutting threads

Ideas introduced once and then reinforced in every later stage, rather than taught in isolation:

- **`const`-correctness** — `const` values (01) → pass-by-`const&` (02) → `const` methods (07) →
  `const&` in `Vec2`/physics/engine APIs (08, 15–16, 25–26). Python has no equivalent, so each
  README calls out where it applies and why.
- **RAII** — named at 07 (destructors) → files auto-close (10) → smart pointers / rule of 0/3/5
  (11) → engine resource ownership (25–26).
- **Debugging** — compiler errors (00) → linker errors (04) → `lldb` (05) → AddressSanitizer (06)
  → tests + planted-bug hunt (13).
- **Unit testing** — a dedicated stage (13), then reinforced in every logic-heavy stage after it:
  math (15), collision (16), Pong logic (18), tilemap parsing (20), particle pool (22), resource
  manager (25). The rule: keep game *logic* pure and testable, separate from rendering.
- **Coding style** — a documented convention applied from stage 00; the tools (`clang-format`,
  `clang-tidy`) are formally taught at stage 05. See [`STYLE.md`](STYLE.md).
- **`<random>`** — introduced at 01 (vs Python's `random`) for the dice battle, reused for
  particle jitter (22) and any procedural content.

---

## Stages

### Phase A — C++ core (single-file, plain `clang++`)

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 00 | [Hello World & toolchain](stages/00-hello-cpp/) | compiled vs interpreted, `clang++`, reading a compiler error (translation units teased here, covered at 04) | character-sheet printer |
| 01 | [Types, `const`, control flow & random](stages/01-types-const-control-flow/) | static typing, numeric types, `const`, `static_cast`, `std::array` + range-`for`, `auto`, loops, `if`/`switch`, `<random>` | dice battle over a `std::array` of combatants |
| 02 | [Functions & memory basics](stages/02-functions-and-memory/) | value / ref / `const&` params, overloading; stack vs heap (conceptual) | game-math funcs + in-place `reflect` + overloaded `clamp` |
| 03 | [Enums, structs & strings](stages/03-enums-and-structs/) | `enum class`, `switch`, `struct` vs `class`, brace init, `std::string` basics | `Stats`/`Item` structs + `DamageType` table |

### Phase B — Multi-file + tooling

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 04 | [Headers & multi-file compilation](stages/04-headers-and-multi-file/) | `.hpp`/`.cpp`, declaration vs definition, `#pragma once`, ODR, hand-linking, linker errors | split the math lib across files |
| 05 | [CMake basics, debugging & code style](stages/05-cmake-and-debugging-basics/) | `CMakeLists.txt`, Debug/Release, first `lldb` session, `clang-format` + `clang-tidy` | convert the stage-4 project to CMake; debug it |

### Phase C — Pointers, classes, RAII, generics

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 06 | [Pointers, references & memory debugging](stages/06-pointers-references-asan/) | `*`/`&`/`nullptr`, refs vs pointers, `new`/heap, dangling pitfalls, AddressSanitizer | pointer swap + heap linked list under ASan |
| 07 | [Classes & OOP](stages/07-classes-and-oop/) | ctors/dtors, member-init list, encapsulation, `const` methods, inheritance/`virtual`, RAII | `Character`/`Enemy` battle + a real-cleanup dtor |
| 08 | [Operator overloading, templates & namespaces](stages/08-operators-templates-namespaces/) | operator overloading, function & class templates, `namespace` | `Vec2<T>` class template + generic `clamp<T>` |
| 09 | [STL, lambdas & `std::function`](stages/09-stl-and-lambdas/) | `vector`/`std::array`, `unordered_map` vs `map`, iterators, `<algorithm>`, lambdas, stored `std::function` | inventory: keyed, sorted, filtered |
| 10 | [File I/O & error handling](stages/10-file-io-and-error-handling/) | `ifstream`/`ofstream`/`fstream`, parsing, RAII close; exceptions vs codes vs `std::optional` | save/load inventory + stats |
| 11 | [Smart pointers, move & rule of 0/3/5](stages/11-smart-pointers-move-rule-of-five/) | `unique_ptr`, `shared_ptr`, `std::move`, rule of 0/3/5 | `unique_ptr` entities sharing a `shared_ptr` resource |

### Phase D — Build & test infrastructure

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 12 | [Multi-target CMake + FetchContent](stages/12-cmake-multitarget/) | multi-target lib+exe, install/layout, `FetchContent` (fetch `fmt`) | reorganize into lib+exe, use `fmt::format` |
| 13 | [Unit testing with Catch2](stages/13-unit-testing-catch2/) | Catch2, `ctest`, assertions, `SECTION`s, fixtures, `GENERATE`, TDD, pure vs side-effect, planted bug | full test suite for math + inventory + save/load |

### Phase E — SFML foundations, math & physics

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 14 | [SFML: window, loop & delta time](stages/14-sfml-window-loop-delta-time/) | window/event-poll, update/render split, coordinate system, variable + fixed timestep | ball under a fixed-timestep accumulator |
| 15 | [2D vector math & motion](stages/15-2d-vector-math-and-motion/) | integration, gravity, `normalize`, `lerp`/easing/tweening, `Vec2`↔`sf::Vector2f`; unit-tested | aimed projectile + eased platform |
| 16 | [Collision detection & response](stages/16-collision-detection-and-response/) | AABB–AABB, circle–circle, circle–AABB, resolution & restitution; unit-tested | balls + rectangles bouncing in a box |
| 17 | [Sprites, text & animation](stages/17-sprites-text-animation/) | `sf::Texture`/`sf::Sprite`, texture lifetime, sprite sheets + animation, `sf::Text`/`sf::Font` | animated walk-cycle with a label |

### Phase F — First game + debug UI

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 18 | [First real game: Pong](stages/18-first-game-pong/) | input, reuse tested collision + motion, `sf::Text` score, audio (`SoundBuffer` lifetime); unit-tested | full Pong with score + sound |
| 19 | [Dear ImGui debug overlay](stages/19-imgui-debug-overlay/) | `imgui-sfml`, debug panel (FPS, live tunables, entity count), profiling | ImGui overlay on Pong |

### Phase G — 2D world features

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 20 | [Tilemaps](stages/20-tilemaps/) | tile-grid data, vertex-array rendering, tile collision, load map from file; parse-tested | render + walk around a tile level |
| 21 | [Camera & viewport](stages/21-camera-and-viewport/) | `sf::View`, follow target, clamp to bounds, world↔screen coords, parallax | scrolling camera + parallax + HUD |
| 22 | [Particle systems](stages/22-particle-systems/) | emitter, pooled particle buffer, lifetime/velocity/fade (reuses motion + `<random>`); pool-tested | explosion + thruster-trail effect |

### Phase H — Architecture & engine

| # | Stage | Core concepts | Mini-project |
|---|-------|---------------|--------------|
| 23 | [From objects to a component model](stages/23-component-model/) | OOP refactor → component model; ECS vs OOP (EnTT as the grown-up version) | Pong entities as components |
| 24 | [Scenes, state machine, input & events](stages/24-scenes-state-input-events/) | `GameState`, state machine, scene stack, input action mapping, `std::function` event system | Menu/Playing/GameOver scenes |
| 25 | [Resource manager & data-driven design](stages/25-resource-manager-data-driven/) | templated `ResourceManager<T>` + caching; load level/entity config from files | assets/config through the manager |
| 26 | [Extract the engine](stages/26-extract-the-engine/) | pull subsystems into `libengine` (static lib), main-loop ownership, `namespace eng` | port Pong onto `libengine` |
| 27 | [Capstone game](stages/27-capstone-game/) | apply the whole engine to a bigger genre | top-down on `libengine` + inventory + save/load |

> **Out of scope, by design** (so their absence reads as a decision, not an oversight):
> threading/concurrency, networking/multiplayer, spatial-partition broadphase (quadtree/grid —
> noted as further reading on 16/22), and shaders.

---

## Progress checklist

- [ ] **00** — Hello World & toolchain
- [ ] **01** — Types, `const`, control flow & random
- [ ] **02** — Functions & memory basics
- [ ] **03** — Enums, structs & strings
- [ ] **04** — Headers & multi-file compilation
- [ ] **05** — CMake basics, debugging & code style
- [ ] **06** — Pointers, references & memory debugging
- [ ] **07** — Classes & OOP
- [ ] **08** — Operator overloading, templates & namespaces
- [ ] **09** — STL, lambdas & `std::function`
- [ ] **10** — File I/O & error handling
- [ ] **11** — Smart pointers, move & rule of 0/3/5
- [ ] **12** — Multi-target CMake + FetchContent
- [ ] **13** — Unit testing with Catch2
- [ ] **14** — SFML: window, loop & delta time
- [ ] **15** — 2D vector math & motion
- [ ] **16** — Collision detection & response
- [ ] **17** — Sprites, text & animation
- [ ] **18** — First real game: Pong
- [ ] **19** — Dear ImGui debug overlay
- [ ] **20** — Tilemaps
- [ ] **21** — Camera & viewport
- [ ] **22** — Particle systems
- [ ] **23** — From objects to a component model
- [ ] **24** — Scenes, state machine, input & events
- [ ] **25** — Resource manager & data-driven design
- [ ] **26** — Extract the engine
- [ ] **27** — Capstone game
