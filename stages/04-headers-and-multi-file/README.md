# Stage 04 — Headers & multi-file compilation

**Goal:** grow beyond a single `.cpp` file. You'll learn *why* C++ splits code into headers
(`.hpp`) and sources (`.cpp`), what actually happens when you `#include` one, how to compile
several `.cpp` files and link them into one program **by hand**, and — the new debugging skill —
how to read a **linker error** and tell it apart from a compiler error.

**New concepts:** declaration vs definition · header (`.hpp`) vs source (`.cpp`) · `#pragma once`
· `#include` as literal text substitution · the One Definition Rule (ODR) · compiling & linking
multiple translation units · reading a *linker* error (vs a *compiler* error).

This is the last hand-compiled stage. From [stage 05](../05-cmake-and-debugging-basics/) a build
tool (CMake) issues these commands for you — but only once you've felt what it's automating.

---

## 1. Why split code into `.hpp` and `.cpp`?

In [stage 00](../00-hello-cpp/) you saw the compiler pipeline: **preprocess → compile → link**.
The key fact this stage builds on: the compiler processes **one `.cpp` file at a time**, in total
isolation. Each fully-preprocessed `.cpp` is a **translation unit** (TU). When compiling
`main.cpp`, the compiler knows *nothing* about what's in `mathutils.cpp` — not even that it exists.

So how can `main.cpp` call a function whose body lives in `mathutils.cpp`? This is the problem
headers solve, and it rests on one distinction:

- A **declaration** says *"this thing exists — here's its name, parameter types, and return
  type."* It's a promise, no body. `float clamp(float value, float lo, float hi);`
- A **definition** provides the actual **body** — the code that runs.
  `float clamp(float value, float lo, float hi) { ... }`

To *call* a function, a TU only needs its **declaration** (so the compiler can type-check the
call). The **definition** can live in a completely different `.cpp`, compiled separately; the
linker connects the call to the body at the end.

That gives us the convention every C++ project follows:

| File | Holds | Analogy |
|------|-------|---------|
| **`.hpp`** (header) | **declarations** — the *public interface* | a menu: what you can order |
| **`.cpp`** (source) | **definitions** — the *implementation* | the kitchen: how it's made |

Any `.cpp` that wants to use the library `#include`s the header (gets the promises) and is
compiled on its own. The one `.cpp` that *implements* the library holds the bodies, compiled once.
The linker ties them together.

> Why bother? Two reasons. **(1) Separate compilation:** change one `.cpp` and you only recompile
> that file, then relink — not the whole project. On a big codebase that's the difference between
> a 1-second and a 10-minute build. **(2) Interface vs implementation:** the header is a contract.
> Users read the header to learn what's available without reading (or being able to break) the
> implementation.

### Python comparison — this is genuinely new

Python has **no headers and no separate compilation.** You write a function once and `import` it:

```python
# mathutils.py
def clamp(value, lo, hi):
    return max(lo, min(value, hi))

# main.py
import mathutils                 # finds, loads, and RUNS mathutils.py, creating a module OBJECT
print(mathutils.clamp(5, 0, 3))
```

`import` is a **runtime** operation: it locates the module, executes it, and binds a live module
object you access with `mathutils.clamp`. There is no "declaration"; the function *is* its
definition, and the interpreter resolves the name when the line runs.

C++ `#include` is **not** `import`. Keep these three differences front of mind:

| | Python `import mathutils` | C++ `#include "mathutils.hpp"` |
|--|--------------------------|-------------------------------|
| When | at **run time** | at **compile time** (preprocessor) |
| What it does | loads & runs the module, makes a module object | **pastes the header's text** into this file |
| Names | accessed as `mathutils.clamp` | dropped straight into your file: `clamp(...)` |

The next two sections unpack that middle row.

---

## 2. `#include` is literal text substitution

The preprocessor is a glorified copy/paste machine. When it hits

```cpp
#include "mathutils.hpp"
```

