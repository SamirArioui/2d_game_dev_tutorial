# Stage 19 — Dear ImGui debug overlay

**Goal:** bolt a **developer UI** onto the Pong game from stage 18 — a floating panel showing a
live **FPS** readout, a **ball-speed slider** that changes the game *while it runs*, an **entity
counter**, and a pause/reset button. You'll integrate **Dear ImGui** through **imgui-sfml**, learn
its five-call lifecycle, and get a first taste of **frame-time profiling**. Debug overlays are how
real game teams tune values without a recompile — and once you have one, you never go back.

**New concepts:** immediate-mode GUI (and how it differs from retained-mode) · the imgui-sfml
lifecycle (`Init` / `ProcessEvent` / `Update` / `Render` / `Shutdown`) · binding a widget straight
to a variable · live tunables · a rolling-average FPS profiler · fetching a header-only-ish
dependency (ImGui) that has no CMakeLists.

**Python comparison — the headline idea:** ImGui is **immediate-mode**. In a retained-mode toolkit
like **tkinter** or **Qt**, you *construct* widget objects once (`btn = Button(...)`), keep
references, and mutate them later through callbacks; the toolkit retains the widget tree. In
immediate mode there is **no retained tree** — every frame you *call* `ImGui::SliderFloat(...)`
again, and that call both draws the slider and reports its current value. It's closer to writing
`print()`s that happen to be interactive than to building a form. Great for debug tools; you'd
still reach for a retained toolkit for a polished end-user app.

---

## 1. What immediate mode looks like

The entire debug panel is just function calls issued fresh each frame:

```cpp
ImGui::Begin("Debug");                              // open a window
ImGui::Text("FPS: %.1f", fps);                      // a label (printf-style)
if (ImGui::SliderFloat("Ball speed", &speed, 100, 900)) {
    set_ball_speed(state.ball, speed);              // true on the frames it moved
}
ImGui::Text("Entities: %d", entity_count);
ImGui::Checkbox("Pause", &paused);                  // writes straight into `paused`
if (ImGui::Button("Reset match")) { /* ... */ }     // true only when clicked
ImGui::End();
```

Notice there are **no widget objects**. `SliderFloat` takes the *address* of your `float` and reads
& writes it in place. `Button` returns a `bool` that is only true on the frame it was clicked.
There is nothing to allocate, wire up, or free — which is exactly why it's the go-to for throwaway
debug UI.

---

## 2. The imgui-sfml lifecycle

Five calls, in a fixed shape:

```cpp
ImGui::SFML::Init(window);                 // 1) once, after the window exists
while (window.isOpen()) {
    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window, event);  // 2) let ImGui see input first
        /* ...your own event handling... */
    }
    ImGui::SFML::Update(window, deltaClock.restart());  // 3) advance ImGui (needs dt)
    /* ...ImGui::Begin(...) ... build the panel ... ImGui::End()... */
    window.clear();
    /* ...draw the game... */
    ImGui::SFML::Render(window);            // 4) draw the UI ON TOP, before display
    window.display();
}
ImGui::SFML::Shutdown();                    // 5) once at the end
```

Order matters:

- **`ProcessEvent` before your own handling** so ImGui can "capture" a click on the panel and the
  game can ignore it (otherwise clicking a slider would also move a paddle).
- **`Render` after you draw the game** so the overlay sits on top.
- **`Update` takes the frame's `sf::Time`**, which is why we `clock.restart()` once and reuse that
  value for both ImGui and the game's `dt`.

---

## 3. Making the slider actually *do* something

A debug slider is only useful if it changes the game live. The "Ball speed" slider rescales the
ball's velocity while preserving its direction — pure vector math that lives in the tested
`debug::set_ball_speed`:

```cpp
void set_ball_speed(pong::Ball& ball, float speed) {
    gmath::Vec2f dir = gmath::normalize(ball.vel);   // unit direction
    if (dir == zero) dir = {1, 0};                   // still ball -> go right
    ball.vel = dir * speed;                          // same way, new speed
}
```

Because `SliderFloat` returns `true` only on frames the user drags it, we re-apply the speed then —
and also right after a re-serve, so the slider's value "sticks" across points. As always, the UI
code is in `main.cpp`, but the *behavior* is a pure function under `ctest`
([`tests/test_tunables.cpp`](project/tests/test_tunables.cpp)).

---

## 4. Frame-time profiling

"FPS" is just `1 / frame_time`. But a single slow frame makes the instantaneous number lurch, so
we average over a window of recent frames. `debug::FrameProfiler` is a small **ring buffer** with a
running sum, so each update is O(1):

```cpp
profiler.add_sample(dt);          // push dt; evict + subtract the oldest once full
profiler.fps();                   // 1 / (sum / count)
```

That's the seed of real profiling: the same "measure per-frame cost, average it, show it" loop
scales up to timing individual subsystems later. The profiler is pure and tested (steady-stream
average, empty-safe, window eviction).

