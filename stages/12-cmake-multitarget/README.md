# Stage 12 — Multi-target CMake + FetchContent

**Goal:** grow from the one-executable `CMakeLists.txt` of [stage 05](../05-cmake-and-debugging-basics/)
into a real project shape: a reusable **library target** plus an **executable target** that links
it, and learn to pull an **external dependency** from the internet with **`FetchContent`**. By the
end you'll have reorganized the console game into a `game` library + an `app` executable, and made
the app format its output with the fetched **`fmt`** library.

**New concepts:** `add_library` vs `add_executable` · `target_link_libraries` · `PUBLIC` /
`PRIVATE` / `INTERFACE` usage requirements · `target_include_directories` · project layout &
`install` basics · `FetchContent` (fetch + build a dependency at configure time) · calling
`fmt::format` / `fmt::print`.

> This is a build-system stage, so the exercises are small **CMake projects** rather than single
> `.cpp` files. Each `exercises/NN_name/` folder has source files, a `CMakeLists.txt` with `TODO`s
> for you to fill in, and a `CMakeLists.solution.txt` to compare against.

---

## 1. Why split one program into several targets?

In Python you already structure code without thinking about a "build": you split it into
**modules** (`inventory.py`) and **packages** (a folder with `__init__.py`), and `import` pulls
them together. There is no separate step that turns your project into a shippable artifact.

In C++ the compiler works one **translation unit** at a time (stage 04), and you decide how those
units are grouped and combined. A **target** is CMake's word for "a thing we build". The two you
need now:

| Target kind | CMake command | Python-ish analogy | Produces |
|-------------|---------------|--------------------|----------|
| **Library** | `add_library(game ...)` | an importable package (`game/`) | `libgame.a` — compiled code, no `main` |
| **Executable** | `add_executable(app ...)` | the script you actually run | `app` — a runnable program with `main` |

Why bother splitting instead of dumping everything into one `add_executable`?

- **Reuse & separation.** The game *logic* (math, inventory, save/load) compiles once into a
  library. The `app` links it; **so will the test runner in [stage 13](../13-unit-testing-catch2/)**,
  and later so will your engine. Logic in a library, `main` in an executable is the shape the rest
  of this course depends on.
- **Faster rebuilds.** Change `main.cpp` and only `app` recompiles; the library is untouched.
- **Clear dependencies.** The build graph says exactly what needs what.

---

## 2. Three commands: library, executable, link

That's the whole core of the stage:

```cmake
# Compile these sources once into a library called `game` (produces libgame.a).
add_library(game
    src/game_math.cpp
    src/inventory.cpp
    src/save_data.cpp
)

# The program. It has main(); the library does not.
add_executable(app app/main.cpp)

# Wire them together: `app` needs `game` at link time.
target_link_libraries(app PRIVATE game)
```

`target_link_libraries` is the load-bearing line: it tells the linker (stage 04) to resolve the
functions `app` calls (`game::clamp`, `Inventory::add`, …) using the code inside `game`. Forget it
and you get a **linker error** — "undefined reference to …" — not a compiler error. That distinction
is exactly the one stage 04 set up: the compiler was happy (it saw the declarations in the headers);
the *linker* couldn't find the definitions because nothing linked the library.

---

## 3. `PUBLIC`, `PRIVATE`, `INTERFACE` — the one CMake idea people trip on

Every `target_*` command takes one of these keywords. They answer: **"does whatever links me also
need this?"**

- **`PRIVATE`** — I need it to build myself; my consumers don't. (An implementation detail.)
- **`INTERFACE`** — I don't need it to build myself; my consumers do. (Header-only requirement.)
- **`PUBLIC`** — both. (`PUBLIC` = `PRIVATE` + `INTERFACE`.)

The classic use is include directories. Our headers live in `include/`, away from the `.cpp`
files, so:

```cmake
# `game` needs include/ to compile its own sources, AND every target that links
# `game` (the app, the tests) must see include/ too. That's both -> PUBLIC.
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

Because it's `PUBLIC`, `app` inherits the include path automatically just by linking `game` — you
never repeat the path. If you'd marked it `PRIVATE`, `game` would still compile, but `app`'s
`#include "game/inventory.hpp"` would fail with "file not found". These inherited settings are
called **usage requirements**, and getting them right is what "modern, target-based CMake" means.

> Python has no equivalent because there's no compile/link/include model to propagate — `import`
> just finds modules on `sys.path`. Here, *who needs what* is something you declare.

---

## 4. Project layout & `install`

