# Stage 26 — Extract the engine

**Goal:** stop copy-pasting the same window/loop/collision/scene code into every game and package
it once, as a reusable **static library** — `libengine`, in `namespace eng`. Then prove it works
by porting **Pong** onto it: Pong becomes a *Scene*, and a generic `eng::Application` owns the loop.

**New concepts:** what a library target is (and static vs dynamic linking) · **main-loop
ownership** and **inversion of control** (the engine calls your code, not the other way round) ·
drawing a clean seam between an engine and a game · keeping the engine's pure core testable
without a window.

This is a consolidation stage: the *subsystems* were built in stages 14–25. Here we assemble a
self-contained copy of them into one library and give it a spine — the `Application` loop.

---

## 1. Why a library, and why *static*

By stage 25 you had written — more than once — a window, an event poll, a delta-time clock, an
AABB test, a scene stack, a tilemap parser. Copy-pasting that into each new game is how bugs
multiply: you fix the collision resolver in one game and forget the other three.

A **library** is the fix: compile that shared code **once**, into an artifact other programs link
against. Two kinds:

| | Static (`.a` / `.lib`) | Dynamic / shared (`.so` / `.dll` / `.dylib`) |
|---|---|---|
| When it's combined | **Link time** — copied *into* the executable | **Run time** — loaded when the program starts |
| Result | One self-contained binary | Smaller binary + a separate library file to ship |
| "Where's the file?" | Never a problem | Must be found at runtime (rpath / PATH / `LD_LIBRARY_PATH`) |
| Best for | A course engine you just want to *link and run* | Big libs shared by many programs; plugins |

We build `libengine` **static** because it is the simplest thing that works: link it, get one
binary, nothing to deploy alongside the game. (SFML itself is pulled in via `FetchContent` and may
build as a shared library — that's SFML's choice; *our* engine is the static one.)

> **Python comparison.** There's no perfect analogy — Python has no separate link step. The
> closest is turning a folder of scripts into an installable *package* (`pip install mygame_engine`)
> so every project `import`s the same code instead of vendoring a copy. A static library is that
> idea, resolved by the linker at build time instead of by `import` at run time.

---

## 2. Main-loop ownership: who runs the `while`?

This is the heart of the stage. In stages 14–18 **the game owned the loop**:

```
your main() {
    sf::RenderWindow window(...);
    while (window.isOpen()) {          <-- the game's loop
        poll events;
        update world;
        clear / draw / display;
    }
}
```

Every game re-wrote that boilerplate. When you *extract an engine*, you flip it: **the engine owns
the loop**, and calls back into whatever scene is active. That flip is called **inversion of
control** (IoC).

```
   THE GAME OWNS THE LOOP                 THE ENGINE OWNS THE LOOP  (this stage)
   ─────────────────────                  ────────────────────────
   main() {                               eng::Application::run() {
     while (open) {                          while (open) {
       poll();      // you write it            poll  ─▶ scene.handle_input(event)
       update();    // every                   dt    ─▶ scene.update(dt)
       draw();      // single                           scene.render(window)
     }              // game                     }
   }                                        }

                                          your code is just a Scene:
                                            void update(float dt) override { ... }
                                            void render(RenderTarget&) override { ... }
                                            void handle_input(const Event&) override { ... }
```

You have felt IoC before: a **Flask view** or a **pytest test function** is *called by* the
framework — you never write Flask's request loop or pytest's runner. `eng::Application` is that
framework for your game. You hand it a `Scene`; it calls your three methods at the right times.

The payoff is concrete: `main()` for Pong shrinks to *configure → push a scene → run()*, and the
**exact same `Application`** will run the stage-27 capstone unchanged.

---

## 3. The architecture