> **Where the time goes.** In this tiny game the loop is dominated by `window.display()` waiting for
> vsync (we `setFramerateLimit(60)`), so you'll see ~60 FPS / ~16.7 ms. In a heavier game the
> readout is your first clue about which frames blew the budget — turn the limit off to see the
> real ceiling.

---

## 5. Building with a dependency that has no CMakeLists

Dear ImGui ships as a handful of `.cpp`/`.h` files with **no `CMakeLists.txt`**. So the CMake dance
is slightly different from fetching SFML or fmt:

1. `FetchContent_MakeAvailable(imgui)` — because there's no `CMakeLists.txt` at its root, this only
   *downloads* the sources; it does **not** add a build target.
2. We point `IMGUI_DIR` at those downloaded sources.
3. **imgui-sfml** (which *does* have a `CMakeLists.txt`) compiles ImGui for us from `IMGUI_DIR` and
   produces the `ImGui-SFML::ImGui-SFML` target. We set `IMGUI_SFML_FIND_SFML OFF` so it reuses the
   `sfml-*` targets we already fetched instead of searching the system.

Linking `ImGui-SFML::ImGui-SFML` transitively brings in ImGui **and** SFML, so the app target needs
only that one line plus our own `pong_core`. The pinned versions — **SFML 2.6.1**, **Dear ImGui
v1.90.9**, **imgui-sfml v2.6** — are chosen to be mutually compatible (imgui-sfml v2.6 targets the
SFML 2.6 API).

---

## Building and running

**Toolchain:** CMake ≥ 3.16 and a C++17 compiler. First configure git-clones SFML 2.6.1, Dear
ImGui, imgui-sfml, and Catch2.

```bash
cd project
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build

# Verify the logic behind the widgets, headless (this is what CI runs):
ctest --test-dir build --output-on-failure

# Play with the overlay (needs a display):
./build/pong_imgui        # drag "Ball speed"; toggle Pause; watch FPS
```

> Logic-only loop (skips SFML/ImGui entirely):
> `cmake --build build --target pong_tests && ctest --test-dir build`.

**On this course's headless build machine**, `pong_imgui` is verified to **compile and link**
against SFML + Dear ImGui + imgui-sfml; the overlay and window are *unverified-by-execution*. The
logic behind every widget (`set_ball_speed`, `FrameProfiler`) plus the reused Pong rules are
covered by `ctest` and run here.

---

## Exercises

In [`exercises/`](exercises/). Pure drills (`01`, `02`) compile with a bare `clang++` and print
`...passed`; ImGui drills (`03`–`05`) build through the folder's `CMakeLists.txt` and need a
display. Ordered easy → hard.

1. **`01_speed_scale`** *(easy, pure)* — rescale a velocity to a target speed (the slider's math).
2. **`02_frame_profiler`** *(medium, pure)* — a rolling-average FPS profiler with a ring buffer.
3. **`03_imgui_hello`** *(easy, ImGui)* — the five-call lifecycle; a label and a click counter.
4. **`04_imgui_slider`** *(medium, ImGui)* — a slider that resizes a circle live.
5. **`05_fps_overlay`** *(medium, ImGui)* — compute and display a smoothed FPS in the overlay.

```bash
cd exercises
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build
./build/01_speed_scale_solution     # pure -> "...passed"
./build/04_imgui_slider_solution    # ImGui -> drag the slider
```

---

## Mini-project — Pong with an ImGui overlay

In [`project/`](project/). The stage-18 Pong, unchanged in its rules (still the tested `pong_core`
library), now with a debug panel that:

- shows a **live FPS** and average frame time (`debug::FrameProfiler`),
- exposes a **ball-speed slider** that rescales the ball's velocity *live* (`debug::set_ball_speed`),
- shows an **entity count**,
- has **Pause** and **Reset match** controls.

The takeaway is architectural as much as visual: the overlay is a thin skin over pure, tested logic,
added without touching the game rules at all. That "instrument the game through a debug UI" habit
carries straight into the engine stages, where the panel grows to inspect scenes, resources, and
particle counts.

---

## Checklist before moving on

- [ ] I can explain immediate-mode vs retained-mode UI, and why immediate mode suits debug tools.
- [ ] I know the five imgui-sfml calls and why `ProcessEvent` comes before my own event handling
      and `Render` comes after drawing the game.
- [ ] My slider changes the game live because the widget writes straight into a variable I use.
- [ ] My FPS number is a rolling average, not a per-frame jitter, and it's computed by tested code.
- [ ] I understand why ImGui needs the `IMGUI_DIR` + imgui-sfml build dance (no CMakeLists of its own).

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md); the Dear ImGui repo's `imgui_demo.cpp`
is the canonical "what widgets exist" reference — enable the demo window and read along.

Next up: [Stage 20 — Tilemaps](../20-tilemaps/).