A conventional C++ layout, and the one this project uses:

```
project/
├── CMakeLists.txt        # the build description
├── include/game/         # PUBLIC headers (the library's API)
│   ├── game_math.hpp
│   ├── inventory.hpp
│   └── save_data.hpp
├── src/                  # library implementation (.cpp)
│   ├── game_math.cpp
│   ├── inventory.cpp
│   └── save_data.cpp
└── app/                  # the executable
    └── main.cpp
```

Headers under `include/game/` means you write `#include "game/inventory.hpp"` — the `game/`
prefix makes it obvious which library a header belongs to (just like `from game.inventory import
...` in Python).

**Install** is the step that copies build outputs to a real location so *other* projects — or a
user — can use them:

```cmake
install(TARGETS app game)                      # app -> bin/, libgame.a -> lib/
install(DIRECTORY include/ DESTINATION include) # headers -> include/
```

```bash
cmake --install build --prefix ./out   # populates out/bin, out/lib, out/include
```

You won't *deploy* anything in this course, but `install` rules are how libraries become
consumable, and it's a one-liner worth knowing.

---

## 5. `FetchContent` — `pip install`, but for your build

Sooner or later you need code you didn't write: `fmt` now, **Catch2** next stage, **SFML** at
stage 14. In Python you'd `pip install` it into an environment. C++ has no single blessed package
manager, so the approach this course uses is CMake's built-in **`FetchContent`**: at *configure
time* it downloads a dependency's source and builds it as part of your project.

```cmake
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 11.0.2       # pin an exact version -> reproducible builds
    GIT_SHALLOW TRUE     # clone only that commit, not full history
)
FetchContent_MakeAvailable(fmt)   # download (first time) + add fmt's targets
```

After `FetchContent_MakeAvailable(fmt)`, fmt's targets exist in your build exactly as if you'd
written them, and you link the one it exports:

```cmake
target_link_libraries(app PRIVATE game fmt::fmt)
```

| Python (`pip`) | C++ (`FetchContent`) |
|----------------|----------------------|
| `pip install fmt` into an environment | `FetchContent_Declare` + `MakeAvailable` |
| `requirements.txt` pins `fmt==11.0.2` | `GIT_TAG 11.0.2` pins the version |
| installed once, cached in the venv | cloned once into `build/_deps`, cached there |
| `import fmt` | `#include <fmt/format.h>` + link `fmt::fmt` |

Notes:
- The **first** `cmake -S . -B build` needs network access to clone fmt (into `build/_deps/`).
  After that it's cached, so rebuilds and re-configures are offline and fast.
- **Always pin `GIT_TAG`** to a released version or commit — never a moving branch like `master` —
  or your build can change under you.

---

## 6. Calling `fmt` — it's basically Python's `str.format`

`fmt` brings Python-style formatting to C++. If you know f-strings, you know `fmt`:

```cpp
#include <fmt/format.h>

fmt::print("Total: {} gold\n", 222);                 // like print(f"Total: {222} gold")
std::string s = fmt::format("{:.2f}", 5.0f);         // "5.00"  — like f"{5.0:.2f}"
fmt::print("{:<10}{:>3}\n", "HP", 30);               // left/right align in fixed widths
fmt::print("{:=^40}\n", " CHARACTER SHEET ");        // center, pad with '=' to width 40
```

The mini-language after `:` is the same idea as Python's format spec: `fill`, `align`
(`<` `>` `^`), `width`, and `.precision`. Why use `fmt` instead of `std::cout <<`? Because
`std::cout` alignment (via `<iomanip>`) is verbose and stateful, whereas `fmt` is concise,
type-safe, and the direct ancestor of C++20's `std::format`. It's the perfect small dependency to
learn `FetchContent` on.

---

## Building and running

The mini-project lives in [`project/`](project/). Note the `CMakeLists.txt` there is a **starter
you complete** (see "Your task" below), so these commands only produce an `app` once you've filled
in its `TODO`s — the completed reference in `project/solution/` builds with the exact same commands
today:

```bash
cd project
cmake -S . -B build          # first run clones fmt into build/_deps (needs network)
cmake --build build          # builds libgame.a, fmt, then the app
./build/app                  # run it (Windows: .\build\Debug\app.exe)
```

You should see a `fmt`-formatted character sheet, a value-sorted inventory, and a "saved and
reloaded" line proving the whole library works together.

> **Sandbox / offline note:** the first configure needs to reach `github.com`. If your environment
> blocks it, run the configure once with network enabled; the cached clone makes every later build
> offline.

---

