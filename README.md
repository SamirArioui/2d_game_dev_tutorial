# C++ → 2D Games → Game Engine

A gradual, fundamentals-first course that takes you from **"I know Python"** to **"I built a
small 2D game engine"**, using **C++17** and **SFML**. You learn the language by building an
unbroken chain of small games and tools — every concept is introduced when a project needs it,
then reinforced in every stage that follows.

> **Who this is for:** programmers comfortable in a high-level language (Python especially — the
> stage notes are full of Python↔C++ comparisons) who want to learn modern C++ *and* the
> basics of how a 2D game engine is put together, without hand-waving over the compiler, memory,
> or the build system.

---

## What you'll build

The mini-projects tell **one continuous story**, so each stage builds on the last:

> character sheet → dice battle → game-math lib → inventory → save/load → **Pong** →
> 2D-world tech (tilemaps / camera / particles) → a small engine (`libengine`) →
> **capstone game** (which revives the inventory + save/load from the early stages)

Along the way you meet the things that separate "C++ that compiles" from "C++ people can
maintain": `const`-correctness, RAII, smart pointers and move semantics, templates, unit testing
with Catch2, `clang-format`/`clang-tidy`, and a real debugger (`lldb`/`gdb` + AddressSanitizer).

---

## The 28 stages

Stages live in [`stages/`](stages/), numbered `00`–`27` and grouped into eight phases. See
[`ROADMAP.md`](ROADMAP.md) for the full stage-by-stage table (core concepts + the mini-project for
each) and a progress checklist.

| Phase | Stages | Theme |
|-------|--------|-------|
| **A** | 00–03 | C++ core, single-file, compiled by hand with `clang++` |
| **B** | 04–05 | Multi-file compilation, CMake, debugging & code style |
| **C** | 06–11 | Pointers, classes/OOP, templates, the STL, RAII & smart pointers |
| **D** | 12–13 | Multi-target CMake + `FetchContent`, unit testing with Catch2 |
| **E** | 14–17 | SFML foundations: window/loop, 2D math, collision, sprites |
| **F** | 18–19 | First real game (**Pong**) + a Dear ImGui debug overlay |
| **G** | 20–22 | 2D world tech: tilemaps, camera/viewport, particle systems |
| **H** | 23–27 | Architecture → extract `libengine` → **capstone game** |

Each stage folder contains:

- a detailed **`README.md`** — the *why*, the memory/compilation model, and Python comparisons;
- **`exercises/`** — 4–6 drills of gradual difficulty, each with a reference `.solution.cpp`;
- a **`project/`** — one mini-project that combines all of that stage's concepts.

> **Out of scope, by design:** threading/concurrency, networking/multiplayer, spatial-partition
> broadphase, and shaders — so their absence reads as a decision, not an oversight.

---

## Getting started

**1. Install the toolchain.** You need a C++17 compiler, CMake ≥ 3.16, and (from stage 05)
`clang-format` + `clang-tidy`. Platform-by-platform instructions for macOS, Linux, and Windows are
in [`SETUP.md`](SETUP.md).

> SFML, Catch2, and Dear ImGui are **not** installed by hand — CMake downloads and builds them
> automatically (`FetchContent`) the first time a stage needs them.

**2. Stages 00–04** are plain programs you compile directly (each README shows the exact command):

```bash
clang++ -std=c++17 -Wall -Wextra stages/00-hello-cpp/project/src/main.cpp -o hello && ./hello
```

**3. Stages 05+** use CMake. The workflow is identical on every OS:

```bash
cd stages/05-cmake-and-debugging-basics
cmake -S . -B build          # configure (first run may download dependencies)
cmake --build build          # compile
ctest --test-dir build       # run unit tests (stages 13+)
```

Each stage README names the exact binary to run. You can also build every CMake-enabled stage from
the repo root — the top-level [`CMakeLists.txt`](CMakeLists.txt) picks up each stage automatically:

```bash
cmake -S . -B build && cmake --build build
```

---

## Repository layout

```
.
├── README.md          ← you are here
├── ROADMAP.md         full 28-stage table, phases, cross-cutting threads, checklist
├── SETUP.md           install a compiler + CMake (macOS / Linux / Windows), editor setup
├── STYLE.md           naming & formatting conventions used throughout
├── RESOURCES.md       curated external references, grouped by topic
├── CMakeLists.txt     root build that aggregates every CMake-enabled stage
├── .clang-format      formatting rules (the C++ `black`)
├── .clang-tidy        linter rules (the C++ `flake8`/`pylint`)
├── .vscode/           shared VS Code extension recommendations
└── stages/            00-hello-cpp … 27-capstone-game
        └── NN-name/
            ├── README.md       the lesson
            ├── exercises/      drills + .solution.cpp references
            ├── project/        the stage's mini-project
            └── CMakeLists.txt  (stages 05+)
```

---

## Documentation

| File | What's in it |
|------|--------------|
| [`ROADMAP.md`](ROADMAP.md) | The full curriculum: every stage, its concepts and project, the cross-cutting threads (`const`-correctness, RAII, testing…), and a progress checklist. |
| [`SETUP.md`](SETUP.md) | Toolchain install for macOS/Linux/Windows, VS Code extensions, and how to build/run/test a stage. |
| [`STYLE.md`](STYLE.md) | The naming and formatting conventions applied from stage 00, and the habits (`const`-correctness, RAII, `enum class`) the course insists on. |
| [`RESOURCES.md`](RESOURCES.md) | Curated external links (cppreference, CMake, SFML, ImGui, game-architecture) with a Python→C++ mental-model table. |

---

## Conventions in brief

Full details in [`STYLE.md`](STYLE.md); the tools enforce most of it automatically.

- **Types** `PascalCase`, **functions/variables** `snake_case`, **private members** `snake_case_`.
- 4-space indent, ~100-column lines, braces on the same line, always brace control-flow bodies.
- `const`-correct by default; pass big objects by `const&`; prefer RAII over manual cleanup.

---

New here? Read [`SETUP.md`](SETUP.md), skim [`STYLE.md`](STYLE.md), then start at
[`stages/00-hello-cpp/`](stages/00-hello-cpp/).