it finds that file and **replaces the line with the entire contents of the file** — verbatim,
before the compiler proper ever runs. Nothing is "loaded" or "executed." If `mathutils.hpp`
contains three function declarations, then after preprocessing your `.cpp` literally *contains*
those three lines where the `#include` used to be. (Angle brackets `<iostream>` search the
system/standard include paths; quotes `"mathutils.hpp"` search next to your file first.)

You can see the pasted result. `-E` runs *only* the preprocessor and prints the expanded TU:

```bash
clang++ -std=c++17 -E project/src/main.cpp | grep -v '^#' | tail -n 40
```

You'll see your `main` preceded by the declarations that were pasted in from the headers. That
expanded text — your file plus everything it pulled in — is the translation unit the compiler
actually reads.

---

## 3. `#pragma once` — the include guard

Because `#include` is blind text substitution, it's easy to paste the same header **twice** into
one TU (e.g. `main.cpp` includes `player.hpp` and also `world.hpp`, and `world.hpp` *also*
includes `player.hpp`). If that header defines a `struct Player`, the compiler now sees two
definitions of `Player` in one TU — an error:

```
error: redefinition of 'Player'
```

The fix is the first line of **every** header in this course:

```cpp
#pragma once
```

It tells the preprocessor: *"however many times this file is `#include`d in one translation unit,
paste it only once."* Problem gone.

> **Python note:** you never think about this because `import mathutils` twice just reuses the
> already-loaded module object — Python caches modules in `sys.modules`. C++ has no such cache at
> the preprocessor level, so *you* add the guard.

(You may also see the older classic form — `#ifndef PLAYER_HPP` / `#define PLAYER_HPP` / `#endif`.
It does the same job; `#pragma once` is shorter, and supported by every compiler this course
targets. We use `#pragma once`.)

---

## 4. The One Definition Rule (ODR)

The rule that governs all of this, in one sentence:

> **Every non-inline function and every variable must be defined *exactly once* across the whole
> program — no fewer, no more.** Types (like a `struct`) must be defined once *per translation
> unit*, and every such definition must be identical.

Two ways to break it, and they're the two linker errors you'll meet below:

- **Zero definitions** → *"undefined reference / undefined symbols."* You declared a function and
  called it, but never wrote its body anywhere. (Exercise 03.)
- **Two definitions** → *"duplicate symbol."* The same function body ended up in two translation
  units. The classic cause: putting a function *definition* in a header (not just a declaration),
  so every `.cpp` that includes it gets its own copy. `#pragma once` does **not** save you here —
  it only dedups *within one TU*, not *across* TUs. (Exercise 04.)

That's why the convention is strict: **declarations in the header, definitions in one `.cpp`.**
The one deliberate exception is a **type**: a `struct`/`class` *definition* goes in the header,
because every TU that uses the type must see its full layout. That's ODR-legal precisely because
every copy is identical (and `#pragma once` keeps it to one copy per TU). See `player.hpp` in the
mini-project.

---

## 5. Compiling and running (by hand)

With several `.cpp` files, you drive the pipeline yourself. Two mental models — both produce the
same program:

**A) One command, all at once** (simplest; the compiler compiles each TU then links):

```bash
# From this stage folder. List every .cpp — headers are NOT listed (they're #included).
clang++ -std=c++17 -Wall -Wextra \
    project/src/main.cpp project/src/mathutils.cpp project/src/player.cpp \
    -o mathgame
./mathgame
```

