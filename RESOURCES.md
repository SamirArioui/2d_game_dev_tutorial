# Resources

Curated external links, grouped by topic. Each stage README points back to the relevant section
here. Prefer the primary references (cppreference, official docs) over random blog posts — they
stay correct as the language evolves.

> These are third-party resources on the public internet; their content and availability can
> change. Nothing here is required reading to finish a stage — the stage README is self-contained
> — but these go deeper when you want it.

---

## C++ language & standard library

- **cppreference.com** — https://en.cppreference.com/ — the reference. Bookmark it. Every type
  and function you meet (`std::vector`, `std::optional`, `std::ifstream`, …) has a page here.
- **learncpp.com** — https://www.learncpp.com/ — free, thorough, beginner-oriented tutorial that
  parallels this course's early phases well.
- **The C++ Core Guidelines** — https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines —
  Stroustrup & Sutter's canonical "how to write good modern C++". Backs our `STYLE.md`.
- **Back to Basics (CppCon talks)** — https://www.youtube.com/@CppCon — search "Back to Basics
  pointers / RAII / templates / move semantics" for excellent talk-length explanations.
- **Compiler Explorer (godbolt.org)** — https://godbolt.org/ — paste code, see the assembly and
  errors from many compilers side by side. Great for "what does this actually compile to?".

### Coming from Python — quick mental-model mappings

| Python | C++ | Introduced in |
|--------|-----|---------------|
| `black` (formatter) | `clang-format` | stage 05 |
| `flake8` / `pylint` (linter) | `clang-tidy` | stage 05 |
| `pdb` (debugger) | `lldb` / `gdb` | stage 05 |
| `str` | `std::string` | stage 03 |
| `random` module | `<random>` (`std::mt19937`) | stage 01 |
| `for x in seq:` | range-`for` (`for (auto& x : seq)`) | stages 01, 09 |
| duck typing / generics | templates | stage 08 |
| `None` | `std::optional` / `nullptr` | stages 06, 10 |
| `list` | `std::vector` / `std::array` | stage 09 |
| `dict` | `std::unordered_map` / `std::map` | stage 09 |
| `with open(...)` context manager | RAII (`std::ifstream`) | stages 07, 10 |

---

## Build system & tooling

- **CMake documentation** — https://cmake.org/cmake/help/latest/ — reference for commands.
- **"An Introduction to Modern CMake"** — https://cliutils.gitlab.io/modern-cmake/ — the readable
  guide to doing CMake the right (target-based) way.
- **`FetchContent`** — https://cmake.org/cmake/help/latest/module/FetchContent.html — how we pull
  SFML/Catch2/ImGui without a system package manager.
- **clang-format** — https://clang.llvm.org/docs/ClangFormat.html
- **clang-tidy** — https://clang.llvm.org/extra/clang-tidy/
- **LLVM debugging with lldb** — https://lldb.llvm.org/use/tutorial.html
- **GDB tutorial** — https://sourceware.org/gdb/current/onlinedocs/gdb/

---

## Testing

- **Catch2** — https://github.com/catchorg/Catch2 — the unit-testing framework used from stage 12.
  See its `docs/` for the tutorial and assertion macros.
- **CTest** — https://cmake.org/cmake/help/latest/manual/ctest.1.html — CMake's test runner.

---

## SFML (2D graphics/audio/input)

- **SFML official site & tutorials** — https://www.sfml-dev.org/tutorials/ — start with "Graphics"
  (window, sprites, text) and "Audio".
- **SFML API documentation** — https://www.sfml-dev.org/documentation/
- **SFML on GitHub** — https://github.com/SFML/SFML

> Version note: SFML 3.0 changed some APIs from 2.x (e.g. event handling). This course pins a
> specific version via `FetchContent`; follow the version the stage README/CMake specifies so the
> sample code matches.

---

## Dear ImGui (debug UI)

- **Dear ImGui** — https://github.com/ocornut/imgui — the immediate-mode GUI library; read the
  FAQ and `imgui.h` comments.
- **ImGui-SFML** — https://github.com/SFML/imgui-sfml — the backend that binds ImGui to SFML
  (used in stage 16).

---

## Game architecture & engine design

- **Game Programming Patterns** by Robert Nystrom — https://gameprogrammingpatterns.com/ — free
  online; the single most useful book for this course's back half (game loop, state, component,
  observer/event patterns).
- **"Fix Your Timestep!"** by Glenn Fiedler — https://gafferongames.com/post/fix_your_timestep/ —
  the classic article on fixed vs variable timestep (stage 13).
- **EnTT** — https://github.com/skypjack/entt — a production-grade ECS library. We *don't* use it
  (we build a simpler component model by hand), but it's the reference for "what ECS grows into".
- **"How to make a simple entity-component-system in C++"** and similar write-ups — search after
  stage 17 once you understand the OOP baseline we compare against.

---

## Communities

- **r/gamedev** — https://www.reddit.com/r/gamedev/
- **r/cpp** — https://www.reddit.com/r/cpp/
- **The Cherno (YouTube)** — https://www.youtube.com/@TheCherno — C++ and game-engine series.
- **SFML forum** — https://en.sfml-dev.org/forums/
