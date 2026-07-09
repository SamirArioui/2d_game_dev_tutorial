# Stage 14 — SFML: window, game loop & delta time

**Goal:** open your first real-time window and understand the loop that drives every game. By the
end you'll know why a game is a `while` loop that never really stops, how to keep motion the same
speed on a slow and a fast machine (**delta time**), and why serious games run their physics on a
**fixed timestep** fed by an **accumulator** — even when the screen refreshes at a different rate.

**New concepts:** `sf::RenderWindow` + the event queue (`pollEvent`) · the **update / render** split
· SFML's coordinate system (origin top-left, **+y points down**) · measuring **delta time** with
`sf::Clock` · **variable vs fixed timestep** · the **accumulator** pattern + render interpolation.

> This is the first stage that pulls **SFML** (built from source by CMake `FetchContent`, exactly
> like `fmt` in stage 12 and Catch2 in stage 13). We keep the *logic* — motion, bouncing, timestep
> math — in a window-free header so it stays unit-testable, the rule from stage 13.

---

## 1. Why games are a loop (and Python usually isn't)

A typical Python script runs top to bottom and exits. Even a Flask server is *reactive*: it sleeps
until a request arrives, responds, and sleeps again. A game is different — it must keep drawing
**even when nothing happens**, because time itself is part of the simulation:

```python
# There is no standard-library equivalent of this in Python. The mental model:
while running:
    handle_input()      # what did the player / OS do since last frame?
    update(dt)          # advance the world by dt seconds
    render()            # draw the current state
```

That loop is the **game loop**, and it runs 60+ times a second forever. Three responsibilities,
always in this order:

1. **Input / events** — drain everything the OS has queued (key presses, mouse, "window closed").
2. **Update** — advance the simulation by the time that elapsed.
3. **Render** — draw the new state, then show it.

Keeping these three phases separate is the single most important structural habit in game code. We
never move an object *while* drawing it; we finish all updates, then draw one consistent snapshot.

### You must pump the event queue

The OS owns the window; your program only borrows it. Every frame you must **poll the event
queue**, or the OS decides your app has hung and greys it out. This is *pull*, not push — the
opposite of Python's callback-style GUI frameworks:

```cpp
sf::Event event;
while (window.pollEvent(event)) {     // drain ALL pending events this frame
    if (event.type == sf::Event::Closed) {
        window.close();               // user clicked the X
    }
}
```

---

## 2. The smallest SFML program

```cpp
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello SFML");  // (1)
    window.setVerticalSyncEnabled(true);                            // (2)

    while (window.isOpen()) {                                       // (3)
        sf::Event event;
        while (window.pollEvent(event)) {                          // (4)
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(30, 30, 46));                       // (5)
        // ... draw here ...
        window.display();                                          // (6)
    }
    return 0;
}
```

1. **`sf::RenderWindow`** — creates an OS window with an 800×600 drawable area. It's a **RAII**
   object (stage 07): when it goes out of scope its destructor closes the window and frees the GPU
   resources — no manual teardown.
2. **VSync** caps drawing to the monitor's refresh rate so we don't spin at thousands of FPS.
3. **`while (window.isOpen())`** — the game loop; runs until we `close()` the window.
4. **`pollEvent`** in an inner `while` drains *every* queued event this frame.
5. **`clear`** wipes last frame to a background colour. Games redraw everything from scratch each
   frame — there is no "leave the old pixels there".