```
 stages/26-extract-the-engine/
 ├── engine/                     libengine — the reusable static library (namespace eng)
 │   ├── CMakeLists.txt          add_library(engine STATIC ...)
 │   ├── include/eng/
 │   │   ├── math.hpp            gmath::Vec2<T>, Vec2f, length/dot/clamp/lerp   ── PURE (no SFML)
 │   │   ├── collision.hpp       gmath::AABB, Hit, aabb_vs_aabb                 ── PURE
 │   │   ├── tilemap.hpp         Tilemap: parse + tile_at + solid_at            ── PURE
 │   │   ├── resource_manager.hpp  ResourceManager<T> load-once cache           ── PURE (template)
 │   │   ├── scene.hpp           Scene (abstract) + SceneStack                  ── PURE (fwd-decls sf::)
 │   │   ├── sfml_interop.hpp    to_sfml / from_sfml (the gmath↔SFML bridge)
 │   │   ├── application.hpp     Application — OWNS the main loop
 │   │   ├── input.hpp           InputMap — action names → keys
 │   │   ├── camera.hpp          Camera — follow + clamp (wraps sf::View)
 │   │   ├── particles.hpp       ParticleSystem — pooled emitter
 │   │   ├── textures.hpp        procedural sf::Texture generators
 │   │   ├── tile_renderer.hpp   Tilemap → one batched vertex array
 │   │   ├── audio.hpp           make_tone — synthesized beeps (no .wav files)
 │   │   ├── font.hpp            load_system_font — graceful fallback
 │   │   └── engine.hpp          umbrella: #include "eng/engine.hpp" gets it all
 │   └── src/                    the .cpp files that actually touch SFML
 ├── project/                    the mini-project: Pong on libengine
 │   ├── CMakeLists.txt          fetches SFML + Catch2, adds ../engine, builds pong + tests
 │   ├── src/pong_logic.{hpp,cpp}   PURE rules (unit-tested)             ── YOU implement
 │   ├── src/pong_scene.{hpp,cpp}   the Scene (draws + input + audio)   ── YOU implement
 │   ├── src/main.cpp               configure → push → run()  (given, complete)
 │   ├── tests/                     Catch2 tests for the pure logic (RED until you implement)
 │   └── solution/                  complete REFERENCE (its own standalone build)
 └── exercises/                  SFML-free engine-API drills
```

### The pure / impure seam (why it's drawn where it is)

Notice which headers are marked **PURE**: `math`, `collision`, `tilemap`, `resource_manager`,
`scene`. They compile with **no SFML** — `scene.hpp` even *forward-declares* `sf::RenderTarget`
and `sf::Event` instead of including them, because they only ever appear as reference parameters.

That seam is deliberate and it's the same lesson from stage 13 (*keep logic pure and testable,
separate from rendering*), now applied to the engine itself. The consequence: the entire test
suite for the engine's core, and for Pong's rules, runs **headless in milliseconds** — no window,
no GPU, no display. Only the SFML-facing code (`Application`, `Camera`, renderers, audio) needs a
graphics stack, and that code is thin.

---

## 4. The `Application` loop, annotated

The whole IoC idea is ~25 lines (`engine/src/application.cpp`):

```cpp
void Application::run() {
    sf::Clock clock;
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();          // the engine handles the universal case
            }
            scenes_.handle_input(event);  // ... then hands the event to your scene
        }
        const float dt = clock.restart().asSeconds();
        scenes_.update(dt);               // your scene advances by dt
        if (scenes_.empty()) { window_.close(); break; }
        window_.clear(clear_color_);
        scenes_.render(window_);          // your scene draws
        window_.display();
    }
}
```

`scenes_` is a `SceneStack`; each of those three calls forwards to the **top** scene. Pong never
sees this loop — it just implements `update` / `render` / `handle_input`.

---

## Building and running

The mini-project is the buildable unit (like stages 12–13, each subfolder builds on its own).

```bash
cd stages/26-extract-the-engine/project
cmake -S . -B build            # FIRST run clones SFML 2.6.1 + Catch2 v3.7.1 (needs network)
cmake --build build            # builds the given engine + your (stubbed) game
ctest --test-dir build --output-on-failure   # PURE-logic tests — RED until you implement pong_logic.cpp
./build/pong                   # opens the game window (needs a display)
```

**Controls:** left paddle **W/S**, right paddle **↑/↓**, **R** resets the match, close the window
to quit.

Notes and gotchas (mostly environment-specific):

- **Network + sandbox.** `FetchContent` git-clones from GitHub on the first configure. In a
  sandboxed shell you must allow network for that step.
- **Compiler.** On macOS, SFML's windowing backend is Objective-C++; build with Apple Clang
  (`-DCMAKE_CXX_COMPILER=/usr/bin/clang++`) if your default `c++` can't compile `.mm` files.
- **SDK not found / "Could NOT find OpenGL".** That means CMake can't locate the macOS SDK
  (usually a broken `xcode-select`). Point it explicitly, e.g.
  `-DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk`.
- **No display.** The build+link and the `ctest` logic tests are the bar here; actually *seeing*
  the window needs a desktop session. The tests are window-free on purpose.
