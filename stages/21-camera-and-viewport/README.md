# Stage 21 — Camera & viewport

**Goal:** make the world bigger than the window and move a **camera** through it. You'll follow a
character with an `sf::View`, **clamp** the view so it never shows past the level edges, convert
between **screen pixels and world coordinates** (the maths behind mouse clicks), add a **parallax**
background for depth, and draw a **HUD** that stays fixed on screen while the world scrolls beneath
it. As always, the maths is pulled into pure, unit-tested functions; SFML just applies them.

**New concepts:** `sf::View` (a movable/zoomable window into the world) · following a target ·
clamping the view to level bounds · **world↔screen** conversion (`mapPixelToCoords` /
`mapCoordsToPixel`) · **parallax** layers · a **screen-space HUD** drawn in the default view.

**Python comparison:** none direct — there's no everyday Python analogue to a scrolling viewport, so
this stage leans on the coordinate-space maths instead.

---

## 1. Three coordinate spaces (get these straight and the rest is easy)

The one idea that unlocks cameras is that a point can be named in different spaces:

- **World space** — where the level, the player, and enemies actually live. Measured in world
  pixels; can be far bigger than the window (our demo level is 1280×640).
- **Screen space** — window pixels, `0..800` × `0..600`. Where the mouse reports clicks and where a
  HUD is positioned.
- **View** — the rectangle *of world space* that gets stretched onto the screen. Move the view and
  the world appears to scroll; shrink the view and the world appears to zoom in.

Almost every camera bug is a confusion between these. The pure functions in
[`project/include/game/camera.hpp`](project/include/game/camera.hpp) each convert one space to
another, and their names say which.

---

## 2. `sf::View` — the camera

An `sf::View` is just a **center** and a **size** in world space (plus an optional rotation and a
viewport, which we leave at the defaults). SFML draws whatever the view's rectangle covers, scaled
to fill the window. To scroll, you move the center:

```cpp
sf::View game_view(sf::FloatRect(0, 0, 800, 600));  // center (400,300), size 800x600
game_view.setCenter(player_x, player_y);            // move the camera onto the player
window.setView(game_view);                          // everything drawn now uses it
window.draw(world);
```

Anything drawn after `setView` is interpreted in that view. Switch back to
`window.getDefaultView()` (a view locked to the window) to draw screen-space UI. Our loop uses
**two** views: the scrolling `game_view` for the world, and the default view for the parallax
background and the HUD.

> **Python note:** the closest mental model is a scroll offset you'd subtract from every draw
> position by hand (`screen = world - camera`). `sf::View` does that subtraction (and the scaling)
> for you, on the GPU, for every vertex.

---

## 3. Follow, then clamp

Following is trivial: set the view center to the player's center every frame. The problem is the
**edges** — near a corner, a naive follow-camera shows the empty void beyond the level. The fix is
to **clamp** the center so the view rectangle stays inside the level:

```cpp
Vec2f cam = game::clamp_view_center(player_center, view_size, level_size);
game_view.setCenter(cam.x, cam.y);
```

`clamp_view_center` (in [`project/src/camera.cpp`](project/src/camera.cpp)) keeps the center within
`[view_size/2, level_size - view_size/2]` on each axis — so the view's edge can touch the level's
edge but never pass it. If the level is *smaller* than the view on an axis, it centers on the level
instead (you see the whole thing). This is pure arithmetic, so it's unit-tested exhaustively.

---

## 4. World ↔ screen (what a mouse click needs)

A click arrives in **screen pixels**, but the tile or enemy it selects lives in **world space**,
which the camera has scrolled (and maybe zoomed). The conversion:

```
top_left = view_center - view_size/2                  # world corner the view shows
world    = top_left + (pixel / window_size) * view_size
```

That's exactly what **`sf::RenderWindow::mapPixelToCoords`** computes, and its inverse
**`mapCoordsToPixel`** goes the other way. The demo calls the SFML function on a click; the pure
`game::screen_to_world` / `world_to_screen` mirror it so you can test the maths (the round-trip
`world → screen → world` must return the start — the same property-testing idea as save/load in
stage 13). Exercise 02 is the click-to-world drill; exercise 04 is the round-trip.

---

## 5. Parallax — faking depth

Distant scenery should scroll *slower* than the foreground; that speed difference is what your eye
reads as depth. Give each background layer a **factor** in `[0, 1]`: a layer has scrolled
`camera_center * factor`.

- `factor = 1.0` → moves with the world (it *is* the foreground).
- `factor = 0.0` → never moves (the sky, or effectively a HUD).
- `factor = 0.3` → distant hills that drift slowly.

`game::parallax_offset` returns that value. The demo draws a procedurally-generated starfield (an
`sf::Image` of random dots, `setRepeated(true)` so it tiles to any size) in **screen space** and
slides its **texture rectangle** by the parallax offset — cheap, always covers the window, and
scrolls at 30% of the camera. (You could equally give a layer its own `sf::View` centered at the
same offset; both are shown in the comments.)

---

## 6. The HUD — drawn in the default view

A health bar or minimap must ignore the camera and stay put on screen. That's just: switch to the
default view before drawing it.

```cpp
window.setView(game_view);   window.draw(world);   // scrolls
window.setView(hud_view);    window.draw(minimap); // fixed to the window
```

