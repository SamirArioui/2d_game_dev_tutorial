# Stage 05 — CMake basics, debugging & code style

**Goal:** stop typing compiler commands by hand. You'll write your first `CMakeLists.txt` (the
tool that generates and runs those commands for you), learn the **Debug vs Release** distinction,
run your first **`lldb`** debugging session, and meet the two style tools this course uses from
here on — **`clang-format`** and **`clang-tidy`**.

**New concepts:** a minimal `CMakeLists.txt` · configure vs build (`cmake -S . -B build` then
`cmake --build build`) · `Debug` vs `Release` (`-DCMAKE_BUILD_TYPE`) and what `NDEBUG` does to
`assert` · a first `lldb` session (`break` / `run` / `next` / `step` / `print` / `bt`) · auto-
formatting with `clang-format` · linting with `clang-tidy`.

Everything here maps onto tools you already know from Python:

| Python | C++ (this stage) | Role |
|--------|------------------|------|
| `pip` + a `Makefile`/`setup.py`/`tox` | **CMake** | describe & drive the build |
| `pdb` | **`lldb`** (or `gdb`) | interactive debugger |
| `black` | **`clang-format`** | opinionated auto-formatter |
| `flake8` / `pylint` | **`clang-tidy`** | linter / static analyzer |

---

## 1. Why CMake?

In stage 04 you compiled the math library by hand:

```bash
clang++ -std=c++17 -Wall -Wextra src/main.cpp src/mathutils.cpp src/player.cpp -o mathgame
```

That works for three files. It does **not** scale: real projects have hundreds of files, multiple
build configurations (debug/release), external libraries (SFML, Catch2 — coming later), and must
build on macOS, Linux, **and** Windows, where the compiler and its flags are all different. Typing
and maintaining those commands by hand is hopeless.

**CMake** is a *build-system generator*. You describe your project once, declaratively, in a file
called `CMakeLists.txt` ("what are my targets, what files build them, what standard"). CMake then
**generates** the actual build files for your platform (Makefiles / Ninja / a Visual Studio
solution) and drives the compiler for you — with the right flags, only rebuilding what changed.

> **Python comparison:** there's no perfect analogue because Python isn't compiled, but the closest
> feeling is a `Makefile` or a `tox.ini` / `pyproject.toml` build config: one file that captures
> *how to build/run the project* so nobody has to remember the raw commands. CMake is that, for the
> compile-and-link world.

### A minimal `CMakeLists.txt`

Here's the whole thing for a one-file program (exercise 01):

```cmake
cmake_minimum_required(VERSION 3.16)   # refuse ancient CMake whose defaults differ
project(hello_cmake LANGUAGES CXX)     # name it; CXX = "this is a C++ project"

set(CMAKE_CXX_STANDARD 17)             # compile as C++17 (the -std=c++17 you typed by hand)...
set(CMAKE_CXX_STANDARD_REQUIRED ON)    # ...and error out if the compiler can't do C++17

add_executable(hello main.cpp)         # build an executable named `hello` from main.cpp
```