**B) Compile each TU, then link** (shows what's really happening — and what CMake will automate):

```bash
# 1. COMPILE each translation unit to an object file (-c = "compile only, don't link").
clang++ -std=c++17 -Wall -Wextra -c project/src/mathutils.cpp -o mathutils.o
clang++ -std=c++17 -Wall -Wextra -c project/src/player.cpp    -o player.o
clang++ -std=c++17 -Wall -Wextra -c project/src/main.cpp      -o main.o

# 2. LINK the object files into one executable (no -c, no -std needed here).
clang++ main.o mathutils.o player.o -o mathgame
./mathgame
```

Notice you **never** pass `.hpp` files to the compiler. Headers are consumed via `#include`; they
are not separate translation units. Listing a header on the command line is a common beginner
mistake.

> The payoff of model **B**: edit only `player.cpp`, recompile just `player.o`, relink. The other
> object files are untouched. That's *separate compilation* — the reason big C++ projects don't
> rebuild everything on every keystroke. Managing this by hand gets old fast, which is exactly the
> motivation for CMake in stage 05.

---

## 6. Reading a linker error (the new skill)

Stage 00 was about *compiler* errors (`file:line:column`, wrong types, missing `;`). Linker errors
look different and come from a different tool (`ld`). Learning to tell them apart saves real time.

**Undefined symbol** — you called something with no definition anywhere:

```
Undefined symbols for architecture arm64:
  "armor_rating(int, int)", referenced from:
      _main in main.o
ld: symbol(s) not found for architecture arm64
clang++: error: linker command failed with exit code 1
```

Read the tells:
- **`ld:`** — that's the *linker*, not the compiler. This error appears *after* every file
  compiled successfully.
- **No `file:line:column`.** The linker works with symbols and object files, not source lines. It
  can tell you *what* symbol is missing and which object *referenced* it, but not a source line.
- **`Undefined symbols` / `referenced from`** — a body is missing. Either you forgot to write the
  definition, or you forgot to pass its `.cpp`/`.o` on the command line.

**Duplicate symbol** — the same thing is defined twice:

```
duplicate symbol 'max_party_size()' in:
    04_duplicate_symbol_odr.o
    04_duplicate_symbol_odr_lib.o
ld: 1 duplicate symbols
```

Fix: keep exactly one definition (move it to a single `.cpp`, or mark a header helper `inline`).

**The rule of thumb:** *"compiler error"* = one file doesn't make sense on its own (wrong syntax,
unknown type, bad call). *"linker error"* = the files are individually fine but don't fit together
(a body is missing, or defined twice). **Exercise 03** makes you trigger an undefined-symbol error
on purpose — and proves the file still *compiles* (`-c`) even though it won't *link*.

---

## Exercises

In [`exercises/`](exercises/). Ordered easy → hard. Some span multiple files: a starter
`NN_name.cpp` (the file **you** edit) plus its `NN_name.solution.cpp`, and — where noted —
**support files** (`.hpp` / `*_main.cpp` / `*_lib.cpp`) that are already complete and marked *"do NOT edit."*
Compile the support files *alongside* the exercise (each exercise's header comment shows the exact
command).

1. **`01_declaration_vs_definition`** *(easy, single file)* — declare a function at the top, define
   it at the bottom; see that a declaration is enough to *call* it. The idea headers are built on.
2. **`02_split_across_files`** *(medium, multi-file)* — write the definitions (`.cpp`) for a given
   header, then compile two translation units into one program. Your first real hand-link.
3. **`03_undefined_reference`** *(medium, single file)* — **triggers a linker error on purpose.** A
   function is declared and called but never defined. Prove it *compiles* (`-c`) yet fails to
   *link*, then fix it by adding the definition. This is the stage's key debugging skill.
4. **`04_duplicate_symbol_odr`** *(medium–hard, multi-file)* — the opposite ODR failure: the same
   function defined in two TUs → *duplicate symbol*. Fix it so exactly one definition survives.
5. **`05_shared_struct`** *(medium, multi-file)* — a `struct` defined in a header and shared across
   two `.cpp` files; you write the functions that operate on it. The mini-project's pattern in
   miniature.

Try each yourself before peeking at the solution. Compile examples, e.g.:

```bash
cd exercises
clang++ -std=c++17 -Wall -Wextra 02_split_across_files.cpp 02_split_across_files_main.cpp -o ex && ./ex
```

---

## Your task — split the game-math library

**You implement it.** [`project/src/`](project/src/) is a **starter** that combines **every**
concept from this stage across **three translation units and two headers**:

