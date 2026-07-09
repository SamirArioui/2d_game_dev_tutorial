# Stage 16 — Collision detection & response

**Goal:** make things actually *hit* each other. You'll implement the three
colliders that carry almost every 2D game — **AABB–AABB**, **circle–circle**, and
**circle–AABB** — learn to compute not just *whether* two shapes overlap but *how
much* and *in which direction* (the **penetration** vector), and then **respond**:
push them apart and **bounce** them with a tunable **restitution**. As always the
maths is pure and unit-tested; the window is just the show.

**New concepts:** axis-aligned bounding boxes · overlap tests · the
**penetration vector / MTV** (minimum translation vector) · the **contact
normal** · closest-point circle-vs-box · **positional correction** · **restitution**
(bounce) via `reflect_velocity` · `std::optional` as "hit or miss".

> This is the payoff of stages 14–15: stage 14 gave the loop and `dt`, stage 15
> gave motion and the `gmath` vector functions (`dot`, `length`, `normalize`) —
> and collision is built entirely from those. The AABB-vs-AABB case here is
> exactly what **Pong** (stage 18) needs for ball-vs-paddle.

---

## 1. Two questions: detection and response

Collision splits cleanly into two jobs, and we keep them as separate functions:

1. **Detection** — *are these two shapes overlapping?* And if so, by how much
   (**depth**) and along what direction (**normal**)? We answer all three at once
   with a **penetration vector**.
2. **Response** — *given that they overlap, what do we do?* Separate them
   (move them apart) and change their velocities (bounce, stop, slide).

Every detector returns `std::optional<Hit>` — the C++ way to say "maybe a
result". `std::nullopt` means "no collision" (Python's `return None`); a value
carries the `Hit`. The caller writes:

```cpp
if (auto hit = aabb_vs_aabb(a, b)) {   // like `if (h := aabb_vs_aabb(a,b)) is not None`
    resolve(a, b, *hit);
}
```

---

## 2. The penetration vector (MTV)

The single most useful piece of collision data is the **penetration vector**,
also called the **minimum translation vector (MTV)**: the *shortest* shift that
would separate the two shapes. It bundles everything response needs:

- its **direction** is the **contact normal** (which way to push),
- its **length** is the **overlap depth** (how far),
- and by convention here, **adding it to shape A's position moves A just clear of
  B**.

Our shared contract (used across the course) is deliberately small:

```cpp
struct AABB { gmath::Vec2f pos, size; };   // pos = TOP-LEFT corner (+y is down)
struct Hit  { gmath::Vec2f penetration; }; // add to A's pos to un-overlap it
std::optional<Hit> aabb_vs_aabb(const AABB& a, const AABB& b);
```

---

## 3. AABB vs AABB

An **axis-aligned bounding box** is a rectangle that never rotates. Two of them
overlap iff they overlap on **both** axes. On each axis the overlap depth is
`min(far edges) − max(near edges)`; if either is ≤ 0 there's a gap and they miss.

When they do overlap, the cheapest way out is along the axis of **smaller**
overlap — you back out the short way — signed by comparing the boxes' centres:

```cpp
overlap_x = min(a.right, b.right) − max(a.left, b.left);
overlap_y = min(a.bottom, b.bottom) − max(a.top, b.top);
// pick the smaller; if A is left of B, push A left (−x); if above, push up (−y); ...
```

That's the whole `aabb_vs_aabb`. It's cheap (a handful of comparisons), which is
why AABBs are the default collider and the basis of broad-phase culling.

---

## 4. Circle vs circle

The cheapest test of all: two circles overlap iff the distance between centres is
less than the sum of radii. Compare **squared** distances first so a miss costs
no `sqrt` (this is exactly why `length_squared` exists in `gmath`):

```cpp
delta = a.center − b.center;             // from B toward A
if (length_squared(delta) >= (rA+rB)²) return nullopt;   // miss, no sqrt
depth = (rA + rB) − length(delta);
penetration = normalize(delta) * depth;  // push A away from B
```

---

## 5. Circle vs AABB (the closest-point trick)

Mixing shapes sounds hard but has an elegant shortcut: the **closest point on a
box to any point** is that point with its x clamped to `[left, right]` and its y
clamped to `[top, bottom]` — one `clamp` per axis. If the circle's centre is
within one radius of that closest point, they collide; push the circle out along
the line from the closest point to the centre.

```cpp
closest = { clamp(c.x, left, right), clamp(c.y, top, bottom) };
diff = c.center − closest;
if (length_squared(diff) > r²) return nullopt;
penetration = normalize(diff) * (r − length(diff));
```

One corner case genuinely is special: when the circle's **centre is inside the
box**, `closest == centre` and there's no direction to push. The full
[`collision.hpp`](project/include/collision.hpp) handles it by ejecting through
the **nearest face** (the exercise version covers the common centre-outside case
and points this out).

---

## 6. Response: separate, then bounce

Once detection hands you a `Hit`, response is two steps:

1. **Positional correction** — move the shape(s) out of overlap using the
   penetration. Move one shape by the whole vector, or split it between both.
