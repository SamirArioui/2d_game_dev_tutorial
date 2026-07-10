# Stage 22 — Particle systems

**Goal:** build a particle system — the effect layer behind explosions, sparks, smoke and exhaust —
and, more importantly, build it the way games actually do: on a **pooled**, fixed-capacity buffer
that recycles dead particles instead of allocating a fresh one every frame. You'll write an
**emitter**, a per-particle **lifetime/velocity/fade**, and two effects (an **explosion burst** and
a **thruster trail**) driven by one shared pool. The pool is pure logic, so its rules are
**unit-tested** with no window.

**New concepts:** an **emitter** (a helper that spawns a shape of particles) · a **pooled**
fixed-capacity buffer (recycle dead slots) · why pooling beats per-frame allocation · per-particle
lifetime, velocity integration (reusing stage-15 motion) and alpha **fade** · jitter with `<random>`
(`std::mt19937`, from stage 01).

**Python comparison:** object pooling vs the garbage collector.

---

## 1. Why pool? (object pooling vs the GC)

A busy effect spawns hundreds of particles a second, each living well under a second. The naive
approach allocates one object per spawn and frees it on death:

```python
# Python: a new Particle object per spawn; the GC must later collect them all
for _ in range(200):
    particles.append(Particle(pos, vel))   # 200 allocations, this frame alone
```

In Python that feeds the **garbage collector**, which periodically stops the world to sweep up the
dead ones — a classic cause of frame-time hitches in games. C++ has no GC, but the equivalent cost
is real: every `new`/`delete` hits the allocator, and freshly-allocated objects scatter across
memory (cache misses when you iterate them).

**Pooling removes both costs.** Allocate a fixed array *once*, up front, and then reuse its slots
forever:

```python
pool = [Particle() for _ in range(CAPACITY)]   # allocate ONCE
slot = find_dead(pool)                          # "spawn" = revive a dead slot
slot.reset(pos, vel)
# ...later, when it dies, just mark it dead so a future spawn can reuse it
```

No per-frame allocation, no GC pressure, and the particles stay in one contiguous block that's fast
to iterate. The trade-off is a **hard cap**: when the pool is full, new spawns are simply dropped.
For particles that's fine — a few missing sparks are invisible. This same pattern reappears for
bullets, enemies, any spawn-heavy system.

---

## 2. The `ParticlePool` — pure logic, no SFML

[`project/include/game/particle_pool.hpp`](project/include/game/particle_pool.hpp) /
[`project/src/particle_pool.cpp`](project/src/particle_pool.cpp) define it. A `Particle` is plain
data — position, velocity, `life`, `max_life`, an `(r,g,b)` colour, and an `alive` flag. The pool is
a `std::vector<Particle>` sized to capacity in the constructor and **never resized again**:

```cpp
ParticlePool(std::size_t capacity) { particles_.resize(capacity); }  // the ONLY allocation

bool spawn(const Particle& proto);  // revive a dead slot; false if full (never grows)
void update(float dt);              // integrate motion, age, recycle the expired
std::size_t alive_count() const;    // O(1) — a counter kept in sync
```

- **`spawn`** scans for the first `!alive` slot, copies the prototype in, and marks it alive. If
  none is free it returns `false` — capacity is a budget, not a suggestion. (A linear scan keeps
  the idea obvious; a production pool keeps a free-list of dead indices for O(1) spawn — noted as
  further reading.)
- **`update`** does, per alive particle: `position += velocity * dt` (the stage-15 motion), then
  `life -= dt`, and if `life <= 0` it sets `alive = false` — **recycling** the slot so the next
  `spawn` can reuse it.

Because there's no SFML in here, every one of these rules is unit-testable (section 5).

---

## 3. Emitters — spawning a *shape* of particles