6. **`display`** swaps the back buffer to the screen (double-buffering: you draw to a hidden buffer,
   then reveal it all at once so there's no tearing/flicker).

> **SFML 2.6 API note.** This course pins **SFML 2.6.1**. That means `window.pollEvent(event)`
> (not the SFML 3 `std::optional` style), `sf::Keyboard::Escape`, `sf::VideoMode(w, h)`, and
> `window.draw(shape)`. If you read SFML 3 tutorials online the names differ — stick to 2.6 here.

---

## 3. SFML's coordinate system: y points DOWN

This trips up everyone once. In a maths class, y grows upward. On a screen — SFML, pygame, tkinter,
image files — the origin `(0, 0)` is the **top-left** corner, +x goes right, and **+y goes down**:

```
(0,0) ────────────► +x
  │  ┌──────────────────┐
  │  │ (40,40) here is   │
  ▼  │ near the TOP-left │
 +y  │                   │
     │        •(400,300) = centre of an 800x600 window
     └──────────────────┘  (799,599) bottom-right
```

Consequences you'll hit immediately: "move up" means **subtract** from y; gravity (stage 15) is a
**positive** y acceleration; the "bottom wall" is `y == height`, not `y == 0`.

Shapes default to positioning by their **top-left corner**. We almost always call
`shape.setOrigin(w/2, h/2)` so `setPosition` places the **centre** instead — that way a shape's
"position" matches the centre we store in physics code.

---

## 4. Delta time: making speed independent of frame rate

The classic beginner bug is moving something a fixed amount **per frame**:

```cpp
x += 5;   // 5 pixels per FRAME  →  twice as fast on a 120 Hz screen as on 60 Hz
```

The fix is **delta time (`dt`)**: measure how long the previous frame took, and move by
*speed × dt*, where speed is in **pixels per second**:

```cpp
sf::Clock clock;                            // starts counting
while (window.isOpen()) {
    float dt = clock.restart().asSeconds(); // seconds since last frame; also resets the clock
    x += speed * dt;                        // px/sec * sec = px this frame
}
```

Now a slow 30 FPS machine takes big `dt` steps and a fast 240 FPS machine takes tiny ones, but both
travel the **same distance per real second**. Every moving thing in a game multiplies by `dt`.
Python has no built-in loop to compare to; the closest analogue is manually differencing
`time.perf_counter()`, but here it's the backbone of the whole program.

---

## 5. Variable vs fixed timestep, and the accumulator

Feeding the raw `dt` straight into your physics is the **variable timestep**. It's simple, but has
two problems:

- **Non-deterministic** — the same inputs produce slightly different results on different hardware,
  because the step sizes differ. Bad for physics, replays, and networked games.
- **Unstable for big `dt`** — if a frame hitches (you dragged the window, hit a breakpoint), a huge
  `dt` can make a fast object *tunnel* straight through a wall in one step.

The **fixed timestep** updates the simulation in constant slices (say 1/120 s). It's stable and
reproducible. But real frames don't arrive in neat 1/120 s intervals — so we use an **accumulator**
to bridge irregular real time and fixed simulation time:

```cpp
accumulator += dt;                 // bank the real time that elapsed
while (accumulator >= FIXED_DT) {  // spend it in whole fixed slices
    update(FIXED_DT);
    accumulator -= FIXED_DT;
}
```

Two safeties matter:

- **Cap the steps per frame** (`max_steps`). If the game stalls for a full second, the loop would
  otherwise demand 120 catch-up updates at once, making the *next* frame even slower — a runaway
  called the **spiral of death**. Capping the count breaks the spiral.
- **Clamp `dt`** before banking it, so one monstrous frame can't flood the accumulator.

### Render interpolation

After the loop there's usually **leftover** time in the accumulator (less than one slice). If you
draw the raw physics state, a 120 Hz sim shown on a 60 Hz screen looks subtly juddery. The fix is to
draw **between** the last two states, blended by `alpha = accumulator / FIXED_DT`:

```cpp
draw_pos = lerp(previous_state, current_state, alpha);
```

(We hand-roll a tiny `lerp` here; stage 15 promotes it to the shared `gmath` library.)

All of this timestep math is pure arithmetic, so it lives in [`project/include/sim.hpp`](project/include/sim.hpp)
as free functions (`steps_to_run`, `clamp_frame_dt`, `integrate`, `reflect_in_bounds`) and is
unit-tested in [`project/tests/test_sim.cpp`](project/tests/test_sim.cpp) with **no window at all**.

---

## 6. Building and running

SFML is fetched and compiled from source on the **first** configure, so that step needs a network
connection and takes a few minutes. After that it's cached.

```bash
cd project
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build

./build/ball_demo                             # the window app
ctest --test-dir build --output-on-failure    # the pure-logic tests (no window)
```

> **Compiler note.** We pass `-DCMAKE_CXX_COMPILER=/usr/bin/clang++` because some environments
> default to a compiler whose runtime doesn't link cleanly on macOS. On your own Linux box plain
> `cmake -S . -B build` is usually fine.
>
> **Controls in `ball_demo`:** `Space` toggles fixed ↔ variable timestep (watch the title bar);
> `Esc` or the window's X quits.

---

## Exercises

In [`exercises/`](exercises/). Drills 01–03 are **window** programs you build and watch; 04–05 are
**Catch2** test drills with no window (the `*.solution` versions are registered with `ctest`).
Ordered easy → hard:

```bash
cd exercises
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build
./build/01_open_window            # window drills: run and watch
ctest --test-dir build --output-on-failure   # test drills: the solutions pass
```

1. **`01_open_window`** *(easy)* — create a window and run the clear/display loop; close on the X
   and on `Escape`. The SFML skeleton every later program starts from.
2. **`02_coordinate_system`** *(easy-medium)* — place shapes to *feel* the top-left origin and
   +y-down axis; use `setOrigin` so a position means a shape's centre.
3. **`03_frame_rate_independent`** *(medium)* — move a box with `speed * dt` instead of a per-frame
   constant, and wrap it around the screen.
4. **`04_fixed_accumulator`** *(medium-hard, test)* — implement `steps_to_run`: bank time, spend it
   in whole slices, cap the count. The heart of the fixed timestep.
5. **`05_reflect_edges`** *(medium, test)* — implement the per-axis edge bounce (snap in, reverse
   velocity) and prove it with tests.

Try each yourself before peeking at the `.solution.cpp`.

---

## Mini-project — Bouncing ball under a fixed-timestep accumulator

In [`project/`](project/). A constant-velocity ball reflects off the window edges, driven by the
full **fixed-timestep accumulator with render interpolation**, and a `Space` key **toggles to a
variable timestep** so you can feel the difference. It combines every concept in the stage:

- a `RenderWindow` + event loop with the **update/render** split,
- **delta time** from `sf::Clock`, clamped against stalls,
- the **accumulator** (`steps_to_run`) running physics at a fixed 1/120 s,
- **render interpolation** (`render_lerp`) so a 60 Hz screen shows smooth motion,
- SFML's **y-down** coordinate system (the ball starts moving down-right).

All motion/timestep math is in the window-free `sim.hpp` and covered by the Catch2 suite, which you
can run headless here. The window itself is *unverified-by-execution* in this course's build box
(no display), but it compiles and links against SFML.

> **Deferred on purpose:** the ball moves at constant velocity and only *reflects* off walls. Real
> acceleration and **gravity** arrive in **stage 15**; real overlap/penetration **collision** and
> restitution arrive in **stage 16**. This stage is about the *loop and time*, not the physics.

---

## Checklist before moving on

- [ ] I can open an SFML 2.6 window and run a clear → draw → `display` loop that closes on the X and
      `Escape`.
- [ ] I can explain the **update/render split** and why I must drain the event queue every frame.
- [ ] I know SFML's origin is **top-left** and **+y points down**, and what that means for "up".
- [ ] I move things by `speed * dt` and can say why per-frame constants are a bug.
- [ ] I can describe a **variable** vs a **fixed** timestep, implement the **accumulator**, and
      explain the **spiral of death** and why `max_steps` prevents it.
- [ ] I kept the motion/timestep logic in a **window-free** header and unit-tested it.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — the SFML tutorials, and Glenn
Fiedler's *"Fix Your Timestep!"* (the canonical accumulator article this stage is built on).

Next up: [Stage 15 — 2D vector math & motion](../15-2d-vector-math-and-motion/), where the ball
stops moving in a straight line: we add velocity, acceleration, gravity and the `gmath` vector
functions, and bridge `gmath::Vec2` to `sf::Vector2f`.