```
project/src/
  mathutils.hpp   # DECLARATIONS: clamp / lerp / distance          (#pragma once) — GIVEN (the contract)
  mathutils.cpp   # DEFINITIONS of those three functions           — YOU fill in the bodies
  player.hpp      # a shared TYPE: struct Player { std::string name; int hp; } + print_player() (#pragma once) — GIVEN
  player.cpp      # DEFINITION of print_player                     — YOU fill in the body
  main.cpp        # includes both HEADERS; uses the library; holds main() — YOU fill in main()
```

The two **headers are given** — they're the *contract* (the declarations every file agrees on).
Your job is to write the **definitions**: fill in each `// TODO(stage 04)` in `mathutils.cpp`,
`player.cpp`, and `main.cpp`. The starter already compiles and links (the bodies are stubbed with
placeholders); as you replace each stub the program grows toward the expected output below.

`main.cpp` includes only the two *headers* (the declarations); the *definitions* you write in
`mathutils.cpp` and `player.cpp` are joined by the linker. `struct Player` demonstrates a type
shared across TUs: both `main.cpp` and `player.cpp` include `player.hpp`, so both agree on its
layout.

Build & run your version (list every `.cpp`; headers are NOT listed — they're `#include`d):

```bash
clang++ -std=c++17 -Wall -Wextra \
    project/src/main.cpp project/src/mathutils.cpp project/src/player.cpp \
    -o mathgame
./mathgame
```

Expected output once every `TODO` is done:

```
=== Stage 04 — multi-file math demo ===

Party:
  Aria the Bold (hp 8)

A +40 potion would clamp HP to 30 (max is 30, so the overheal is discarded).

Healing 8 -> 30 over 5 frames (lerp):
  frame 0: hp = 8
  frame 1: hp = 12.4
  frame 2: hp = 16.8
  frame 3: hp = 21.2
  frame 4: hp = 25.6
  frame 5: hp = 30

Distance to chest: 5
```

Stuck? A complete reference is in [`project/solution/src/`](project/solution/src/) — try it
yourself first, then compare:

```bash
clang++ -std=c++17 -Wall -Wextra \
    project/solution/src/main.cpp project/solution/src/mathutils.cpp project/solution/src/player.cpp \
    -o mathgame
```

**Poke it to learn:** delete `#pragma once` from `player.hpp` and add a second
`#include "player.hpp"` to `main.cpp` → *redefinition of 'Player'* (a compile error). Or leave
`mathutils.cpp` off the link command → *undefined symbols* for `clamp`/`lerp`/`distance` (a link
error). Restore afterward. Breaking it on purpose is the fastest way to internalize which stage
each error comes from.

> In [stage 05](../05-cmake-and-debugging-basics/) this exact project becomes your first CMake
> build — CMake will run the compile-each-then-link commands you just typed by hand.

---

## Checklist before moving on

- [ ] I implemented the mini-project starter (`project/src/`) — filled in `main.cpp`,
      `mathutils.cpp`, and `player.cpp` — and it links and prints the expected output.
- [ ] I can state the difference between a **declaration** and a **definition**.
- [ ] I know why `.hpp` holds declarations and `.cpp` holds definitions, and that headers are
      never passed to the compiler directly.
- [ ] I understand `#include` is **text substitution** at compile time, not Python's `import`.
- [ ] I know what `#pragma once` prevents and why blind text-pasting makes it necessary.
- [ ] I can state the **One Definition Rule** and both ways to break it.
- [ ] I compiled several `.cpp` into one program, both in one command and via `-c` + link.
- [ ] I triggered an **undefined-symbol** linker error and can tell a *linker* error from a
      *compiler* error at a glance (look for `ld:` and the absence of `file:line:column`).

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference on the preprocessor and
the ODR; and paste a two-file program into godbolt.org (Compiler Explorer) to watch each TU compile
independently.

Next up: [Stage 05 — CMake basics, debugging & code style](../05-cmake-and-debugging-basics/).