- **No font on the machine.** `eng::load_system_font` tries common system fonts and reports
  failure gracefully — the game then runs without the on-screen score rather than crashing.

---

## Exercises

In [`exercises/`](exercises/). All six are **SFML-free** drills against the engine's pure API, so
they need only Catch2 — fast, headless. Each `NN_name.cpp` is a starter whose `REQUIRE`s are wrong
on purpose (marked `// <-- fix`); the matching `.solution.cpp` is correct and is what `ctest` runs.

```bash
cd stages/26-extract-the-engine/exercises
cmake -S . -B build            # clones Catch2 on first run
cmake --build build
ctest --test-dir build --output-on-failure   # the SOLUTIONS -> all green
# work a drill, then run YOUR version:
cmake --build build --target 01_vec2_math && ./build/01_vec2_math
```

1. **`01_vec2_math`** *(easy)* — `gmath::Vec2f` arithmetic, `length`, `normalized`.
2. **`02_aabb_overlap`** *(easy–med)* — predict the minimum-translation vector from `aabb_vs_aabb`.
3. **`03_resource_cache`** *(med)* — prove `ResourceManager<T>` loads once and caches (with a fake
   asset type, so no SFML).
4. **`04_scene_stack`** *(med)* — push/pop/`current`, and that only the **top** scene updates.
5. **`05_tilemap_parse`** *(med–hard)* — parse a map and answer `tile_at` / `solid_at`.
6. **`06_particle_pool`** *(hard)* — the pool fills on emit, drains as particles expire, and caps
   at capacity.

---

## Your task — port Pong onto `libengine`

In [`project/`](project/). The engine in [`engine/`](engine/) is **provided complete** — you write
**no** engine code this stage. Your job is to build the GAME on top of it: the same Pong from stage
18, restructured to *prove the engine is reusable*. The starter already compiles, links and runs
(every game body is a placeholder), but the pong tests are **RED** until you implement:

- **`src/pong_logic.cpp`** — the rules as pure data + free functions over `gmath` (ball motion,
  wall/paddle bounces via `aabb_vs_aabb`, scoring). Zero SFML → this is exactly what `tests/` drives
  headless. The `Config`/`State`/`Event` types and the declarations are given in `pong_logic.hpp`;
  you implement the bodies.
- **`src/pong_scene.cpp`** — a `eng::Scene` that draws the state (procedural textures, no image
  files), maps input through `eng::InputMap`, and plays a synthesized beep (`eng::make_tone`) on
  each bounce/score. The class definition is given in `pong_scene.hpp`; you implement the methods.
- **`src/main.cpp`** — configure an `eng::AppConfig`, push a `PongScene`, call `run()`. **Given and
  complete**; read it to see how small the entry point becomes.

**Which engine subsystem does what, here:** `Application` = the loop; `Scene`/`SceneStack` =
Pong-as-a-mode; `InputMap` = rebindable actions; `textures` = paddle/ball sprites; `audio` = the
beep; `collision`+`math` = the physics inside the pure rules. (Tilemap, camera and particles ride
along in the library unused — the stage-27 capstone exercises those.)

Work the pure rules first — run `ctest` until the pong tests go green — then bring the scene to
life. Stuck? A complete reference is in [`project/solution/`](project/solution/) (a self-contained,
standalone build). Try it yourself first, then compare:

```bash
cmake -S project/solution -B project/solution/build
cmake --build project/solution/build
ctest --test-dir project/solution/build --output-on-failure   # all green
./project/solution/build/pong                                  # opens a window (needs a display)
```

The lesson lands when you notice `main.cpp` no longer contains a loop, a window, or a clock, and
that this same `Application` is what stage 27 reuses.

---

## Checklist before moving on

- [ ] I can explain static vs dynamic linking and why the course engine is static.
- [ ] I can state what **inversion of control** means and point to it in `Application::run()`.
- [ ] I can name the pure/impure seam and why `scene.hpp` forward-declares SFML types.
- [ ] I implemented the pure Pong rules (`pong_logic.cpp`) and the Scene (`pong_scene.cpp`); `ctest` goes green and I understand why the tests need no window.
- [ ] I can describe how Pong went from *owning main()* (stage 18) to *being a Scene* (here).

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — game-loop patterns (Gaffer on Games'
"Fix Your Timestep"), and CMake's library/target model.

Next up: [Stage 27 — Capstone game](../27-capstone-game/).
