# Stage 15 — 2D vector math & motion

**Goal:** give the things in your game a *direction* and make them move under
*forces*. You'll extend the `gmath` library with the vector functions every 2D
game leans on (`dot`, `length`, `normalize`, `distance`, `lerp`), learn how
**integration** turns acceleration into motion (and why the order of two lines
matters for **gravity**), and build **tweening/easing** for smooth, hand-crafted
movement. Along the way you'll wire up the **`Vec2` ↔ `sf::Vector2f` bridge** —
and understand why we keep our own vector type instead of just using SFML's.

**New concepts:** the vector free functions `dot` / `length` / `normalize` /
`distance` / `lerp` · position / velocity / **acceleration** · **Euler
integration** (explicit vs **semi-implicit**) · **gravity** · `normalize` to aim
· **lerp / easing / tweening** · the SFML bridge `to_sfml` / `from_sfml`.

> Stage 14 gave us a loop and a `dt`. Here we fill the `update(dt)` step with
> real motion. Everything is kept **pure and unit-tested** (stage 13's rule):
> the maths lives in headless headers, and SFML only appears at the drawing edge.

---

## 1. Extending `gmath`: vectors as direction + magnitude

Stage 08 built `gmath::Vec2<T>` with `+ - *` and `==`. A vector is two things at
once: a **direction** and a **magnitude** (length). The functions we add this
stage let us pull those apart and recombine them:

```cpp
namespace gmath {
    T       dot(const Vec2<T>& a, const Vec2<T>& b);   // alignment (a scalar)
    T       length(const Vec2<T>& v);                  // magnitude, sqrt(x²+y²)
    T       length_squared(const Vec2<T>& v);          // magnitude², no sqrt
    Vec2<T> normalize(const Vec2<T>& v);               // same direction, length 1
    T       distance(const Vec2<T>& a, const Vec2<T>& b);
    T       lerp(T a, T b, T t);                        // scalar blend
    Vec2<T> lerp(const Vec2<T>& a, const Vec2<T>& b, T t);  // vector blend
}
```

These are **free functions**, not methods — they read like the maths
(`length(v)`, `dot(a, b)`) and don't need Vec2's internals. In Python they'd be
module-level functions; here they live beside the class in the `gmath` namespace.

Two you'll reach for constantly:

- **`normalize`** turns "toward the target" into a pure direction of length 1, so
  you can scale it by any speed. **Guard the zero vector**: it has no direction,
  and dividing by its zero length gives `NaN`. We return `{0,0}` instead.
- **`length_squared`** avoids a `sqrt`. When you only *compare* distances ("which
  enemy is closest?"), compare the squares — `a < b` iff `a² < b²` for lengths.

> **`const`-correctness (a course thread):** every one of these takes its inputs
> by `const&` and returns a new value — they never mutate their arguments. That's
> the same discipline as the stage-08 operators, now applied to free functions.

---

## 2. Integration: from acceleration to motion

Movement in a game is *numerical integration*: each frame we step position and
velocity forward by `dt`. With an acceleration `a` (gravity, thrust), the update
is two lines — **but the order changes the result:**

```cpp
// EXPLICIT (forward) Euler — the "obvious" order, moves with the OLD velocity:
pos += vel * dt;
vel += accel * dt;

// SEMI-IMPLICIT (symplectic) Euler — update velocity FIRST, move with the NEW one:
vel += accel * dt;
pos += vel * dt;
```

They differ by one swapped line, but explicit Euler slowly **gains energy** and
lags under constant acceleration, while semi-implicit is stable and conserves
energy well. For gravity, springs, and orbits, **use semi-implicit** — it's the
default in virtually every 2D engine. The unit tests in
[`test_motion.cpp`](project/tests/test_motion.cpp) show the one-step difference
directly.

**Gravity** is just a constant downward acceleration. Remember stage 14: +y
points **down** in SFML, so gravity is a **positive** y value, e.g.
`Vec2f{0, 900}` px/s².

---

## 3. Aiming: `normalize(target - origin) * speed`

To fire a projectile from `origin` toward `target` at a fixed `speed`:

```cpp
Vec2f dir = normalize(target - origin);  // direction only, length 1
Vec2f vel = dir * speed;                 // magnitude = exactly `speed`, aimed
```

That's the whole idea behind `motion::aim_velocity`, and it's why `normalize`
exists: it cleanly separates *which way* from *how fast*. The mini-project fires
projectiles at wherever you click using exactly this.

---

## 4. Lerp, easing & tweening: hand-crafted motion

Not everything obeys physics. A moving platform, a menu sliding in, a camera
catching up — these are **tweens**: interpolations from A to B over time.

- **`lerp(a, b, t)` = `a + (b - a) * t`.** At `t=0` you're at `a`; at `t=1` at
  `b`; linearly in between. The single most useful function in game feel.
- A raw lerp moves at **constant speed** and stops abruptly. An **easing
  function** reshapes `t` (still 0→1) along a curve so motion eases in and/or
  out. `smoothstep(t) = 3t² − 2t³` is the classic S-curve; `ease_in_quad` starts
  slow, `ease_out_quad` ends slow. You then feed the eased value into `lerp`:

  ```cpp
  pos = lerp(a, b, smoothstep(t));   // glides out and settles in
  ```

- To go back and forth forever, drive `t` with **`ping_pong`**, which folds an
  ever-growing time into a 0→1→0 triangle wave. The mini-project's platform is
  `lerp(a, b, smoothstep(ping_pong(elapsed * rate)))`.

---

## 5. The `Vec2` ↔ `sf::Vector2f` bridge (and why keep both)

SFML has its own vector, `sf::Vector2f`. So why do we keep `gmath::Vec2f` and
convert at the edges instead of just using SFML's everywhere?

1. **Purity / testability.** `gmath::Vec2f` and all the motion code stay free of
   SFML, so the unit tests compile and run **headless** — no window, no GPU. The
   instant our vector type drags in `<SFML/...>`, every test needs SFML linked.
2. **Capability.** `sf::Vector2f` has no `dot`/`length`/`normalize`/`lerp`. Our
   type owns the maths the gameplay needs.
3. **Independence.** Gameplay/physics shouldn't depend on the rendering library.
   If we swapped SFML out later, only the thin bridge would change.

So the bridge is deliberately tiny and lives in its **own** header,
[`vec2_sfml.hpp`](project/include/vec2_sfml.hpp) — the only file that includes
both worlds:

```cpp
sf::Vector2f gmath::to_sfml(const Vec2f& v);   // just before window.draw(...)
gmath::Vec2f gmath::from_sfml(const sf::Vector2f& v);  // e.g. reading the mouse
```

We convert **only at the boundary**: do all the maths in `gmath`, call `to_sfml`
the moment we hand a position to a shape. This "keep your domain types, adapt at
the edges" pattern recurs throughout the engine stages.

---

## 6. Building and running

```bash
cd project
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build

./build/motion_demo                           # the window app
ctest --test-dir build --output-on-failure    # the pure maths tests (no window)
```

The tests link **no SFML** — that's the payoff of the bridge split. On first run
CMake fetches SFML + Catch2 from source (needs a network connection).

> **Controls in `motion_demo`:** left-click to re-aim and re-launch the
> projectile at the cursor; `Esc`/window-X quits. Watch the platform ease
> (accelerate out, decelerate in) rather than slide at constant speed.

---

## Exercises

In [`exercises/`](exercises/) — six **Catch2** drills, no window, so they all run
here. Each is self-contained; the `*.solution` versions are registered with
`ctest`. Ordered easy → hard:

```bash
cd exercises
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build
ctest --test-dir build --output-on-failure    # the solutions pass
```

1. **`01_dot_length`** *(easy)* — implement `dot` and `length`; the two atoms
   everything else is built from.
2. **`02_normalize`** *(medium)* — implement `normalize` with the **zero-vector
   guard** (no `NaN`).
3. **`03_lerp`** *(medium)* — implement scalar and vector `lerp`.
4. **`04_semi_implicit_euler`** *(medium-hard)* — implement the semi-implicit
   Euler step and see it differ from explicit.
5. **`05_aim_velocity`** *(medium)* — implement `normalize(target - origin) *
   speed`; the projectile's launch velocity.
6. **`06_easing_tween`** *(medium-hard)* — implement `smoothstep` and
   `ping_pong` for a back-and-forth eased tween.

Try each yourself before peeking at the `.solution.cpp`.

---

## Mini-project — Aimed projectile + eased platform

**Your task.** In [`project/`](project/). The SFML scaffolding — the window, the event loop, mouse
re-aiming and all the drawing through the `to_sfml()` bridge in
[`src/main.cpp`](project/src/main.cpp) — is complete. What's missing is the **pure maths** the demo
leans on: the vector free functions in [`include/vec2.hpp`](project/include/vec2.hpp) (`dot`,
`length`, `normalize`, `distance`, vector `lerp`) and the motion helpers in
[`include/motion.hpp`](project/include/motion.hpp) (the two Euler integrators, `aim_velocity`, the
easing curves, `ping_pong`) ship as `// TODO(stage 15)` stubs. The app builds and links, but the
projectile won't fly and the tests start **RED**. Implement each function (the header comments give
the formula; do `vec2.hpp` first, since `aim_velocity` uses `normalize`) and drive the suite green.
Two movers, one per half of the stage:

- a **projectile** launched with `aim_velocity(origin, target, speed)` and
  falling under **gravity** via **semi-implicit Euler** — click to re-aim it;
- a **platform** that glides between two points with
  `lerp(a, b, smoothstep(ping_pong(t)))` — an **eased, ping-ponged tween**.

Positions are computed entirely in `gmath::Vec2f` and handed to SFML only through
`to_sfml()`. The maths — integration steps, `normalize`, `lerp`, easing — is
covered by the Catch2 suite you can run headless here. The window itself is
*unverified-by-execution* in this course's build box (no display) but compiles
and links against SFML.

```bash
cd project
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build
ctest --test-dir build --output-on-failure    # RED until you implement vec2.hpp + motion.hpp
```

Stuck? A complete reference is in [`project/solution/`](project/solution/) — try it yourself first,
then compare. It's a self-contained project you build on its own:

```bash
cmake -S project/solution -B project/solution/build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build project/solution/build
ctest --test-dir project/solution/build --output-on-failure   # GREEN
```

> **Builds on stage 14:** the loop still clamps `dt` against hitches. **Feeds
> stage 16:** the projectile currently flies off-screen and relaunches — giving
> it something to *collide* with is the next stage.

---

## Checklist before moving on

- [ ] I implemented the mini-project's `vec2.hpp` (`dot` / `length` / `normalize` / `distance` /
      `lerp` as **free functions**, `normalize` guarding the zero vector) and `motion.hpp`, driving
      `test_vec2.cpp` + `test_motion.cpp` from red to green.
- [ ] I can integrate motion with **semi-implicit Euler** and explain why it beats
      explicit Euler for gravity.
- [ ] I know gravity is a **positive-y** acceleration because +y is down.
- [ ] I can aim with `normalize(target - origin) * speed` and say why `normalize`
      is the key step.
- [ ] I can build an **eased tween** with `lerp` + `smoothstep` (+ `ping_pong` to
      loop it).
- [ ] I can explain why `gmath::Vec2f` stays **separate** from `sf::Vector2f`, and
      I only convert through the `to_sfml` / `from_sfml` **bridge**.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — Fiedler's *Integration
Basics*, the *Easing Functions Cheat Sheet* (easings.net), and the "dot/cross
product" chapters of any game-math text.

Next up: [Stage 16 — Collision detection & response](../16-collision-detection-and-response/),
where moving things finally start to *hit* each other: AABB, circle, penetration,
and bounce (restitution).
