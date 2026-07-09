# Stage 02 — Functions & memory basics

**Goal:** write your own functions and understand the three ways C++ can hand an argument to one —
by value, by reference, by `const` reference — plus function overloading. Along the way, build the
mental model of the **call stack** that explains *where* your variables live and why references
work. By the end you'll have a small, reusable game-math toolbox.

**New concepts:** defining/calling functions · pass **by value** / **by reference** / **by
`const&`** · function **overloading** · the **stack vs heap** mental model (conceptual — real heap
use arrives in [stage 06](../06-pointers-references-asan/)).

> Prerequisite: [stage 01](../01-types-const-control-flow/) — types, `const`, `std::array`, loops.

---

## 1. Defining a function

A C++ function must declare the type it **returns** and the type of **every parameter** — there's
no inference at the boundary like Python's untyped `def`:

```cpp
int add(int a, int b) {   // returns int; takes two ints
    return a + b;
}
```

```python
def add(a, b):            # Python: no types written
    return a + b
```

A function that returns nothing uses `void`:

```cpp
void greet() {
    std::cout << "Ready!\n";
}
```

Two C++ rules with no Python counterpart:

- A function must be **declared before it's used** (the compiler reads top-to-bottom). For now,
  just define your helpers *above* `main`. [Stage 04](../04-headers-and-multi-file/) introduces
  header *declarations* so order stops mattering across files.
- The types are part of the contract the compiler checks at every call site.

---

## 2. The call stack — where your variables live

This is the key mental model of the stage. When a function is called, the program sets aside a
block of memory called a **stack frame** to hold that call's parameters and local variables. When
the function returns, its frame is **popped** and that memory is instantly reclaimed. Calls stack
up and unwind like a stack of plates — hence "the call stack."

```
main()            ── frame: hero_level, gold, ...
  └─ level_up()   ── frame: level (a COPY)      <- top of stack while running
                     ...returns -> this frame is popped, 'level' is gone
```

Why this matters:

- **Local variables are automatic.** They're born when the frame is pushed and die when it's
  popped. You don't `free` them — no manual cleanup, no garbage collector pauses. This is why C++
  locals are fast and predictable.
- It explains **pass-by-value**: a value parameter is just a *new local variable in the callee's
  frame*, initialized from a copy of the caller's argument. Scribbling on it can't reach back into
  the caller's frame.