The demo's HUD is a corner **minimap**: a translucent panel plus a dot at
`player / level * panel_size` — a world→panel-space conversion, the same shape of maths as
world→screen. (We use shapes, not `sf::Text`, so the stage ships no binary font file.)

---

## 7. Building and running

The mini-project lives in [`project/`](project/) with three CMake targets: `camera_core` (pure
maths + the carried tilemap), the SFML `camera_demo`, and the Catch2 `camera_tests`. It ships as a
**starter**: the SFML `sf::View` / parallax / HUD boilerplate and the headers are complete, but the
pure camera maths in [`project/src/camera.cpp`](project/src/camera.cpp) is left for you to write, so
out of the box the camera-maths tests are **RED**. You make them green.

```bash
cd project
cmake -S . -B build          # first run clones SFML 2.6.1 + Catch2 v3.7.1 (needs network)
cmake --build build

ctest --test-dir build --output-on-failure   # RED until you implement src/camera.cpp
./build/camera_demo          # run from project/ so it finds assets/level2.map
```

Move with **WASD** / arrows: the camera follows and clamps at the level edges, the starfield drifts
slowly behind, the minimap dot tracks you, and **clicking** drops a world-space marker under the
cursor (the click-to-world conversion in action).

The complete, all-green reference is in [`project/solution/`](project/solution/) — build it the same
way (`cmake -S project/solution -B project/solution/build && cmake --build project/solution/build`)
to see `100% tests passed`.

> **Sandbox / offline note:** the first `cmake` clones SFML and Catch2 from `github.com`; run it
> with the sandbox disabled if your network is restricted. Later builds are offline (cached in
> `build/_deps`). **If linking fails**, add `-DCMAKE_CXX_COMPILER=/usr/bin/clang++`.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; `NN_name.solution.cpp`
is the reference. Ordered easy → hard. 01/02/05 are standalone (02/05 only need the header); 03/04
call the `camera_core` source, so compile it alongside:

```bash
cd exercises
clang++ -std=c++17 -Wall -Wextra 01_clamp_scalar.solution.cpp -o ex && ./ex
clang++ -std=c++17 -Wall -Wextra -I ../project/include 02_screen_to_world.solution.cpp -o ex && ./ex
clang++ -std=c++17 -Wall -Wextra -I ../project/include \
    ../project/solution/src/camera.cpp 03_clamp_view.solution.cpp -o ex && ./ex
```

Each program asserts its own results and prints `all checks passed`.

1. **`01_clamp_scalar`** *(easy)* — write `clamp`, the atom of all camera bounds work.
2. **`02_screen_to_world`** *(medium)* — the **click-to-world** conversion, implemented by hand.
3. **`03_clamp_view`** *(medium)* — use `clamp_view_center` and reason about the clamped corner.
4. **`04_roundtrip`** *(medium)* — prove `world → screen → world` is identity (round-trip property).
5. **`05_parallax`** *(medium)* — implement `parallax_offset` and show far layers scroll slower.

---

## Your task — scrolling camera + parallax + HUD

In [`project/`](project/). The mini-project **combines every concept of the stage**:

- an `sf::View` that **follows** the player and **clamps** to the 1280×640 level;
- a **parallax** starfield that scrolls at 30% of the camera;
- a fixed **HUD** minimap (drawn in the default view) with a live player dot;
- **click-to-world** via `mapPixelToCoords`, dropping a world-space marker;
- a self-contained **tilemap** copy (from stage 20) for the level and its collision;
- a Catch2 suite for the pure camera maths, run by `ctest`.

**What you implement.** Everything above is wired up *except* the pure camera maths — the functions
in [`project/src/camera.cpp`](project/src/camera.cpp) (`clamp`, `clamp_view_center`,
`screen_to_world`, `world_to_screen`, `parallax_offset`) ship as `// TODO(stage 21)` stubs with
placeholder returns. The starter compiles and links, but the tests in
[`project/tests/test_camera.cpp`](project/tests/test_camera.cpp) are **RED** until you fill them in.
The contract (formulas + edge cases) for each function is in
[`project/include/game/camera.hpp`](project/include/game/camera.hpp). Implement them, run `ctest`,
and drive it to green — the SFML `sf::View` / parallax / HUD layer is already done for you (that half
is *unverified-by-execution* on the build machine anyway; the bar is that it **builds and links**,
while the camera *maths* you write is proven by the tests).

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone
(`cmake -S project/solution -B project/solution/build`) to compare.

---

## Checklist before moving on

- [ ] I implemented the pure camera maths (`clamp_view_center`, `screen_to_world`/`world_to_screen`,
      `parallax_offset`) and drove the camera-maths tests from RED to green.
- [ ] I can name the difference between world, screen, and view space.
- [ ] I moved an `sf::View` to follow a target and switched views to draw a fixed HUD.
- [ ] I clamped the view center so the camera never shows past the level edges.
- [ ] I converted a click from screen pixels to world coordinates (and back), and know
      `mapPixelToCoords` / `mapCoordsToPixel` do the same maths.
- [ ] I made a background scroll slower than the foreground for a parallax depth effect.
- [ ] I ran the camera-maths tests with `ctest`.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — the SFML `sf::View` tutorial and
articles on 2D camera follow/clamp and parallax.

Next up: [Stage 22 — Particle systems](../22-particle-systems/) — emit and recycle thousands of
short-lived particles for explosions and thruster trails.