An **emitter** is just a function that fills a pool with a particular pattern. Both of ours take an
`std::mt19937&` (stage 01's RNG) so the caller owns the seed — which also makes them reproducible in
tests — and both return how many particles they *actually* spawned (fewer if the pool filled up).

- **`emit_burst`** — an explosion: `count` particles flung from a point in **all directions**
  (`angle` uniform in `[0, 2π)`, random `speed`), warm colours, ~1s lifetimes.
- **`emit_trail`** — a thruster: particles moving roughly **opposite** the ship's velocity
  (`normalize(-ship_velocity)`), with a little jitter so the exhaust spreads, cool colours, short
  lifetimes.

The direction maths reuses `gmath` (`normalize`, `dot`) from stage 15, and the jitter reuses
`<random>` from stage 01 — this stage is mostly *combining* tools you already have.

---

## 4. Rendering & fade — one draw call

[`project/src/particle_render.cpp`](project/src/particle_render.cpp) (the SFML layer) turns the pool
into pixels the same way stage 20 drew the tilemap: **one `sf::VertexArray`**, four vertices per
alive particle, drawn in a single call — not one sprite each. The nice touch is the **fade**: each
particle's alpha is its remaining-life fraction,

```
alpha = clamp(life / max_life, 0, 1) * 255
```

so particles smoothly dissolve instead of popping out of existence. (Exercise 04 writes exactly this
function.) The demo [`project/src/main.cpp`](project/src/main.cpp) flies a ship with WASD that
streams a **trail**, and drops an **explosion** wherever you left-click — both feeding the *same*
4000-slot pool.

---

## 5. Unit-testing the pool (the stage's testable core)

[`project/tests/test_pool.cpp`](project/tests/test_pool.cpp) is pure Catch2 over `particle_core` —
**no SFML, no window** — pinning down the pooling invariants:

- **capacity is fixed** — a fresh pool reports its capacity and zero alive; the buffer is allocated
  up front;
- **spawn caps at capacity** — the N+1th spawn returns `false` and nothing grows;
- **dead particles recycle** — after everything expires, `alive_count()` is 0, the buffer is still
  exactly `capacity` slots, and new spawns reuse them;
- **lifetimes expire** — a particle survives `update(0.5)` then dies once cumulative `dt` passes its
  life;
- **motion integrates** — `position += velocity * dt` lands on the expected point;
- **the emitters** — `emit_burst` caps at the free slots and makes live particles; `emit_trail`'s
  particles all point *opposite* the ship (their velocity dotted with the ship's is negative).

This is the whole course philosophy in one file: the *logic* (pooling, motion, direction) is proven
by fast tests; the *rendering* is left to build-and-eyeball.

---

## 6. Building and running

The mini-project lives in [`project/`](project/) with three CMake targets: the pure `particle_core`
library, the SFML `particle_demo`, and the Catch2 `particle_tests`. It ships as a **starter**: the
SFML vertex-array renderer (with its life-based alpha fade), the demo, and the headers are complete,
but the pooling logic in [`project/src/particle_pool.cpp`](project/src/particle_pool.cpp) is left for
you to write, so out of the box the pool tests are **RED**. You make them green.

```bash
cd project
cmake -S . -B build          # first run clones SFML 2.6.1 + Catch2 v3.7.1 (needs network)
cmake --build build

ctest --test-dir build --output-on-failure   # RED until you implement src/particle_pool.cpp
./build/particle_demo        # WASD to fly (leaves a trail); left-click to explode
```

The complete, all-green reference is in [`project/solution/`](project/solution/) — build it the same
way (`cmake -S project/solution -B project/solution/build && cmake --build project/solution/build`)
to see `100% tests passed`.

> **Sandbox / offline note:** the first `cmake` clones SFML and Catch2 from `github.com`; run it
> with the sandbox disabled if your network is restricted. Later builds are offline (cached in
> `build/_deps`). **If linking fails**, add `-DCMAKE_CXX_COMPILER=/usr/bin/clang++`.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; `NN_name.solution.cpp`
is the reference. Ordered easy → hard. 01 and 04 are standalone; 02/03/05/06 use the `particle_core`
source, so compile it alongside:

```bash
cd exercises
clang++ -std=c++17 -Wall -Wextra 01_lifetime.solution.cpp -o ex && ./ex
clang++ -std=c++17 -Wall -Wextra -I ../project/include \
    ../project/solution/src/particle_pool.cpp 02_spawn_cap.solution.cpp -o ex && ./ex
```

Each program asserts its own results and prints `all checks passed`.

1. **`01_lifetime`** *(easy)* — update one particle: move it, age it, report whether it's dead.
2. **`02_spawn_cap`** *(medium)* — drive a real `ParticlePool` and confirm spawn caps at capacity.
3. **`03_recycle`** *(medium)* — expire every particle, then prove the freed slots are reused with
   no growth.
4. **`04_fade`** *(medium)* — the `life/max_life → alpha` fade function.
5. **`05_burst_emitter`** *(medium–hard)* — write an outward explosion emitter with `<random>` that
   respects the pool's capacity.
6. **`06_trail_emitter`** *(medium–hard)* — write a thruster emitter whose particles stream opposite
   the ship's velocity (checked with a dot product).

> Exercises 05/06 name their function `spawn_burst`/`spawn_trail` so they don't collide with the
> library's `emit_*` — you're re-implementing the same idea from scratch.

---

## Your task — explosion burst + thruster trail

In [`project/`](project/). The mini-project **combines every concept of the stage**:

- ONE fixed-capacity `ParticlePool` feeding **both** effects (spawns dropped when full);
- an **`emit_burst`** explosion on left-click and an **`emit_trail`** exhaust behind a WASD-driven
  ship;
- per-particle **motion integration** and **alpha fade** over lifetime;
- **`<random>`** jitter for organic-looking spread;
- one-draw-call rendering via `sf::VertexArray`;
- a Catch2 suite proving the pool's invariants, run by `ctest`.

**What you implement.** Everything above is wired up *except* the pooling logic — in
[`project/src/particle_pool.cpp`](project/src/particle_pool.cpp) the `ParticlePool::spawn` (cap +
recycle) and `ParticlePool::update` (integrate + age + recycle) methods and the two emitters
(`emit_burst`, `emit_trail`) ship as `// TODO(stage 22)` stubs with placeholder returns (the
constructor that allocates the buffer is done for you). The starter compiles and links, but the
tests in [`project/tests/test_pool.cpp`](project/tests/test_pool.cpp) are **RED** until you fill them
in. The invariants each function must uphold are documented in
[`project/include/game/particle_pool.hpp`](project/include/game/particle_pool.hpp). Implement them,
run `ctest`, and drive it to green — the SFML renderer (including the `life/max_life` alpha fade) and
demo are already done for you (that half is *unverified-by-execution* on the build machine anyway;
the bar is that it **builds and links**, while the pool *logic* you write is proven by the tests).

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone
(`cmake -S project/solution -B project/solution/build`) to compare.

---

## Checklist before moving on

- [ ] I implemented `spawn`/`update` and both emitters, and drove the pool tests from RED to green.
- [ ] I can explain why pooling beats allocating a particle per spawn (allocator/GC pressure, cache).
- [ ] My pool allocates once, caps `spawn` at capacity, and recycles dead slots on `update`.
- [ ] I integrated per-particle motion in `update`, and know the renderer fades alpha by
      `life / max_life`.
- [ ] I wrote an emitter that spawns a shape of particles and respects the capacity.
- [ ] I used `<random>` for jitter and `gmath` (`normalize`/`dot`) for direction.
- [ ] I ran the pool tests with `ctest` and understand which parts are testable and why.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — object-pool patterns (free-list
spawn), and the note in the roadmap on spatial partitioning for very large particle counts.

Next up: [Stage 23 — From objects to a component model](../23-component-model/) — where the ad-hoc
game objects start becoming a real architecture.
