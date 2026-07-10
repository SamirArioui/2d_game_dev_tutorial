# Stage 00 — Hello World & the toolchain

**Goal:** get one C++ program to compile and run, and — just as important — understand *what
actually happens* when it does. By the end you'll know why C++ has a separate "compile" step that
Python doesn't, how to read a compiler error, and how to run a program and check its exit code.

**New concepts:** compiled vs interpreted languages · the compiler toolchain (preprocess →
compile → link) · the anatomy of a `clang++` command · `#include`, `main`, `std::cout`,
`return` · reading a compiler error message. (*Translation units* are named here as a teaser and
covered properly in [stage 04](../04-headers-and-multi-file/).)

---

## 1. Why C++ has a step Python doesn't

In Python you run source directly:

```bash
python game.py        # the Python interpreter reads game.py and runs it, line by line
```

There is no separate build step. The `python` program *is* running your code.

C++ is **compiled**. Your source code is translated **ahead of time** into a standalone
executable full of CPU machine instructions. You run *that*, not the compiler:

```bash
clang++ game.cpp -o game     # translate game.cpp into a machine-code program called `game`
./game                       # run the program directly — the compiler isn't involved anymore
```

Why bother? Because the translation lets the compiler (a) catch whole classes of mistakes
*before* the program ever runs (wrong types, misspelled names, missing functions), and (b)
produce fast, optimized machine code with no interpreter overhead — which is exactly what a game
loop running 60+ times a second needs.

The trade-off is the one you're about to feel: you must compile before you can run, and the
compiler is strict. That strictness is a feature — it's a tireless reviewer that reads all your
code every time.

---

## 2. What "compiling" actually does

`clang++` is not one tool but a pipeline. For a single-file program the stages are:

```
   game.cpp
      │   1. Preprocessor  — handles lines starting with '#'. `#include <iostream>` is literally
      │                       replaced by the entire contents of the iostream header.
      ▼
 (expanded source)         — this fully-expanded source is called a TRANSLATION UNIT.
      │   2. Compiler      — translates the translation unit into an "object file" of machine
      │                       code (game.o). Type checking happens here.
      ▼
   game.o
      │   3. Linker        — stitches object file(s) together with the standard library into one
      │                       runnable executable, resolving every referenced function/symbol.
      ▼
    game  (executable)
```

For now everything lives in one file, so there's one translation unit and the pipeline is
invisible — `clang++ game.cpp -o game` runs all three steps for you. It matters later:

- **Translation unit** (compiler stage) — you'll meet this again in stage 04, when a program is
  split across several `.cpp` files and each is compiled separately.
- **Linker** (link stage) — you'll deliberately trigger a *linker error* in stage 04 to see the
  difference between "the compiler doesn't understand this file" and "the linker can't find this
  function".

---

## 3. The program, line by line

Look at [`project/src/main.cpp`](project/src/main.cpp) (the mini-project). The smallest useful
version is:

```cpp
#include <iostream>              // (1)

int main() {                    // (2)
    std::cout << "Hello, game dev!" << std::endl;   // (3)
    return 0;                   // (4)
}
```

1. **`#include <iostream>`** — a preprocessor directive. It pastes in the declarations for the
   input/output stream library so you can use `std::cout`. Python's `print` is built in; in C++
   you must include what you use. (More on headers in stage 04.)
2. **`int main()`** — every C++ program starts at a function called `main`. There is no
   module-level code that "just runs" like in Python; execution begins here. It returns an `int`.
3. **`std::cout << ... << std::endl;`** — `std::cout` is the standard output stream; `<<` sends
   values to it (read it as "put this into cout"). `std::endl` ends the line (and flushes). The
   `std::` prefix means "from the standard library namespace" (namespaces come in stage 08).
   Every statement ends with a **semicolon** — C++ does not use newlines to end statements the
   way Python does, so forgetting a `;` is the single most common beginner error.
