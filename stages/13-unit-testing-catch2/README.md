# Stage 13 — Unit testing with Catch2

**Goal:** stop verifying your code by squinting at `std::cout` and start proving it with automated
tests. You'll pull **Catch2** with `FetchContent` (exactly the pattern you learned last stage for
`fmt`), write assertions, sections, fixtures and parameterized tests, run them with **`ctest`**,
and internalize the habit that carries the rest of the course: **keep game logic pure and tested,
keep rendering separate.** You'll also hunt a deliberately **planted bug** and watch a test catch it.

**New concepts:** Catch2 via `FetchContent` · `TEST_CASE` · `REQUIRE` vs `CHECK` · `SECTION`s ·
fixtures (`TEST_CASE_METHOD`) · `GENERATE` / parameterized tests · `Catch::Approx` for floats ·
running & filtering tests with `ctest` · pure logic vs side-effects · Test-Driven Development
(red → green) · a planted-bug hunt.

**This is a cross-cutting stage.** From here on, every logic-heavy stage adds tests — vector math
(15), collision (16), Pong logic (18), tilemap parsing (20), the particle pool (22). Learn the
tools once here; reuse them everywhere.

---

## 1. Why bother? (You already do this in Python)

If you've used **`pytest`**, you already believe in this. A test is a small function that runs your
code with known inputs and asserts the result:

```python
# test_math.py  (pytest)
from game_math import clamp

def test_clamp_below_range():
    assert clamp(-5, 0, 10) == 0
```

```bash
pytest            # discovers and runs every test_*.py::test_* it can find
```

The payoff is the same in any language: tests catch regressions the moment you introduce them,
they document what the code is *supposed* to do, and they let you refactor fearlessly. In C++ the
argument is even stronger — there's no REPL to poke at, and a game loop is awkward to inspect by
hand. A fast test binary you run after every change is the tightest feedback loop you'll get.

**Catch2 is C++'s pytest.** Here's the same test:

```cpp
#include <catch2/catch_test_macros.hpp>
#include "game/game_math.hpp"

TEST_CASE("clamp snaps a below-range value up to lo", "[math]") {
    REQUIRE(game::clamp(-5.0f, 0.0f, 10.0f) == 0.0f);
}
```

| pytest | Catch2 |
|--------|--------|
| `def test_x():` | `TEST_CASE("x", "[tag]") { ... }` |
| `assert a == b` | `REQUIRE(a == b)` (hard) / `CHECK(a == b)` (soft) |
| `@pytest.fixture` | a fixture struct + `TEST_CASE_METHOD` |
| `@pytest.mark.parametrize` | `GENERATE(...)` |
| `pytest.approx(x)` | `Catch::Approx(x)` |
| `pytest` (runner) | `ctest` (runner) |

---

## 2. Getting Catch2 — the `FetchContent` pattern you already know

You fetched `fmt` last stage; Catch2 is the identical move (it's a compiled library in v3, not a
single header):

```cmake
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.7.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)
```

Then a **test runner** is just an executable that links Catch2 — reusing the `game` **library** we
built in [stage 12](../12-cmake-multitarget/). This is *why* we put the logic in a library and kept
`main` out of it: the app links it, and so does the test runner.

```cmake
add_executable(tests tests/test_math.cpp tests/test_inventory.cpp tests/test_save_load.cpp)
target_link_libraries(tests PRIVATE game Catch2::Catch2WithMain)  # WithMain supplies main()
```

`Catch2::Catch2WithMain` provides `main()` for you, so none of your test files need one — like how
`pytest` is the entry point, not your test modules.

---

## 3. `ctest` — the runner, and `enable_testing()` / `catch_discover_tests`

Building the `tests` executable gives you a program you *can* run directly (`./build/tests`). But
CMake ships its own test runner, **`ctest`**, and wiring your tests into it means one command runs
every test target in a project and reports a summary — the equivalent of typing `pytest`.

```cmake
enable_testing()                                   # turn CTest on for this project

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)   # register EACH TEST_CASE with CTest individually
```

`catch_discover_tests` inspects the built binary and registers every `TEST_CASE` as its own CTest
entry, so `ctest` lists and filters them by name. (The simpler `add_test(NAME all COMMAND tests)`
also works — it just registers the whole binary as one test.) Then:

```bash
ctest --test-dir build                 # run all tests, print a pass/fail summary
ctest --test-dir build --output-on-failure   # also dump output for any that fail
ctest --test-dir build -R math         # run only tests whose name matches "math"
```

