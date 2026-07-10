# Stage 18 — First real game: Pong

**Goal:** build a *complete, playable game* — and do it the way the rest of the course will scale:
the **rules live in a pure, unit-tested library**, and the SFML `main()` is a thin shell that reads
input, calls those functions, draws rectangles, and beeps. You'll reuse the **collision** (stage
16) and **motion** (stage 15) you already tested, add **keyboard input**, an `sf::Text`
**scoreboard**, and **audio** — meeting the buffer-lifetime pitfall that mirrors stage 17's texture
rule.

**New concepts:** real-time keyboard input (`sf::Keyboard::isKeyPressed`) vs events · composing a
game loop from tested step-functions · a text scoreboard · generating sound from raw samples
(`sf::SoundBuffer` / `sf::Sound`) · the **buffer-must-outlive-the-sound** rule · guarding against a
large `dt` (tunnelling).

**Python comparison:** if you've written Pong in pygame, the shape is familiar — the difference is
that here the *rules* are a typed, compiled library with tests, not logic tangled into the render
loop. "Keep the fun in testable code" is the habit this stage cements.

---

## 1. The architecture: pure logic + thin shell

Look at what's where:

```
project/
  include/gmath/vec2.hpp        # the Vec2f you know (SFML-free)
  include/collision/aabb.hpp    # AABB overlap (carried from stage 16)
  include/pong/pong.hpp         # ALL the game rules, as pure functions
  src/pong.cpp  src/aabb.cpp    # their implementations
  src/main.cpp                  # the ONLY file that includes SFML
  tests/test_pong.cpp           # rules verified headless, under ctest
  tests/test_collision.cpp
```

`pong.hpp` has no `#include <SFML/...>` anywhere. That's deliberate: it means the entire game can
be *simulated and tested without a window*. `main.cpp` is the only place SFML appears. When you run
`ctest`, you are exercising the real game logic — serving, bouncing, spin, scoring — with zero
graphics. This is the single most important habit in the whole course, and Pong is where it becomes
concrete.

> **Python note:** this is the same instinct behind keeping your game rules in plain functions that
> `pytest` can call, instead of poking a live pygame surface. C++ just makes the separation
> physical: the logic library literally does not link SFML.

---

## 2. Reusing motion and collision

Nothing new to invent — we lean on earlier stages:

- **Motion (stage 15):** `integrate(ball, dt)` is one line — `pos += vel * dt`. dt is seconds.
- **Collision (stage 16):** `ball_hits_paddle` is `aabb_vs_aabb(ball_box, paddle_box)` — the exact
  overlap test you unit-tested two stages ago, now deciding whether the ball bounces.

The Pong-specific rules build on top:

```cpp
void integrate(Ball& ball, float dt);                 // motion
bool bounce_off_walls(Ball& ball, const Court& court); // reflect vy at top/bottom; true = beep
bool ball_hits_paddle(const Ball&, const Paddle&);     // AABB overlap
void bounce_off_paddle(Ball& ball, const Paddle&);     // reverse vx + spin, keep speed, nudge clear
Scorer check_scoring(const Ball&, const Court&);       // who crossed an edge
Ball serve(const Court&, bool to_left, float speed);   // reset to centre, launch a direction
```

Two subtleties worth calling out, both tested:

- **Spin.** `bounce_off_paddle` doesn't just flip `vx`; it steers the ball based on *where* it hit
  the paddle (top → up, bottom → down) and then **renormalizes to keep the original speed**, so
  rallies stay lively without the ball accelerating out of control.
- **Nudge-clear.** After a bounce we shove the ball just outside the paddle. Without it, the ball
  can still overlap next frame, flip again, and get "stuck" vibrating against the paddle — a
  classic bug. The test `the ball is nudged clear so it won't re-collide` locks this behavior in.

---

## 3. Input: events vs real-time state

SFML gives you two input channels, and mixing them up is a common beginner confusion:

```cpp
// EVENTS — discrete, queued. Great for "happened once": close, key just pressed.
sf::Event event;
while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) { window.close(); }
}

// REAL-TIME STATE — "is this key down RIGHT NOW?". Great for smooth movement.
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { state.left.pos.y -= speed * dt; }
```

Pong uses events only to close the window, and real-time state for the paddles (W/S on the left,
Up/Down on the right). Movement is scaled by `dt`, so paddles travel the same speed regardless of
frame rate — the delta-time lesson from stage 14.

> **Tunnelling guard.** If the window stalls (you dragged it, the OS hiccuped), the next `dt` can be
> huge, and `pos += vel * dt` could teleport the ball straight *through* a paddle without ever
> overlapping. `main.cpp` clamps `dt` to a sane maximum to prevent that. Fixed-timestep loops
> (stage 14) are the fuller cure.

---

## 4. Audio, and the buffer-lifetime pitfall

We generate a beep from scratch — no `.wav` shipped — by filling an array of 16-bit samples with a
fading sine wave and handing it to an `sf::SoundBuffer`:

```cpp
std::vector<sf::Int16> samples(count);
for (unsigned i = 0; i < count; ++i) {
    float t = float(i) / sample_rate;
    samples[i] = sf::Int16(std::sin(2*pi*freq*t) * 0.3f * fade_out * 32767);
}
sf::SoundBuffer buffer;
buffer.loadFromSamples(samples.data(), count, /*channels=*/1, sample_rate);
```

Then the pitfall — and it's **the same shape as stage 17's texture rule**:

> **`sf::Sound` stores a pointer to its `sf::SoundBuffer`; it does not copy the samples. The buffer
> must outlive the sound.**