2. **Velocity response** — bounce off the contact normal `n = normalize(penetration)`:

   ```cpp
   v' = v − (1 + e) · dot(v, n) · n
   ```

   `e` is the **coefficient of restitution** (bounciness): `e = 1` is a perfect
   elastic bounce (reverses the normal component, keeps the tangential — a ball
   off a wall), `e = 0` kills the normal component (no bounce — slide/stop).
   Reflect **only** when the object is moving *into* the surface (`dot(v, n) < 0`),
   or it can stick and jitter.

> **Two-body caveat.** The mini-project uses an equal-"mass" approximation: split
> the penetration and reflect each body off the contact normal. A physically
> exact response between two moving bodies weighs the bounce by their masses and
> applies a single shared **impulse**. That's beyond this stage; the wall/paddle
> case (one side immovable), which is what Pong needs, is exactly the simple
> `reflect_velocity` above.

---

## 7. A note on rotated shapes: SAT (further reading)

Everything here assumes axis-aligned boxes and circles. For *rotated* convex
polygons the generalisation is the **Separating Axis Theorem (SAT)**: two convex
shapes are disjoint iff there exists some axis on which their projections don't
overlap; test each shape's face normals as candidate axes. The AABB test in §3 is
SAT's simplest instance (the only axes are x and y). We don't need rotation in
this course — see [`../../RESOURCES.md`](../../RESOURCES.md) if you're curious.
Spatial **broad-phase** structures (quadtrees / grids) to avoid testing every
pair are similarly out of scope (noted in the ROADMAP).

---

## 8. Building and running

```bash
cd project
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build

./build/collision_demo                        # the window app
ctest --test-dir build --output-on-failure    # every predicate + response, headless
```

The tests link **no SFML** — the collision math is pure. On first run CMake
fetches SFML + Catch2 from source (needs a network connection).

> **`collision_demo`:** a mix of balls and boxes bounce around the window,
> colliding with the walls and each other. `Esc`/window-X quits.

---

## Exercises

In [`exercises/`](exercises/) — six **Catch2** drills, no window, so they all run
here. The `*.solution` versions are registered with `ctest`. Ordered easy → hard:

```bash
cd exercises
cmake -S . -B build -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake --build build
ctest --test-dir build --output-on-failure    # the solutions pass
```

1. **`01_aabb_overlap`** *(easy)* — the boolean box-overlap test (both axes).
2. **`02_aabb_penetration`** *(medium)* — the shallow-axis **MTV**, signed by
   centre comparison.
3. **`03_circle_vs_circle`** *(medium)* — distance-vs-radii, squared-distance
   early-out, penetration.
4. **`04_circle_vs_aabb`** *(medium-hard)* — the **closest-point** method
   (centre-outside case).
5. **`05_reflect_restitution`** *(medium)* — `reflect_velocity` with `e`, and the
   moving-away guard.
6. **`06_resolve_and_bounce`** *(medium-hard)* — combine penetration + reflect
   into a full **resolve** step.

Try each yourself before peeking at the `.solution.cpp`.

---

## Mini-project — Balls & rectangles bouncing in a box

In [`project/`](project/). A handful of circles and AABBs drift around the window;
each frame they move, bounce off the four container walls, and are tested pairwise
with the right predicate (`circle_vs_circle`, `aabb_vs_aabb`, `circle_vs_aabb`).
On a hit they're separated by the penetration vector and both velocities bounce
with restitution. It exercises **all** of the stage:

- all three detectors, dispatched by shape pair,
- the penetration vector driving **positional correction**,
- `reflect_velocity` for the **bounce** (walls and pairs),
- the `dt` clamp habit from stage 14 so nothing tunnels through a wall.

Every predicate and the response math is covered by the Catch2 suite you can run
headless here. The window itself is *unverified-by-execution* in this course's
build box (no display) but compiles and links against SFML.

> **Feeds stage 18:** the AABB-vs-AABB and reflect-with-restitution pieces are
> literally Pong's ball-vs-paddle and ball-vs-wall — you'll reuse this tested code
> rather than rewrite it.

---

## Checklist before moving on

- [ ] I can implement and explain **AABB–AABB**, **circle–circle**, and
      **circle–AABB** overlap tests.
- [ ] I know what the **penetration vector / MTV** is, that its direction is the
      **contact normal** and its length the **depth**, and the convention that it
      moves shape A off B.
- [ ] I use `std::optional<Hit>` to mean "hit or miss" and know why squared
      distance avoids a needless `sqrt`.
- [ ] I can **resolve** a collision: positional correction + `reflect_velocity`
      with **restitution**, only when moving into the surface.
- [ ] I know **SAT** is the rotated-shape generalisation and that broad-phase
      structures exist — both are out of scope here.
- [ ] I kept every predicate and the response **pure and unit-tested**, separate
      from the window.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — Randy Gallegos /
"N Tutorial" on AABB & SAT, Real-Time Collision Detection (Ericson) for the
closest-point derivations, and Fiedler on collision response.

Next up: [Stage 17 — Sprites, text & animation](../17-sprites-text-animation/),
where the plain shapes become textured sprites — and then **stage 18** turns this
tested collision + motion code into a real game of **Pong**.