- It also explains a hazard for later: never hand out the address of a local — once its frame is
  popped, that memory is garbage. (We'll weaponize that lesson in stage 06.)

### Stack vs heap (the preview)

There are two regions where data lives:

| | **Stack** | **Heap** |
|--|-----------|----------|
| Lifetime | automatic — tied to the enclosing `{ }` block/call | manual/explicit — you decide when it's freed |
| Speed | very fast (just moves a pointer) | slower (a real allocator) |
| Size | limited (megabytes) | large |
| Used for | locals, parameters, small fixed data | big or variable-sized data whose lifetime outlives one call |

**Everything in this stage lives on the stack.** You won't touch the heap explicitly until
[stage 06](../06-pointers-references-asan/) (`new`/pointers) and won't do it *safely and idiomatically*
until [stage 11](../11-smart-pointers-move-rule-of-five/) (smart pointers). Python, by contrast,
puts essentially *everything* on the heap and frees it with a garbage collector; you never think
about it. In C++ the stack/heap split is something you choose, and choosing the stack whenever you
can is the default that keeps code fast and leak-free.

---

## 3. The three ways to pass an argument

This is the heart of the stage. When you call `f(x)`, how does `x` reach `f`?

### By value — `void f(int x)`

`x` is a **copy**. Changes inside `f` don't touch the caller's variable. Great for small things
(an `int`, a `float`, a `bool`) where a copy is free.

```cpp
void level_up(int level) { level += 1; }   // bumps the COPY only

int hero_level = 3;
level_up(hero_level);
// hero_level is STILL 3
```

### By reference — `void f(int& x)`

`x` is an **alias** for the caller's variable — no copy, and writing through it changes the
**original**. This is how a function returns extra results or edits something in place:

```cpp
void add_xp(int& xp, int amount) { xp += amount; }   // & means "the caller's own xp"

int xp = 0;
add_xp(xp, 30);
// xp is now 30
```

### By `const` reference — `void f(const BigThing& x)`

`x` is an alias (no copy) **that you promise not to modify**. This is the everyday default for
passing anything bigger than a number that you only want to read: you skip the copy *and* the
`const` guarantees read-only access.

```cpp
int sum(const std::array<int, 5>& values) {   // no 5-int copy; can't modify values
    int total = 0;
    for (int v : values) { total += v; }
    return total;
}
```

**How to choose** (a rule you'll keep for the whole course):

| You want to… | Use | Why |
|--------------|-----|-----|
| read a small value (`int`, `float`, `bool`) | by value `int x` | a copy is free |
| read a big value without copying | `const T&` | avoids the copy, can't accidentally change it |
| modify the caller's variable | `T&` | changes are meant to be visible |

> **Python contrast — two surprises.** (1) Python has *no* way to make a function reassign a
> caller's integer/string; immutables can never be changed through a parameter. C++ references make
> that both possible and explicit. (2) Python has no copy-vs-reference *choice* and no `const`, so
> the whole "how should I pass this?" decision is new. Right now our types are tiny, so `const&`
> barely earns its keep — but the moment real types exist (`std::string` in
> [stage 03](../03-enums-and-structs/), `Vec2` in [stage 08](../08-operators-templates-namespaces/)),
> passing by `const&` instead of by value is the difference between copying a whole object every
> call and not.

---

## 4. Function overloading

C++ lets **several functions share one name** as long as their parameter lists differ. The compiler
picks the right one from the argument types at the call site:

```cpp
int    clamp(int v, int lo, int hi)       { /* ... */ }
float  clamp(float v, float lo, float hi) { /* ... */ }

clamp(150, 0, 100);          // -> int overload
clamp(-0.3f, 0.0f, 1.0f);    // -> float overload
```

The name stays meaningful (`clamp` is `clamp` whatever the type), and there's no runtime cost —
the choice is made at **compile time**. Note: the parameter *lists* must differ; you can't overload
on return type alone.

```python
# Python has no overloading — a second `def clamp` simply REPLACES the first.
# You'd use dynamic typing, default arguments, or functools.singledispatch instead.
```

Overloading is a stepping stone to **templates** ([stage 08](../08-operators-templates-namespaces/)),
where you write the body *once* and the compiler generates a version per type — but seeing the
manual version first makes clear what templates automate.

---

## Compiling and running

One file per program, built by hand:

```bash
# an exercise
clang++ -std=c++17 -Wall -Wextra exercises/05_reflect_and_clamp.solution.cpp -o ex && ./ex

# the mini-project
clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o gamemath
./gamemath
echo $?        # -> 0
```

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference. Ordered easy → hard.

1. **`01_first_function`** *(easy)* — define and call functions; prove that pass-by-**value** leaves
   the caller's variable unchanged.
2. **`02_pass_by_reference`** *(easy–medium)* — reference parameters that *do* change the caller's
   variables (including one call that updates two of them).
3. **`03_const_ref`** *(medium)* — pass a `std::array` by `const&`: no copy, read-only.
4. **`04_overloading`** *(medium)* — two overloads each of `describe` and `max_of`; watch the
   compiler pick by argument type.
5. **`05_reflect_and_clamp`** *(medium–hard)* — combine references + overloading: an in-place
   `reflect(float&, float&)` and an overloaded `clamp(int)` / `clamp(float)`. This is the
   mini-project's core.

Try each yourself before peeking at the solution.

---

## Mini-project — Game-math function set

In [`project/src/main.cpp`](project/src/main.cpp). A small toolbox of the math every 2D game leans
on, written as free functions so each demonstrates a different passing style:

- `clamp` — **overloaded** for `int` and `float`, both **by value**.
- `lerp(a, b, t)` — linear interpolation, by value.
- `distance(x1, y1, x2, y2)` — Pythagoras, by value (`<cmath>`'s `std::sqrt`).
- `magnitude(const std::array<float, 2>&)` — a vector length, passed **by `const&`** (a 2-float
  array is small, but this rehearses the pattern you'll use for `Vec2` and containers later).
- `reflect(float& vx, float& vy)` — bounce a velocity straight back, **in place via reference
  out-parameters**.

It **exercises every concept from this stage**: value / reference / `const&` parameters, overloading,
and (in the code comments + this README) the stack model. Sample output:

```
== Game-math toolbox ==

clamp(150, 0, 100)        = 100  (int overload)
clamp(-0.3f, 0.0f, 1.0f)  = 0  (float overload)

lerp(20, 100, t):
  t=0 -> 20
  t=0.25 -> 40
  t=0.5 -> 60
  t=0.75 -> 80
  t=1 -> 100

distance((0,0),(3,4))     = 5
magnitude({3, 4})         = 5

Before reflect: velocity = (5, -2.5)
After  reflect: velocity = (-5, 2.5)
```

These functions are the third link in the throughline (dice battle → **game-math lib** →
inventory → …). In [stage 04](../04-headers-and-multi-file/) you'll split this exact toolbox across
a `.hpp`/`.cpp` pair; in [stage 15](../15-2d-vector-math-and-motion/) `lerp`/`distance`/`reflect`
become the backbone of real motion.

---

## Checklist before moving on

- [ ] I can define a function with an explicit return type and typed parameters, and I know it must
      appear before it's called.
- [ ] I can sketch the call stack and explain why a value parameter can't change the caller's
      variable.
- [ ] I can pass by value, by reference (`T&`), and by `const&`, and say when to use each.
- [ ] I can write a function that returns extra results through reference out-parameters
      (`reflect`).
- [ ] I can overload a function on parameter types and explain why Python can't.
- [ ] I can state, in one sentence, the difference between the stack and the heap — and that
      everything here lives on the stack.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md).

Next up: [Stage 03 — Enums, structs & strings](../03-enums-and-structs/).