In `main.cpp` `beep_buffer` is declared *before* `beep_sound`, so it is destroyed *after* it — the
sound's pointer is valid for the whole program. Store a sound whose buffer is a local that already
went out of scope, and you get silence, static, or a crash on shutdown. The mental model to carry
forward: **texture→sprite, font→text, buffer→sound — all the same "the heavy data must outlive the
lightweight handle that points at it" rule** you first met as dangling pointers in stage 06.

---

## Building and running

**Toolchain:** CMake ≥ 3.16 and a C++17 compiler. First configure git-clones **SFML 2.6.1 (with
audio)** and Catch2.

The mini-project in [`project/`](project/) ships as a **starter**: the SFML shell (input, render,
audio, the game loop in [`src/main.cpp`](project/src/main.cpp)) is complete, but the pure game-rule
bodies in [`src/pong.cpp`](project/src/pong.cpp) and the AABB test in
[`src/aabb.cpp`](project/src/aabb.cpp) are left as `TODO`s, so out of the box `ctest` is **RED**.
You implement them to make it green.

```bash
cd project
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build

# Verify the game RULES headless (this is what CI runs):
ctest --test-dir build --output-on-failure    # RED until you implement pong.cpp + aabb.cpp

# Play it (needs a display + audio):
./build/pong          # left = W/S, right = Up/Down
```

> Iterate on logic without compiling SFML at all:
> `cmake --build build --target pong_tests && ctest --test-dir build`.

The complete, all-green reference is in [`project/solution/`](project/solution/) — build it the
same way to see every rule pass:

```bash
cmake -S project/solution -B project/solution/build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build project/solution/build
ctest --test-dir project/solution/build --output-on-failure
```

**On this course's headless build machine**, the `pong` app is verified to **compile and link**
against SFML graphics/window/system/**audio**; the window and sound are *unverified-by-execution*.
Every rule (motion, wall bounce, paddle bounce + spin, scoring, serving) is covered by `ctest` and
runs here.

---

## Exercises

In [`exercises/`](exercises/). Pure drills (`01`–`03`) compile with a bare `clang++` and print
`...passed`; SFML drills (`04`–`06`) build through the folder's `CMakeLists.txt` and need a display
(and, for `05`, audio). Ordered easy → hard.

1. **`01_aabb_overlap`** *(easy, pure)* — the ball-vs-paddle overlap test, from scratch.
2. **`02_paddle_bounce`** *(easy, pure)* — reverse X and add spin from the contact point.
3. **`03_scoring_and_serve`** *(medium, pure)* — decide who scored and which way to serve.
4. **`04_keyboard_input`** *(medium, SFML)* — move a paddle with real-time key state + `dt`.
5. **`05_beep`** *(medium, SFML audio)* — synthesize a sine beep and respect the buffer-lifetime
   rule.
6. **`06_scoreboard`** *(medium–hard, SFML)* — draw a live `sf::Text` score with graceful font
   loading.

```bash
cd exercises
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build
./build/01_aabb_overlap_solution      # pure -> "...passed"
./build/05_beep_solution              # audio -> press SPACE
```

---

## Mini-project — full Pong

**Your task.** In [`project/`](project/). The SFML shell is done for you — real-time keyboard input,
the rectangle/scoreboard rendering, the synthesized beep (buffer declared before sound), and the
game loop in [`src/main.cpp`](project/src/main.cpp) are all complete. What's left is the **rules**:
the pure functions in [`src/pong.cpp`](project/src/pong.cpp) (`integrate`, `clamp_paddle`,
`bounce_off_walls`, `ball_hits_paddle`, `bounce_off_paddle`, `check_scoring`, `serve`,
`make_initial_state`) and the [`src/aabb.cpp`](project/src/aabb.cpp) overlap test are stubbed with
`// TODO` placeholders, so the starter compiles and links but
[`tests/test_pong.cpp`](project/tests/test_pong.cpp) and
[`tests/test_collision.cpp`](project/tests/test_collision.cpp) start **RED**. Implement the bodies
until `ctest` is green — then run `./build/pong` and play.

The finished project **combines every concept of the stage**: two paddles driven by
real-time input, a ball that integrates motion, wall and paddle bounces via the tested AABB
collision, spin on paddle hits, scoring with an auto re-serve, an `sf::Text` scoreboard, and a
`sf::Sound` beep on every bounce (buffer declared before sound). The rules are the pure `pong_core`
library; `main.cpp` is a thin shell. The tests in `tests/` pin the behavior so you can refactor the
game fearlessly.

Stuck? The complete reference is in [`project/solution/`](project/solution/) — build it with
`cmake -S project/solution -B project/solution/build -DCMAKE_CXX_COMPILER=/usr/bin/clang++` and
compare, but try it yourself first.

This same Pong is the base for **stage 19**, where you'll bolt on a Dear ImGui debug overlay (a
live FPS readout and a ball-speed slider), and it returns again in stage 23 when we re-express its
entities as components.

---

## Checklist before moving on

- [ ] My game logic is in a library with **no SFML include**, and `ctest` exercises it headless.
- [ ] I can explain the difference between event input and `isKeyPressed` real-time input, and why
      movement is multiplied by `dt`.
- [ ] Paddle bounce reverses direction, adds spin from the hit point, and nudges the ball clear.
- [ ] I synthesized a sound from samples and kept the `sf::SoundBuffer` alive longer than the
      `sf::Sound` — the same lifetime rule as texture→sprite.
- [ ] I understand why a huge `dt` can tunnel the ball through a paddle, and how the clamp helps.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — SFML's *Window → Keyboard* and
*Audio → Playing sounds* tutorials.

Next up: [Stage 19 — Dear ImGui debug overlay](../19-imgui-debug-overlay/).
