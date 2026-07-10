# Stage 01 — Types, `const`, control flow & random

**Goal:** stop printing literals and start computing with *typed* data. By the end you'll know
why C++ makes you name a type for every variable, how the numeric types differ in memory, how to
convert between them on purpose with `static_cast`, how to loop and branch, and how to generate
real randomness — everything a dice-driven battle needs.

**New concepts:** static typing · the numeric types `int` / `float` / `double` / `bool` · `const`
· `static_cast` · `std::array` + range-`for` · `auto` · loops (`for` / `while`) · `if` / `switch`
· `<random>` (`std::mt19937` + a distribution).

> Prerequisite: [stage 00](../00-hello-cpp/) — you can compile and run a single `.cpp` file and
> read a compiler error.

---

## 1. Static typing — the compiler needs to know the *type*

In Python a name is just a label that can point at anything, and it can point at something else of
a different kind a line later:

```python
level = 3          # an int
level = "three"    # now a str — totally fine in Python
```

In C++ every variable has **one type, fixed the moment you declare it**, and it keeps that type for
its whole life:

```cpp
int level = 3;      // 'level' is an int, forever
level = 5;          // fine — still an int
level = "three";    // COMPILE ERROR — you can't put a string in an int
```

**Why bother?** Two payoffs you'll feel all course long:

- **The compiler catches type mistakes before the program ever runs.** Misspell a field, pass the
  wrong thing to a function, mix up a name — the compiler stops you at build time, not at 2 a.m.
  when a player hits the bug.
- **The machine knows the exact size and layout of every value ahead of time**, so it generates
  fast code with no runtime "what type is this?" checks. A game loop running 60+ times a second
  can't afford Python's per-operation type lookups.

The mental model: a Python variable is a *name tag* you can move between boxes; a C++ variable *is*
a box of a fixed size and shape, decided when you declare it.

---

## 2. The numeric types: `int`, `float`, `double`, `bool`

Python has essentially **one** integer type (arbitrary precision) and **one** float type. C++
exposes the hardware more directly, so you choose:

| Type | Holds | Typical size | Literal example |
|------|-------|--------------|-----------------|
| `int` | whole numbers | 32 bits (~±2.1 billion) | `42`, `-7` |
| `float` | decimals, single precision | 32 bits (~7 digits) | `12.5f` (note the `f`) |
| `double` | decimals, double precision | 64 bits (~15 digits) | `2.0`, `3.14159` |
| `bool` | truth values | 1 byte | `true`, `false` |

Things that trip up Python programmers:

- **`float` vs `double`.** A decimal literal like `3.14` is a **`double`** by default. Writing
  `3.14f` makes it a **`float`**. Games often prefer `float` (half the memory, plenty of precision
  for positions/velocities, and what SFML uses), so you'll see the `f` suffix a lot. Use `double`
  when you want extra precision (e.g. an average).
- **`bool` prints as `1` / `0`** by default, not `True` / `False`. That's expected.
- Integer types have **limited range** and can *overflow* (wrap around) — not something Python's
  big integers do. We won't hit that here, but it's why the size matters.

```cpp
int    level          = 3;       // whole number
float  attack_power   = 12.5f;   // single precision (the 'f')
double crit_multiplier = 2.0;    // double precision (the default for decimals)
bool   is_boss        = false;   // true / false
```

---

## 3. `const` — a promise that a value won't change

`const` marks a value as **read-only after initialization**. Try to change it and the program
won't compile:

```cpp
const int max_level = 99;
max_level = 100;   // COMPILE ERROR: cannot assign to a const variable
```

Python has no real equivalent — a leading-underscore name or an ALL_CAPS constant is only a *hint*;
nothing stops you reassigning it. In C++ `const` is **enforced by the compiler**.

**Why use it?** It documents intent ("this is a fixed rule of the game, not a moving part"), and it
lets the compiler catch a whole class of "oops, I clobbered that" bugs. `const`-correctness is a
thread that runs through the entire course — here it's just `const` values; in [stage 02](../02-functions-and-memory/)
it becomes `const&` parameters, and in stage 07 `const` member functions. Get in the habit now:
**if a value shouldn't change, mark it `const`.**

---

## 4. `static_cast` and the integer-division trap

C++ does **not** silently convert between number types when it might lose information — you ask
explicitly with `static_cast<TargetType>(value)`. This shows up constantly because of one sharp
edge: **integer division throws away the remainder.**

```cpp
int total_xp = 250;
int battles  = 4;

total_xp / battles;                         // == 62   (an int / an int is an int; the .5 is gone)
static_cast<double>(total_xp) / battles;    // == 62.5 (one side is now double, so it keeps the fraction)
```

Python separates these with two operators — `/` always gives a float, `//` floors:

| Python | C++ |
|--------|-----|
| `250 / 4`  → `62.5` | `static_cast<double>(250) / 4` → `62.5` |
| `250 // 4` → `62`   | `250 / 4` → `62` |

`static_cast` also truncates the other way — `static_cast<int>(30.4)` is `30` (chops toward zero,
it does not round). The mini-project uses exactly that to turn a `float` damage value into a whole
number. The cast is deliberately visible so a reader can see *where* a type change happens.

---

## 5. `std::array` + range-`for` + `auto`

`std::array<T, N>` is a **fixed-size** sequence of `N` values, all of type `T`. Unlike a Python
list it cannot grow or shrink, and every element is the same type. That rigidity is the point: the
size is known at compile time, the elements sit **contiguously in memory**, and access is as fast
as it gets — ideal for a fixed roster of combatants.

```cpp
#include <array>
std::array<int, 5> rolls = {3, 6, 2, 5, 4};   // exactly 5 ints, forever
```

A **range-`for`** walks every element without you managing an index — the direct analogue of
Python's `for x in xs:`:

```cpp
int total = 0;
for (auto v : rolls) {   // 'auto' asks the compiler to deduce v's type (int here)
    total += v;
}
```

`auto` tells the compiler "figure out the type from the initializer." It's not dynamic typing —
the type is still fixed and checked; you just didn't have to spell it out. Use it when the type is
obvious or annoyingly long (you'll be grateful when iterator and template types appear later).

| Python | C++ |
|--------|-----|
| `xs = [3, 6, 2, 5, 4]` (can grow) | `std::array<int, 5> xs = {3, 6, 2, 5, 4};` (fixed size) |
| `for v in xs:` | `for (auto v : xs) {` |
| `x = 3` (type inferred, and mutable) | `auto x = 3;` (type inferred once, then fixed) |

> `std::array` is the safe, modern fixed array. There is also a raw C array (`int xs[5];`) — we
> avoid it because it doesn't know its own size and decays to a pointer. The growable
> `std::vector` (Python's real list) arrives in [stage 09](../09-stl-and-lambdas/).

---

## 6. Loops: `for` and `while`

Same ideas as Python, C-style syntax, and **braces instead of indentation**:

```cpp
for (int i = 3; i > 0; --i) {   // init; test; step  — a classic counter loop
    std::cout << i << " ";
}

while (hp > 0) {                // repeat while the condition is true
    hp -= poison;
}
```

The `for` header has three parts separated by `;`: an initializer, a continue-test, and a step
(`--i` decrements, `++i` increments). Course style: **always brace the body**, even a one-liner —
it prevents a notorious class of bugs where a second line silently falls outside the loop.

| Python | C++ |
|--------|-----|
| `for i in range(3, 0, -1):` | `for (int i = 3; i > 0; --i) {` |
| `while hp > 0:` | `while (hp > 0) {` |
| indentation defines the body | `{ ... }` braces define the body |

---

## 7. Branching: `if` and `switch`

`if` / `else if` / `else` is Python's `if` / `elif` / `else` with parentheses and braces:

```cpp
if (turns <= 3) {
    // ...
} else if (turns <= 5) {
    // ...
} else {
    // ...
}
```

`switch` picks one branch by matching a single **integer / `char` / enum** value against fixed
`case` labels. It's clearer than a long `else if` chain when you're testing *one* value against
many constants:

```cpp
switch (roll) {
    case 1:  /* fumble  */ break;
    case 20: /* crit    */ break;
    default: /* normal  */ break;   // like 'else'
}
```

**The one gotcha:** each `case` needs a `break`. Without it, execution "falls through" into the
next `case` — a classic C++ bug. Always `break` unless you fall through on purpose.

| Python | C++ |
|--------|-----|
| `if / elif / else` | `if / else if / else` |
| `match / case` (3.10+) | `switch / case` (integers, chars, enums only) |
| no fall-through | **must** `break` each case |

---

## 8. `<random>` — real randomness in two pieces

Python hides randomness behind `random.randint(1, 6)`. C++ splits it into two explicit parts, which
feels verbose but gives you precise control:

1. **A generator** — the engine producing raw random bits. `std::mt19937` (a Mersenne Twister) is a
   solid default. You **seed** it once; the same seed always produces the same sequence.
2. **A distribution** — shapes those bits into the range/shape you want.
   `std::uniform_int_distribution<int> die(1, 6);` gives a fair 1–6.

```cpp
#include <random>
std::mt19937 rng(12345);                          // seed -> reproducible sequence
std::uniform_int_distribution<int> die(1, 6);     // fair d6
auto face = die(rng);                             // roll: an int in [1, 6]
```

| Python | C++ |
|--------|-----|
| `import random` | `#include <random>` |
| `random.seed(12345)` | `std::mt19937 rng(12345);` |
| `random.randint(1, 6)` | `std::uniform_int_distribution<int> d(1,6); d(rng);` |

**Why the split is a feature:** a **fixed seed makes runs reproducible** — invaluable for
tutorials, tests, and debugging ("it crashed on seed 42, let me replay exactly that"). For genuine
unpredictability, seed from `std::random_device{}()` instead. This same two-piece model powers
particle jitter in [stage 22](../22-particle-systems/).

---

## Compiling and running

Everything is one file per program, built by hand (same as stage 00):

```bash
# an exercise
clang++ -std=c++17 -Wall -Wextra exercises/06_dice_roller.solution.cpp -o ex && ./ex

# the mini-project
clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o battle
./battle
echo $?        # -> 0
```

`-std=c++17` picks the language version; `-Wall -Wextra` turn on the warnings you should always fix.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference. Ordered easy → hard; the last one combines several concepts.

1. **`01_typed_variables`** *(easy)* — declare an `int` / `float` / `double` / `bool` and print
   them; feel what "static typing" means.
2. **`02_const_and_cast`** *(easy–medium)* — a `const` value, plus the integer-division trap and
   the `static_cast<double>` fix.
3. **`03_array_and_range_for`** *(medium)* — sum and scan a `std::array` with a range-`for` and
   `auto`.
4. **`04_loops_and_control`** *(medium)* — a counter `for`, a `while` drain loop, and an
   `if`/`else if`/`else` verdict.
5. **`05_switch_outcome`** *(medium–hard)* — range-`for` over rolls, classifying each with a
   `switch` (fumble / crit / hit).
6. **`06_dice_roller`** *(hard)* — `<random>` + `std::array` + a loop + `static_cast`, tallying
   d6 rolls into a percentage histogram. This is the dress rehearsal for the mini-project.

Try each yourself before peeking at the solution.

---

## Mini-project — Dice-roll battle simulator

**Your task.** [`project/src/main.cpp`](project/src/main.cpp) is a **starter** with `TODO`s — you
implement it, **combining every concept from this stage**: the numeric types, `const` tuning
values, `static_cast` for the damage, a `std::array` roster walked by range-`for` (and a classic
`for` for the preview), `auto` to bind each roll, `if` for HP checks, `switch` for the outcome, and
`<random>` for the rolls.

A lone hero runs a gauntlet against a fixed `std::array` of enemies. Each enemy swings once; a d20
roll decides how it lands (`switch`: a `1` fumbles, a `20` is a critical, everything else is a
normal hit), damage is a `float`-scaled roll made whole with `static_cast`, and the hero fights
until the roster is cleared or HP hits zero. The setup, roster preview and summary are given as
scaffolding — you fill in the two `TODO`s inside the fight loop (the swing `switch` and the HP
`if`/`else`).

Build the starter and run it:

```bash
clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o battle
./battle
echo $?          # should print 0
```

The seed is fixed (`121`), so what your finished program should print is exactly:

```
== The gauntlet ==
 1. Goblin
 2. Orc
 3. Slime
 4. Bandit
 5. Warg
Hero HP: 55

Goblin rolls 3 and hits for 2 damage. Hero HP: 53
Orc rolls 9 and hits for 8 damage. Hero HP: 45
Slime rolls 2 and hits for 1 damage. Hero HP: 44
Bandit rolls 20 — CRITICAL HIT for 36 damage. Hero HP: 8
Warg rolls 1 and FUMBLES for 0 damage. Hero HP: 8

== Result ==
Enemies defeated : 5 / 5
Total damage taken: 47
Avg damage / round: 9.4
The hero cleared the gauntlet with 8 HP to spare!
```

Change the seed to `std::random_device{}()` and the fight plays out differently every run. This
dice battle is the second link in the course's throughline (character sheet → **dice battle** →
game-math lib → …).

Stuck? A complete reference is in [`project/solution/src/main.cpp`](project/solution/src/main.cpp)
— try it yourself first, then compare:

```bash
clang++ -std=c++17 -Wall -Wextra project/solution/src/main.cpp -o battle
```

---

## Checklist before moving on

- [ ] I can explain why C++ needs a type for every variable, and what `auto` does (and doesn't do).
- [ ] I know why `3.14` is a `double` and `3.14f` is a `float`, and when I'd pick each.
- [ ] I can mark a value `const` and explain what it buys me.
- [ ] I can predict `7 / 2` vs `static_cast<double>(7) / 2`, and use `static_cast<int>` to truncate.
- [ ] I can walk a `std::array` with a range-`for` and `auto`.
- [ ] I can write a `for`, a `while`, an `if`/`else if`/`else`, and a `switch` (and I remember
      `break`).
- [ ] I can seed a `std::mt19937` and draw from a `std::uniform_int_distribution`.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md).

Next up: [Stage 02 — Functions & memory basics](../02-functions-and-memory/).