Read it top to bottom: version floor, project declaration, language settings, then **targets**.
`add_executable(name sources...)` is the workhorse — it's CMake's `-o name file.cpp`. For several
sources you just list them (that's the mini-project below).

### Configure, then build

CMake is a **two-step** tool, and this trips people up at first:

```bash
cmake -S . -B build      # 1. CONFIGURE: read CMakeLists.txt, generate a build system into build/
cmake --build build      # 2. BUILD:     run that build system (compile + link)
./build/hello            # run the program CMake produced
```

- `-S .` = **source** dir (where `CMakeLists.txt` lives). `-B build` = **build** dir (a throwaway
  folder for all generated/compiled output — it's in `.gitignore`; delete it anytime to start
  clean).
- You only re-run **configure** when `CMakeLists.txt` changes. Day to day you just re-run
  `cmake --build build`, and CMake recompiles only the files you edited (the separate-compilation
  payoff from stage 04, now automatic).

> On Windows with Visual Studio, `cmake --build build --config Debug` picks the configuration at
> *build* time (it's a "multi-config" generator), and the binary lands in `build/Debug/`. On
> macOS/Linux with Makefiles or Ninja, the config is chosen at *configure* time (next section).

---

## 2. Debug vs Release

The same source can be compiled two very different ways:

- **Debug** — no optimization, full **debug symbols** (so `lldb` can map machine code back to your
  lines and show variable names), and `assert`s **active**. Slow but transparent. What you develop
  and debug with.
- **Release** — aggressive optimization (`-O2`/`-O3`), and the macro **`NDEBUG`** defined, which
  compiles every `assert()` **out to nothing**. Fast, but harder to step through. What you ship.

With a single-config generator you pick at configure time:

```bash
cmake -S . -B build     -DCMAKE_BUILD_TYPE=Debug     # symbols on, asserts on
cmake -S . -B build-rel -DCMAKE_BUILD_TYPE=Release   # optimized, asserts compiled out
```

(The course's CMake defaults to **Debug** if you don't specify, so the debugger "just works.")

**`assert` and `NDEBUG`** are worth understanding now (exercise 02). `assert(cond)` aborts with a
message if `cond` is false — but *only* in a Debug build. A Release build defines `NDEBUG`, and the
`assert` vanishes entirely. So asserts are free internal sanity checks during development that cost
nothing in the shipping binary.

> **Python comparison:** Python's `assert` is also switched off in "optimized" mode (`python -O`),
> but that's a **run-time** flag. C++ decides at **compile time**: a Release binary physically has
> no assert code in it. Same idea — "checks for development, gone in production" — different moment.

---

## 3. Your first debugger session (`lldb`)

When a program misbehaves, `print`-debugging only goes so far. A debugger lets you **pause** a
running program, **step** through it line by line, and **inspect** live variables. `lldb` is the
LLVM/Clang debugger (macOS default); `gdb` is its GNU counterpart (common on Linux) with nearly
identical commands. If you know Python's `pdb`, you already know the moves — only the spelling
changes:

| Do this | `pdb` | `lldb` (long form) | `lldb` (short) |
|---------|-------|--------------------|----------------|
| set a breakpoint | `b func` | `breakpoint set --name func` | `b func` |
| break at file:line | `b file.py:12` | `breakpoint set --file f.cpp --line 12` | `b f.cpp:12` |
| start running | `run` | `run` | `r` |
| execute one line, **over** calls | `n` | `next` | `n` |
| execute one line, **into** a call | `s` | `step` | `s` |
| print a variable/expression | `p x` | `print x` | `p x` |
| show all locals | — | `frame variable` | `fr v` |
| show the call stack | `bt` / `w` | `bt` | `bt` |
| resume until next stop/exit | `c` | `continue` | `c` |
| leave the debugger | `q` | `quit` | `q` |

**The one prerequisite:** build with **debug symbols** (a Debug build, i.e. `-g`). Optimized code
has been rearranged so much that stepping through it is confusing — always debug a Debug build.

Here's a real session on the mini-project (built with CMake, which defaults to Debug). We break
inside `clamp`, look at its arguments, step a line, and print the call stack. Comments (`#`) are
added for explanation — you don't type them:

```console
$ lldb ./build/mathgame
(lldb) breakpoint set --file mathutils.cpp --line 11   # pause at the first line of clamp()
Breakpoint 1: where = mathgame`clamp(float, float, float) + 16 at mathutils.cpp:11:5

(lldb) run                                             # start; it stops when clamp is called
...(the program's earlier output prints, then:)...
* thread #1, stop reason = breakpoint 1.1
    frame #0: mathgame`clamp(value=48, lo=0, hi=30) at mathutils.cpp:11:5
->  11      if (value < lo) {

(lldb) frame variable                                  # show all locals/args at this point
(float) value = 48
(float) lo = 0
(float) hi = 30

(lldb) next                                            # execute one line (step OVER any call)
->  14      if (value > hi) {

(lldb) print value                                     # inspect one variable
(float) 48

(lldb) bt                                              # backtrace: who called whom
  * frame #0: mathgame`clamp(value=48, lo=0, hi=30) at mathutils.cpp:14:5
    frame #1: mathgame`main at main.cpp:36:9            # clamp was called from main, line 36

(lldb) continue                                        # let the program finish
Process ... exited with status = 0
(lldb) quit
```

Two things to notice. The breakpoint report already shows the arguments — `clamp(value=48, lo=0,
hi=30)` — so you can often see the bug before typing anything. And `bt` gives you the *path* that
led here (`clamp` ← `main:36`), which is invaluable when a function is called from many places.
**Exercise 03** is a planted bug you'll find this way (watch a variable that never changes).

---

## 4. Code style tools: `clang-format` and `clang-tidy`

The course has a documented [`STYLE.md`](../../STYLE.md), but you don't enforce it by hand — two
tools do, configured by [`.clang-format`](../../.clang-format) and [`.clang-tidy`](../../.clang-tidy)
at the repo root. Both find their config by searching **upward** from the file, so they work from
any stage folder.

**`clang-format`** — the layout autoformatter, exactly like Python's **`black`**: indentation,
brace placement, spacing, line length. It never changes behavior, only appearance. There's nothing
to argue about — you run it and move on:

```bash
clang-format -i src/*.cpp src/*.hpp     # -i = edit files in place
```

**`clang-tidy`** — a linter / static analyzer, like **`flake8`**/**`pylint`**. It flags bug-prone
or non-idiomatic code that still *compiles fine*: using `0` where you mean `nullptr`, an
uninitialized variable, a C-style cast, an accidental copy of a big object, and so on. Each finding
names the **check** that produced it, in `[brackets]`:

```bash
clang-tidy src/main.cpp -- -std=c++17   # everything after -- are the COMPILER flags it needs
```

The `--` matters: clang-tidy has to *parse* the file the way the compiler would, so you pass the
compiler flags after `--`. (Once a project exports `compile_commands.json` — our CMake does, via
`CMAKE_EXPORT_COMPILE_COMMANDS` — clang-tidy can read the flags from there and you can drop the
`--` part.)

> **Installing them:** they ship with LLVM. macOS: `brew install llvm` (then use the full path
> `/opt/homebrew/opt/llvm/bin/clang-format` or add it to your `PATH`). Debian/Ubuntu:
> `sudo apt install clang-format clang-tidy`. See [`../../SETUP.md`](../../SETUP.md). They're
> optional-but-recommended from here on; the course code is already formatted and lint-clean.

Exercises **04** (`clang-format`) and **05** (`clang-tidy`) let you run each on messy input and
watch it get fixed.

---

## 5. Compiling and running (the mini-project)

The mini-project is the stage-04 math program, now built by CMake. From
[`project/`](project/):

```bash
cd project
cmake -S . -B build          # configure (defaults to a Debug build)
cmake --build build          # compile all three .cpp files and link them
./build/mathgame             # run
```

Expected output:

```
=== Stage 05 — the stage-04 program, built with CMake ===

Party:
  Aria the Bold (hp 8)

A +40 potion would clamp HP to 30 (max is 30, so the overheal is discarded).

Healing 8 -> 30 over 5 frames (lerp):
  frame 0: hp = 8
  ...
  frame 5: hp = 30

Distance to chest: 5
```

You can also configure the whole stage from its top folder — `cmake -S . -B build` from
`stages/05-cmake-and-debugging-basics/` works too (a thin `CMakeLists.txt` there forwards to
`project/`), and the repo-root `CMakeLists.txt` picks this stage up automatically.

---

## Exercises

In [`exercises/`](exercises/). Ordered easy → hard. Most are a starter `NN_name.cpp` (you edit) plus
its `NN_name.solution.cpp`; the CMake exercise is a small folder.

1. **`01_min_cmake/`** *(easy)* — fill in a minimal `CMakeLists.txt` (three TODOs) until
   `cmake -S . -B build && cmake --build build` produces a runnable program. Compare with
   `CMakeLists.solution.txt`.
2. **`02_assert_ndebug`** *(easy–medium)* — add an `assert`, then build the file **twice** (Debug
   vs `-DNDEBUG`) and watch the assert protect you in one build and disappear in the other.
3. **`03_find_the_bug`** *(medium)* — a program that prints the wrong number. Find the bug with
   `lldb` (`break` / `run` / `next` / `print`) instead of by eye, then fix it.
4. **`04_format_me`** *(easy)* — correct-but-ugly code; run `clang-format -i` and watch it snap to
   the house style. The solution is the formatted result.
5. **`05_tidy_me`** *(medium)* — code that compiles but trips three `clang-tidy` checks
   (uninitialized variable, `0`-as-pointer, C-style cast). Read each `[check-name]` and fix it
   until clang-tidy is silent.

---

## Your task — convert the stage-04 program to CMake, then debug & lint it

**You implement it.** [`project/`](project/) is a **starter**: the same sources as stage 04
(`mathutils.hpp/.cpp`, `player.hpp/.cpp`, `main.cpp`), now driven by a
[`project/CMakeLists.txt`](project/CMakeLists.txt) that is **given** for you. The two headers are
given (the contract) and the `CMakeLists.txt` is given; your job is to fill in each
`// TODO(stage 05)` in `main.cpp`, `mathutils.cpp`, and `player.cpp`. The starter already
configures, builds, and links (the bodies are stubbed with placeholders) — so you can practise the
CMake / `lldb` / `clang-format` / `clang-tidy` workflow immediately, then watch the output become
correct as you replace each stub.

A complete reference is in [`project/solution/`](project/solution/) — a self-contained CMake
build. Try it yourself first, then compare / build it standalone:

```bash
cmake -S project/solution -B build-sol && cmake --build build-sol && ./build-sol/mathgame
```

It exercises **every** concept of this stage. Walk through all three tools:

**1. Build it with CMake** (see §5). Confirm you get `build/mathgame` and it runs. Until you fill
in the `TODO`s the math prints placeholder zeros — implement the bodies to get the output in §5.

**2. Debug it with `lldb`.** Reproduce the session from §3 on your own build:

```bash
lldb ./build/mathgame
(lldb) breakpoint set --file mathutils.cpp --line 11   # inside clamp()
(lldb) run
(lldb) frame variable         # see value / lo / hi at the moment clamp is entered
(lldb) next                   # step a line
(lldb) bt                     # confirm clamp was called from main
(lldb) continue
(lldb) quit
```

Try it yourself: set a breakpoint inside the healing loop in `main.cpp` instead, `run`, and
`print` the loop variable `frame` and the interpolated `hp` on each `continue`.

**3. Format & lint it.** Prove the tools are happy with the course code, then break it and watch
them complain:

```bash
clang-format -i src/*.cpp src/*.hpp          # no changes: the code is already formatted
clang-tidy src/mathutils.cpp -- -std=c++17   # clean: no findings

# Now mangle src/mathutils.cpp (bad indentation; change a `nullptr` to `0`) and rerun both —
# clang-format fixes the layout, clang-tidy flags the 0. Then `git checkout` / undo to restore.
```

**Build Debug and Release and compare:**

```bash
cmake -S . -B build     -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B build-rel -DCMAKE_BUILD_TYPE=Release
```

Both produce the same output here (the program has no asserts), but the Release binary is optimized
and carries no debug symbols — try `lldb ./build-rel/mathgame` and notice stepping is far less
informative. That's *why* you debug the Debug build.

---

## Checklist before moving on

- [ ] I implemented the mini-project starter (`project/src/`), built it with CMake, and it prints
      the expected output.
- [ ] I can write a minimal `CMakeLists.txt` (`cmake_minimum_required`, `project`, `add_executable`).
- [ ] I know CMake is two steps — **configure** (`-S . -B build`) then **build** (`--build build`) —
      and that I only re-configure when `CMakeLists.txt` changes.
- [ ] I can explain **Debug vs Release** and what `NDEBUG` does to `assert`.
- [ ] I ran an `lldb` session: set a breakpoint, `run`, `next`, `print` a variable, read a `bt`.
- [ ] I know the Python→C++ tool mapping: `pdb`→`lldb`, `black`→`clang-format`,
      `flake8`/`pylint`→`clang-tidy`.
- [ ] I ran `clang-format` and `clang-tidy` and understand what each is (and isn't) for.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — the "Introduction to Modern CMake"
guide, the lldb tutorial, and the clang-format/clang-tidy docs.

Next up: [Stage 06 — Pointers, references & memory debugging](../06-pointers-references-asan/) —
where the debugger and a new tool (AddressSanitizer) become essential.
