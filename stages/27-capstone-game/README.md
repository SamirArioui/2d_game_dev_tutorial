# Stage 27 — Capstone game

**Goal:** put the whole course together. Build a small but **complete** top-down explorer on
`libengine`, reviving the throughline's **inventory** and **save/load**. This stage adds almost no
new *engine* code — that's the point. The engine from stage 26 is done; here you *use* it, and the
only new code is the **game**.

**What "complete" means here (the vertical slice):**

- a **player** that walks around a **tilemap**, with a **camera** that follows and **clamps** to
  the map edges;
- **solid-tile collision** (you can't walk through walls, and you *slide* along them);
- **items** on the map that drop into an **inventory** (with stacked counts) when you touch them;
- a **particle burst** at each pickup, and a synthesized **pickup sound**;
- **save / load** of the player's position + inventory + `Stats` (`F5` / `F9`);
- an on-screen **HUD** (`sf::Text`) showing stats, gold value, the bag, and the controls.

Why top-down and not a platformer? A platformer needs gravity, jump arcs, coyote-time, one-way
platforms — lots of *feel* tuning that isn't really about the engine. A top-down mover exercises
the same subsystems (tilemap, camera, collision) with simpler, more testable physics, so the
capstone stays *achievable and complete* rather than ambitious and half-finished.

---

## 1. The whole course, in one screen

Everything you built now collaborates. Here's who does what:

```
                         ┌──────────────────────────────────────────┐
                         │           eng::Application                │  stage 26
                         │   owns the loop: poll → update → render    │
                         └───────────────┬──────────────────────────┘
                                         │ drives (IoC)
                                         ▼
             ┌───────────────────────────────────────────────────────────┐
             │                    capstone::ExploreScene                   │  THIS stage
             │                                                             │
   input ───▶│  eng::InputMap    "up/down/left/right" → keys               │
             │        │                                                    │
             │        ▼                                                    │
             │  game::resolve_move ──uses──▶ eng::Tilemap.solid_at         │  PURE logic
             │        │                       gmath::AABB / aabb_vs_aabb   │  (world.*, tested)
             │        ▼                                                    │
             │  game::try_pickup  ──adds──▶  game::Inventory (+ counts)    │  PURE logic
             │        │                                                    │  (tested)
             │        ├──▶ eng::ParticleSystem.emit_burst  (sparks)        │
             │        └──▶ eng::make_tone sound            (ding)          │
             │                                                             │
             │  render:  eng::Camera.apply → eng::TileRenderer → sprites   │
             │           → particles → (switch to screen view) → HUD text  │
             │                                                             │
             │  F5/F9:   game::save / game::load  (Stats + inventory + pos)│  PURE logic
             └───────────────────────────────────────────────────────────┘

   character sheet → dice battle → math lib → inventory → save/load → Pong →
   tilemaps/camera/particles → engine → ★ this game (revives inventory + save/load)
```

### Which engine subsystem contributes what

| Subsystem (from stage) | Role in the capstone |
|---|---|
| `Application` + `Scene` (26/24) | owns the loop; the game is one `ExploreScene` |
| `Tilemap` (20) | the walkable world, loaded from `assets/level.txt` (data-driven, 25) |
| `TileRenderer` (20) | draws the map as one batched vertex array of coloured quads |
| `Camera` (21) | follows the player, clamps so you never see past the map edge |
| collision `gmath::AABB` / `aabb_vs_aabb` (16) | wall collision + item overlap |
| `InputMap` (24) | movement by action name, not raw keys |
| `ParticleSystem` (22) | the pickup spark burst |
| `audio::make_tone` (18) | the pickup "ding", synthesized (no audio files) |
| `textures` (17) | player/item sprites generated in code (no image files) |
| `ResourceManager` (25) | available in the engine; this small game inlines its few textures |
| `Inventory` + `SaveData` (09–13) | **the revived throughline**: the bag and the save file |

---

## 2. The one design idea worth repeating: the pure/impure seam

The gameplay *rules* are pure functions in `world.cpp`, `inventory.cpp`, `save_data.cpp` — no SFML,
no window:

```cpp
// world.hpp — the rules the game is really made of
gmath::Vec2f resolve_move(const eng::Tilemap&, const gmath::AABB& body, const gmath::Vec2f& delta);
std::vector<gmath::Vec2f> try_pickup(std::vector<WorldItem>&, const gmath::AABB& player, Inventory&);
```

`ExploreScene::update()` is a thin conductor over them:

```cpp
player_pos_ = game::resolve_move(map_, player_box(), delta);           // collision (pure)
auto picked = game::try_pickup(items_, player_box(), inventory_);      // pickup    (pure)
for (auto& where : picked) { particles_.emit_burst(where, ...); sound.play(); }  // feedback (impure)
```

Because the rules are pure, the *actual game behaviour* — "a body stops at a wall", "you slide
along a wall", "touching an item banks it", "a save round-trips" — is unit-tested headless in
milliseconds (`project/tests/`, run by `ctest`). Only the drawing/sound/window code needs a
display, and it's the thin part. This is stage 13's discipline scaled up to a whole game.

> **Python comparison.** It's the same reason you keep business logic out of your Flask view
> functions: a pure `def resolve_move(...)` is trivially testable with `pytest`, while the view
> that renders it needs a request/response. Keep the logic pure; let the framework (here, the
> engine) own the I/O.

---

## 3. Tile collision, briefly

`resolve_move` moves the body **one axis at a time** and snaps it flush against any solid tile it
would enter:

```
    moving right into a wall column          axes resolved SEPARATELY
    ───────────────────────────────         ────────────────────────
    [body]→        ██                         press ↘ into a wall:
           [body]  ██   ← snapped so the        X is blocked  ─┐
                   ██     right edge rests       Y is free     ─┴─▶ you SLIDE down the wall
                          on the wall            (instead of stopping dead)
```

Resolving X then Y independently is the whole trick behind "slide along the wall instead of
sticking to it" — exercise 03 tests exactly that. Off-the-map cells count as solid, so the border
walls keep the player in.

---

## 4. Architecture on disk

```
 stages/27-capstone-game/
 ├── engine/            a SELF-CONTAINED copy of libengine (identical to stage 26)
 ├── project/
 │   ├── CMakeLists.txt fetch SFML+Catch2, add ../engine, build capstone + tests
 │   ├── assets/level.txt   the tilemap (24×16, 32px tiles) — data-driven
 │   ├── src/
 │   │   ├── inventory.{hpp,cpp}   game::Inventory       ── PURE (tested)
 │   │   ├── save_data.{hpp,cpp}   Stats+Inventory+pos   ── PURE (tested)
 │   │   ├── world.{hpp,cpp}       resolve_move, try_pickup ── PURE (tested)
 │   │   ├── explore_scene.{hpp,cpp}  the Scene (SFML)
 │   │   └── main.cpp             configure → push scene → run()
 │   └── tests/         Catch2 tests for the pure logic
 └── exercises/         SFML-free drills on the game logic
```

The engine is **carried in whole** so this stage builds on its own — you never `#include` across
stage folders. (In a real repo you'd instead depend on one shared engine; copying keeps each stage
independently buildable, which is the course's convention.)

---

## Building and running

```bash
cd stages/27-capstone-game/project
cmake -S . -B build            # FIRST run clones SFML 2.6.1 + Catch2 v3.7.1 (needs network)
cmake --build build
ctest --test-dir build --output-on-failure   # PURE-logic tests -> all green, headless
./build/capstone               # opens the game window (needs a display)
```

**Controls:** **WASD** move · **F5** save · **F9** load · close the window to quit. The save file
`capstone_save.txt` is written in your current directory.

Same environment notes as stage 26 apply (they're mostly about the machine, not the code):

- **Network + sandbox** — the first configure git-clones SFML/Catch2 from GitHub.
- **Compiler** — on macOS build with Apple Clang (`-DCMAKE_CXX_COMPILER=/usr/bin/clang++`); SFML's
  window backend is Objective-C++.
- **"Could NOT find OpenGL" / SDK** — point CMake at the SDK, e.g.
  `-DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk`.
- **No display / no font** — build + `ctest` are the bar; the window and the `sf::Text` HUD need a
  desktop session and an available system font. `eng::load_system_font` degrades gracefully (the
  game runs without the HUD text rather than crashing). **The window's visual behaviour is
  therefore *unverified-by-execution* in a headless setup** — the game logic that the tests cover
  is what's guaranteed.

---

## Exercises

In [`exercises/`](exercises/). Five **SFML-free** drills on the capstone's game logic (Catch2 only).
Each `NN_name.cpp` starter has `REQUIRE`s that are wrong on purpose (`// <-- fix`); the
`.solution.cpp` is correct and is what `ctest` runs.

```bash
cd stages/27-capstone-game/exercises
cmake -S . -B build && cmake --build build
ctest --test-dir build --output-on-failure    # the SOLUTIONS -> all green
```

1. **`01_inventory_totals`** *(easy)* — `add`, `count_of`, `total_value`.
2. **`02_tile_collision`** *(med)* — where does a body stop when it hits a wall?
3. **`03_slide_along_wall`** *(med)* — axis separation: blocked on X, still slides on Y.
4. **`04_pickup`** *(med)* — only overlapping items are collected.
5. **`05_save_roundtrip`** *(med–hard)* — save then load preserves stats, position and inventory.

---

## Mini-project — the game itself

In [`project/`](project/). This *is* the capstone. Walk around with WASD, grab the five items,
watch the sparks and hear the ding, check the HUD update, then `F5` to save and `F9` to reload —
you snap back to the saved spot with the saved bag. Everything you learned across 28 stages is in
that loop:

- **C++ core & memory** (00–11): values vs references, `const`-correctness, RAII (textures/sound
  buffers freed automatically), smart pointers (`unique_ptr<Scene>` in the stack), templates
  (`Vec2<T>`, `ResourceManager<T>`).
- **Build & test** (12–13): CMake targets + `FetchContent`, and a Catch2 suite over the pure logic.
- **SFML, math, physics** (14–18): the window/loop, `Vec2`, AABB collision, sprites/text, audio.
- **World tech** (20–22): tilemaps, camera, particles.
- **Architecture** (23–26): components-as-data, scenes/state/input/events, the resource manager,
  and the extracted engine driving it all.

### Where to take it next

- give items real effects (a potion raises `hp`; reflect it in the HUD and the save);
- add a second `Scene` (a title screen, or a pause overlay) and push/pop it on the `SceneStack`;
- swap procedural tile colours for a real tileset texture through `ResourceManager<sf::Texture>`;
- restore *world* item state on load (currently load restores the player + bag, not which items
  are still on the ground).

---

## Checklist — you've finished the course

- [ ] I built the capstone, ran `ctest` (collision, pickup, inventory, save/load — all green).
- [ ] I can point to each engine subsystem in `ExploreScene` and say what it contributes.
- [ ] I can explain why the game *rules* are pure functions and how that makes them testable.
- [ ] I understand this game reuses the stage-26 `Application`/engine unchanged — only the game is new.
- [ ] I can trace the throughline from the stage-00 character sheet to this game's inventory + save.

That's the whole path: from `std::cout << "Hello"` to a small 2D game on your own engine. Congratulations.

Back to the [`ROADMAP.md`](../../ROADMAP.md) · further reading in [`RESOURCES.md`](../../RESOURCES.md).