## Exercises

In [`exercises/`](exercises/). Each is a self-contained mini-project folder. Fill in the `TODO`s in
its `CMakeLists.txt`, then build:

```bash
cd exercises/01_single_target
cmake -S . -B build && cmake --build build && ./build/hello
```

Compare your `CMakeLists.txt` against the folder's `CMakeLists.solution.txt`. Ordered easy → hard:

1. **`01_single_target`** *(easy)* — write your first `CMakeLists.txt` from scratch: one
   `add_executable`. Reestablishes the stage-05 baseline.
2. **`02_library_and_exe`** *(easy–medium)* — split logic into `add_library` and link it into an
   executable with `target_link_libraries`. The heart of the stage.
3. **`03_public_includes`** *(medium)* — move headers into `include/` and expose them with a
   `PUBLIC` `target_include_directories` so the app inherits the path. Break it to `PRIVATE` and
   watch the app fail to find the header — that's the lesson.
4. **`04_fetch_fmt`** *(medium)* — pull `fmt` with `FetchContent` and call `fmt::print`. This is
   the exercise that needs network on first configure.
5. **`05_install_and_alias`** *(medium–hard)* — add `install()` rules and a namespaced **alias**
   target (`math::math`) so linking your own lib looks just like linking `fmt::fmt`.

---

## Mini-project — the console game as `game` lib + `app` exe

**Your task.** In [`project/`](project/). The C++ sources are already complete — the *lesson* of
this stage is the build system, so your job is the **`CMakeLists.txt`**. It ships as a guided
starter full of `TODO(stage 12)` markers: as-is it configures but declares no targets, so nothing
builds yet. Fill in each `TODO` to declare the **`game` library target**, the **`app` executable
target**, link them (plus fetched `fmt`), and pull `fmt` via **`FetchContent`**. This
**combines everything from this stage**:

- All the logic from earlier stages — the math functions, `Inventory` (sort + `std::function`
  filter), and `Stats`/`SaveData` save/load — compiled into a single **`game` library target**
  (`src/*.cpp`, headers under `include/game/`).
- An **`app` executable target** (`app/main.cpp`) that links `game` **and** the fetched
  **`fmt`**, builds a character, and prints a formatted sheet with `fmt::print`/`fmt::format`:

```
=========== CHARACTER SHEET ============
HP         30
Attack     12
Defense     8
--------- INVENTORY (by value) ---------
  Iron Sword       x1   (120 gold each)
  Health Potion    x3   (15 gold each)
  Gold Coin        x57  (1 gold each)
----------------------------------------
Total inventory value: 222 gold
Distance (0,0)->(3,4) = 5.00
Half HP (lerp 0->30 @0.5) = 15.0
Saved and reloaded 3 distinct item(s) from 'savegame.txt'.
```

Build & run your version:

```bash
cd project
cmake -S . -B build          # first run clones fmt into build/_deps (needs network)
cmake --build build          # builds libgame.a, fmt, then the app
./build/app                  # run it (Windows: .\build\Debug\app.exe)
```

Stuck? A complete, working `CMakeLists.txt` is in
[`project/solution/CMakeLists.txt`](project/solution/CMakeLists.txt) — try it yourself first, then
compare (it is self-contained: `cmake -S project/solution -B project/solution/build`).

This `game` library is not throwaway: **stage 13 links this exact library from a Catch2 test
runner** to unit-test the math, inventory, and save/load — which is the whole reason we put the
logic in a library and kept `main` out of it.

---

## Checklist before moving on

- [ ] I can explain the difference between a **library** and an **executable** target, and why
      logic goes in the library.
- [ ] I used `target_link_libraries` to link a target, and I know a missing link is a *linker*
      error, not a compiler one.
- [ ] I can say what `PUBLIC` vs `PRIVATE` means for `target_include_directories` and predict which
      one lets the app find the library's headers.
- [ ] I fetched `fmt` with `FetchContent` (pinned with `GIT_TAG`) and actually **called**
      `fmt::format`/`fmt::print`.
- [ ] I can map `pip install` / `requirements.txt` onto `FetchContent_Declare` / `GIT_TAG`.
- [ ] I completed the mini-project starter `CMakeLists.txt` (`project/`) so it builds as a `game`
      library + `app` executable and prints fmt-formatted output.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — the "Modern CMake" guide, the
`FetchContent` docs, and the fmt project.

Next up: [Stage 13 — Unit testing with Catch2](../13-unit-testing-catch2/) — where we link this
same `game` library from a test runner and prove the logic correct.