---

## 4. Assertions, `SECTION`s, fixtures, `GENERATE`

**`REQUIRE` vs `CHECK`.** Both assert; the difference is what happens on failure. `REQUIRE` aborts
the test immediately (use it when continuing makes no sense — e.g. a pointer must be non-null
before you dereference it). `CHECK` records the failure and keeps going (use it to see several
independent results from one run). Catch2's killer feature: on failure it **decomposes the
expression** and prints the actual values — `REQUIRE(a == b)` shows you both `a` and `b`, no
`assertEqual`-style boilerplate needed.

**`SECTION`s** share setup without leaking it. Catch2 re-runs the `TEST_CASE` body once per leaf
section, so each section starts from a *fresh* copy of everything declared above it:

```cpp
TEST_CASE("a vector's size changes are isolated") {
    std::vector<int> v(5);          // this runs again for EACH section below
    SECTION("grow")   { v.push_back(9); REQUIRE(v.size() == 6); }
    SECTION("shrink") { v.resize(2);    REQUIRE(v.size() == 2); }  // still starts at 5
}
```

**Fixtures** bundle shared setup as a struct's members; `TEST_CASE_METHOD` runs inside a *fresh*
instance each time — the direct analogue of a pytest fixture:

```cpp
struct BagFixture { Bag bag; BagFixture() { bag.add("apple", 3); } };
TEST_CASE_METHOD(BagFixture, "starts with three apples") { REQUIRE(bag.count("apple") == 3); }
```

**`GENERATE`** parameterizes a test — it re-runs the case once per value, like
`@pytest.mark.parametrize`:

```cpp
auto v = GENERATE(-100.0f, 0.0f, 999.0f);
CAPTURE(v);                                  // print v if a check fails
REQUIRE(clamp(v, 0.0f, 10.0f) <= 10.0f);
```

**`Catch::Approx`** compares floats with a tolerance, because `0.1f + 0.2f != 0.3f` exactly —
`REQUIRE(x == Catch::Approx(0.3f))`. Same idea as `pytest.approx`.

---

## 5. Pure logic vs side-effects (the reason some code is easy to test)

Notice how effortless the math tests are: `clamp`, `lerp`, `distance` are **pure** — the output
depends only on the inputs and nothing else changes. A test just names the known answer. That is
not luck; it's the design rule this course keeps repeating:

> **Keep game logic pure and value-returning. Keep side-effects (drawing, sound, file I/O, input)
> in a thin layer on top.**

Why? Because side-effects are hard to test. You *can't* easily `REQUIRE` that a sprite appeared on
screen or a sound played — you'd need to capture pixels or audio. But you *can* trivially test the
pure function that computed *where* the sprite should be. So when a later stage does physics or
collision, we test the math (`does this AABB overlap that one?`) and leave the rendering untested by
design. Save/load sits in the middle: it has a side-effect (a file), but we make it testable by
writing to a throwaway temp file and checking the **round-trip property** — *what we saved is what
we load back* — rather than the exact bytes.

---

## 6. TDD: write the failing test first (red → green)

Test-Driven Development inverts the order: you write the test *before* the code, watch it fail
("red"), then write just enough to make it pass ("green"). It sounds backwards but it's clarifying —
the test forces you to decide what the function should *do* before worrying about *how*. Exercise 05
walks you through it with a `percent(current, max)` function: the test is written and failing, and
your job is to implement the function until `ctest` goes green.

The flip side is the **planted-bug hunt** (exercise 06): a correct test paired with buggy code. The
test fails, points at the exact wrong value, you fix the code, it passes. That is precisely what a
test suite does for you every day — it turns "I think I broke something" into "line 28, expected
15, got 25".

---

## Building and running

The mini-project test suite lives in [`project/`](project/). It ships as a **starter**: the test
files are scaffolded but the assertions are left for you to write, so out of the box `ctest` is
**RED** (each case has a `FAIL("implement me")` placeholder). You make it green.

```bash
cd project
cmake -S . -B build                              # first run clones Catch2 (needs network)
cmake --build build
ctest --test-dir build --output-on-failure       # RED until you write the assertions
```

The complete, all-green reference suite is in [`project/solution/`](project/solution/) — build it
the same way (`cmake -S project/solution -B project/solution/build`) to see `100% tests passed`.

The exercises are their own small project in [`exercises/`](exercises/):