4. **`return 0;`** — `main` returns an **exit code** to the operating system. `0` means success;
   any non-zero value signals an error. (You'll use this in exercise 05.)

### Python ↔ C++ at a glance

| Python | C++ |
|--------|-----|
| `print("hi")` | `std::cout << "hi" << std::endl;` |
| no imports needed for `print` | `#include <iostream>` |
| code at module level just runs | execution starts in `int main()` |
| newline ends a statement | `;` ends a statement |
| indentation defines blocks | `{ ... }` braces define blocks |
| `sys.exit(1)` | `return 1;` from `main` |

---

## 4. Compiling and running

From this folder:

```bash
# Compile. -std=c++17 picks the language version; -Wall -Wextra turn on helpful warnings.
clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o character_sheet

# Run it. The ./ means "the program in this directory".
./character_sheet

# See the exit code the program returned (0 = success).
echo $?
```

On Linux you can use `g++` instead of `clang++` with the exact same flags. On Windows, use the
"Developer PowerShell for VS 2022" and `cl`, or `g++` under MSYS2 (see [`../../SETUP.md`](../../SETUP.md)).

> **Get in the habit of always passing `-Wall -Wextra`.** Warnings are the compiler telling you
> "this compiles, but it's probably a bug." Treat them as errors to fix.

---

## 5. Reading a compiler error (a core skill)

You *will* get compiler errors constantly — that's normal, not a sign you're bad at this. The
skill is reading them. Say you forget a semicolon:

```cpp
std::cout << "Hello, game dev!" << std::endl   // <-- missing ;
return 0;
```

clang++ prints something like:

```
main.cpp:4:48: error: expected ';' after expression
    std::cout << "Hello, game dev!" << std::endl
                                               ^
                                               ;
```

Read it left to right:

- **`main.cpp:4:48`** — file, **line 4**, **column 48**. Go there first.
- **`error: expected ';' after expression`** — what's wrong, in the compiler's words.
- The **`^` caret** points at the exact spot; clang even suggests the fix (`;`) below it.

Two rules that save hours:
1. **Fix the *first* error first, then recompile.** One mistake often cascades into a dozen
   later errors that vanish once you fix the real one.
2. **Read the location, don't guess.** The `file:line:column` is almost always right.

Exercise 03 is a program that won't compile on purpose — your job is to read the error and fix it.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference solution. Ordered easy → hard. Compile any of them with:

```bash
clang++ -std=c++17 -Wall -Wextra exercises/01_hello.cpp -o ex && ./ex
```

1. **`01_hello`** *(easy)* — print a single line. Your first compile-run cycle.
2. **`02_title_screen`** *(easy)* — print a multi-line ASCII title screen; learn `"\n"` vs
   `std::endl` and chaining `<<`.
3. **`03_fix_the_error`** *(medium)* — a program with **three** deliberate compile errors. Read
   each error, fix it, recompile. This is the stage's key skill.
4. **`04_stats_block`** *(medium)* — print a neatly aligned character stats block from literal
   values, mixing text and numbers in one `std::cout` chain.
5. **`05_exit_code`** *(medium–hard)* — print an error to `std::cerr` and return a non-zero exit
   code from `main`; verify it with `echo $?`. Bridges toward "programs report success/failure".

Try each yourself before peeking at the solution.

---

## Mini-project — Character-sheet printer

**Your task.** [`project/src/main.cpp`](project/src/main.cpp) is a **starter** with `TODO`s — you
implement it, **combining everything from this stage**: `#include`, `main`, a multi-line formatted
`std::cout` block mixing text and numeric literals, and a clean `return 0`. Fill in each `TODO` so
the program prints exactly this tabletop-style character sheet:

```
============================
   CHARACTER SHEET
============================
 Name : Aria the Bold
 Class: Ranger
 Level: 3
----------------------------
 HP     : 30
 Attack : 12
 Defense: 8
============================
```

The values are hardcoded literals for now — in **stage 01** you'll make them real typed
variables, and in **stage 02** you'll compute derived stats with functions. This same character
threads through the whole course.

Build & run your version:

```bash
clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o character_sheet
./character_sheet
echo $?          # should print 0
```

Stuck? A complete reference is in [`project/solution/src/main.cpp`](project/solution/src/main.cpp)
— try it yourself first, then compare:

```bash
clang++ -std=c++17 -Wall -Wextra project/solution/src/main.cpp -o character_sheet
```

---

## Checklist before moving on

- [ ] I implemented the mini-project starter (`project/src/main.cpp`) and it prints the sheet.
- [ ] I can explain, in one sentence, why C++ compiles and Python interprets.
- [ ] I know the three pipeline stages (preprocess / compile / link).
- [ ] I can read a `file:line:column` compiler error and fix the first one first.
- [ ] I know `main` returns an exit code and how to check it with `echo $?`.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference, learncpp.com, and
Compiler Explorer (paste this program into godbolt.org to see the machine code it becomes).

Next up: [Stage 01 — Types, `const`, control flow & random](../01-types-const-control-flow/).