```bash
cd exercises
cmake -S . -B build && cmake --build build
ctest --test-dir build                            # runs the reference SOLUTIONS (all green)

# work on a starter, then run YOUR version directly:
cmake --build build --target 05_tdd_percent
./build/05_tdd_percent
```

> **Sandbox / offline note:** the first configure of each project needs to reach `github.com` to
> clone Catch2. After that it's cached in `build/_deps` and every later build is offline.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a complete, passing reference. They're all built by one
`exercises/CMakeLists.txt` (which fetches Catch2 once). Solutions are registered with `ctest`;
starters you build and run by hand. Ordered easy → hard:

1. **`01_first_test`** *(easy)* — write your first `TEST_CASE` with `REQUIRE`, and add a `CHECK` to
   feel the hard-vs-soft difference.
2. **`02_sections`** *(easy–medium)* — use `SECTION`s to prove setup doesn't leak between scenarios.
3. **`03_fixture`** *(medium)* — a `TEST_CASE_METHOD` fixture (the pytest-fixture analogue) over a
   small `Bag` type.
4. **`04_generate`** *(medium)* — parameterize with `GENERATE`: a property check across many
   inputs, plus exact-output cases with `Catch::Approx`.
5. **`05_tdd_percent`** *(medium–hard)* — **TDD**: the test is written and RED; implement a *pure*
   `percent()` until it's green. Includes the pure-vs-side-effect argument.
6. **`06_planted_bug`** *(medium–hard)* — a **planted bug**: `lerp` has one wrong operator and the
   test catches it (`25 == Approx(15)`). Read the failure, fix the code, watch it go green.

> Exercises 05 and 06 start **red on purpose** — running the un-fixed starter returns a non-zero
> exit code and a failing assertion. That's the lesson, not a mistake.

---

## Mini-project — a full test suite for the `game` library

**Your task.** In [`project/`](project/). The `game` library (carried along here so the stage is
self-contained) and the CMake wiring are **already complete** — the *lesson* of this stage is
writing tests, so **you write the assertions**. Each test file is a scaffold: the `TEST_CASE` /
`SECTION` / fixture structure is there with comments describing what to prove, but the bodies hold
`// TODO(stage 13): assert ...` placeholders and one `FAIL("implement me")` per case, so the suite
starts **RED**. Replace each TODO with a real `REQUIRE`/`CHECK`, delete the `FAIL` line, and drive
it to green. It **combines every concept from this stage** into a real suite over the `game`
library from stage 12:

- [`tests/test_math.cpp`](project/tests/test_math.cpp) — `clamp`/`lerp`/`distance` with `SECTION`s
  and `Catch::Approx` (the 3-4-5 triangle, endpoints, symmetry).
- [`tests/test_inventory.cpp`](project/tests/test_inventory.cpp) — an **inventory fixture** exercising
  stacking counts, `sorted_by_value` ordering, and the `std::function` `filter`.
- [`tests/test_save_load.cpp`](project/tests/test_save_load.cpp) — the save/load **round-trip**, plus
  the two "returns `std::nullopt`" cases (missing file, malformed file).

Wired with `enable_testing()` + `catch_discover_tests`, so one command runs it all. When your
assertions are in place you should see:

```
100% tests passed, 0 tests failed out of 11
```

Stuck? The full green suite is in [`project/solution/tests/`](project/solution/tests/) — try each
file yourself first, then compare.

This same discipline — logic in a tested library, thin untested rendering on top — is what the SFML
half of the course is built on.

---

## Checklist before moving on

- [ ] I fetched Catch2 with `FetchContent` and built a test runner that links the `game` library.
- [ ] I can write a `TEST_CASE` and know when to reach for `REQUIRE` vs `CHECK`.
- [ ] I used a `SECTION`, a fixture (`TEST_CASE_METHOD`), and a `GENERATE` parameterized test.
- [ ] I compared floats with `Catch::Approx` instead of `==`.
- [ ] I ran the suite with `ctest` and can filter it by name/tag.
- [ ] I can explain why pure functions are trivial to test and rendering/side-effects aren't.
- [ ] I did the TDD exercise (red → green) and caught the planted bug with a test, then fixed it.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — the Catch2 docs (tutorial + assertion
reference) and the CTest manual.

Next up: [Stage 14 — SFML: window, loop & delta time](../14-sfml-window-loop-delta-time/) — where
`FetchContent` pulls SFML and the game finally gets a window.
