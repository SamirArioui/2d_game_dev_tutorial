# Stage 06 — Pointers, references & memory debugging

**Goal:** understand the thing Python worked hard to hide from you — *where a value actually
lives in memory* — and the two tools C++ gives you to talk about it: **pointers** and
**references**. Then meet the heap (`new`/`delete`), the classic bugs that come with manual
memory management, and **AddressSanitizer**, the tool that catches those bugs for you.

**New concepts:** the address-of (`&`) and dereference (`*`) operators · `nullptr` · references
vs pointers (nullability, no-rebind, when to use each) · the stack vs the heap · `new` / `delete`
/ `new[]` / `delete[]` · dangling pointers, uninitialised pointers, use-after-free, leaks ·
building with `-fsanitize=address,undefined` and **reading an AddressSanitizer report**.

> This is the debugging thread's fourth step: compiler errors (00) → linker errors (04) → `lldb`
> (05) → **AddressSanitizer (06)**. And it's the foundation for stages 07 (destructors), 10
> (files), and 11 (smart pointers), where C++ starts managing this memory *for* you.

---

## 1. The mental model Python never made you learn: names vs. objects vs. addresses

In Python, a variable is a **name** bound to an **object**:

```python
x = 30          # the name x is bound to an int object holding 30
y = x           # y is bound to the SAME object (id(x) == id(y))
y = 25          # y is REBOUND to a new object; x is untouched
print(id(x))    # a number that stands in for "where this object lives" — but you can't use it
```

Python deliberately hides *where* objects live. You get `id()` as a read-only peek, and the
garbage collector frees objects when nothing refers to them. You never allocate or free by hand.

C++ makes all of it explicit. A variable is a **box in memory** with an **address**. You can ask
for that address (`&x`), store it in a **pointer**, and read or write the box **through** the
pointer (`*p`). Nothing is hidden and nothing is freed for you (until stage 11).

| Python | C++ | 
|--------|-----|
| name bound to a heap object | variable = a named box in memory |
| `id(x)` (opaque, read-only) | `&x` (a real, usable address) |
| everything is a reference to a heap object | you choose: value, pointer, or reference |
| GC frees objects automatically | you `delete` what you `new` (until stage 11) |
| `None` | `nullptr` |

---

## 2. `&` and `*`: address-of and dereference

Two operators do all the work. They are inverses of each other:

```cpp
int hp = 30;        // a box called hp, holding 30
int* p = &hp;       // &hp = "address of hp". p is a POINTER TO int, now aiming at hp's box.

std::cout << p;     // prints an address, like 0x16b3c93e0
std::cout << *p;    // *p = "the int p points at" -> 30   (dereference)

*p = 25;            // write THROUGH the pointer -> hp is now 25 (same box)
```

Read the type `int* p` as "`p` is a pointer to `int`". Per the course style the `*` binds to the
type: `int* p`, not `int *p`. Watch the two meanings of `*`:

- in a **declaration** (`int* p`) it's part of the type — "pointer to";
- in an **expression** (`*p`) it's the dereference operator — "the thing pointed at".

Likewise `&` means "address-of" in an expression (`&hp`) but "reference to" in a declaration
(`int& r`, next section). Same symbols, different jobs depending on where they appear.

---

## 3. References: a safer alias for "I definitely have an object"

A **reference** is a second name for an existing variable. You've quietly used them since stage
02 (`const&` parameters); here's what they really are:

```cpp
int score = 100;
int& alias = score;   // alias is ANOTHER NAME for score — not a copy, not a pointer
alias += 50;          // score is now 150
```

References trade flexibility for safety. Compared to a pointer, a reference:

- **cannot be null** — it must be bound to a real object when created (`int& r;` won't compile);
- **cannot be re-seated** — once `alias` names `score`, `alias = other` copies `other`'s *value*
  into `score`; it never makes `alias` name a different variable. A pointer *can* be re-pointed.

### When to use which

| Situation | Use | Why |
|-----------|-----|-----|
| The argument must always exist and you'll use it | **reference** (`const T&` / `T&`) | no null to check, cleaner call site (`f(x)` not `f(&x)`) |
| "No object" is a valid state / the target may change | **pointer** (`T*`, check for `nullptr`) | only a pointer can be null or re-seated |
| A function must modify the caller's variable | either | reference reads cleaner; pointer makes "this is an out-param" visible at the call (`f(&x)`) |

Rule of thumb this course follows: **prefer references; reach for a pointer when you genuinely
need null or re-seating.** (Exercise 03 makes the difference concrete: a "find" that can fail
returns a pointer so it can return `nullptr`; a reference couldn't express "not found".)

---

## 4. The stack vs. the heap, and `new` / `delete`

Local variables live on the **stack**: created when a function is entered, automatically
destroyed when it returns. That's convenient but limiting — a stack object dies at the end of its
function, and its size must be known at compile time.

The **heap** is memory you request explicitly and that lives until you release it:

```cpp
int* hp = new int(50);   // allocate ONE int on the heap, initialised to 50
std::cout << *hp;        // use it through the pointer
delete hp;               // hand it back — exactly one delete per new
hp = nullptr;            // good habit: null the pointer so it can't be reused by accident

int* arr = new int[5];   // allocate an ARRAY on the heap
// ... use arr[0..4] ...
delete[] arr;            // arrays made with new[] must be freed with delete[]
```

The rules that keep you out of trouble:

- **Every `new` needs exactly one matching `delete`** (and `new[]` ↔ `delete[]`).
- **Don't use memory after you `delete` it** (that's a *use-after-free* / *dangling pointer*).
- **Don't `delete` the same pointer twice** (a *double-free*).
- **Don't lose the only pointer to a `new`'d object** without deleting it (a *leak*).

> Coming from Python this feels alien: the GC did all of this. It's also why stage 11 introduces
> **smart pointers** (`unique_ptr`, `shared_ptr`), which tie `delete` to scope automatically —
> giving you back most of Python's convenience without the overhead of a garbage collector. This
> stage teaches the raw mechanics first so the smart-pointer version later makes sense.

---

## 5. AddressSanitizer: a bug detector you compile into your program

Memory bugs are nasty because they're often **invisible**: a use-after-free might print the right
answer today and corrupt something unrelated tomorrow. **AddressSanitizer (ASan)** makes them
loud and immediate. You turn it on with a compiler flag; it instruments every memory access and
**stops the program the instant it touches memory it shouldn't**.

```bash
# -fsanitize=address    turn on ASan (use-after-free, overflow, double-free, leaks*)
# -fsanitize=undefined  turn on UBSan (signed overflow, bad shifts, null deref, ...)
# -g                    keep debug info so the report can name files and lines
# -fno-omit-frame-pointer  keep readable stack traces
clang++ -std=c++17 -g -fsanitize=address,undefined -fno-omit-frame-pointer \
        exercises/06_use_after_free.cpp -o ex_asan
./ex_asan
```

Run the deliberately-buggy exercise 06 and you get a report like this (trimmed):

```
==NNN==ERROR: AddressSanitizer: heap-use-after-free on address 0x... 
READ of size 4 at 0x... thread T0
    #0 ... in main   06_use_after_free.cpp:40      <-- WHERE you touched freed memory
freed by thread T0 here:
    #0 ... operator delete
    #1 ... in main   06_use_after_free.cpp:36      <-- WHERE you called delete
previously allocated by thread T0 here:
    #0 ... operator new
    #1 ... in main   06_use_after_free.cpp:34      <-- WHERE you called new
SUMMARY: AddressSanitizer: heap-use-after-free ... in main
```

Read it like a compiler error, top-down:

1. **The kind of bug** — `heap-use-after-free`.
2. **The access that broke the rule** — `READ of size 4` + a stack trace pointing at the exact
   line that touched the freed memory.
3. **`freed by ... here`** — the stack trace of the `delete` that released it.
4. **`previously allocated ... here`** — the stack trace of the `new` that created it.

Those last two traces are the superpower: the tool literally shows you *where you freed it* and
*where you made it*, so fixing the dangling access is straightforward.

> **`*` on leaks and macOS:** ASan's *leak* detection (LeakSanitizer) doesn't run on macOS, so a
> pure leak may go unreported there — but *use-after-free*, *double-free*, and *buffer overflow*
> are caught on every platform. This stage's planted bug is a use-after-free for that reason.
>
> **Symbolizer note:** if you see `failed to spawn external symbolizer`, the addresses are still
> correct but not resolved to `file:line`. Installing / pointing to `llvm-symbolizer` fixes it;
> the report structure is identical either way.

---

## 6. Building and running

This is a **CMake** stage (introduced at stage 05). The `CMakeLists.txt` exposes an `ENABLE_ASAN`
option that is **ON by default**, so the whole stage is built with ASan + UBSan out of the box:

```bash
# From this stage folder. Configure once, then build.
cmake -S . -B build
cmake --build build

# The mini-project — a CORRECT program, so ASan stays silent and it exits 0.
./build/swap_and_list

# The planted bug — ASan prints the report above and aborts (exit code 134).
./build/ex_06_use_after_free_bug

# Every exercise solution is a target too:
./build/ex_05_swap
```

To build *without* the sanitizers (a plain, fast build):

```bash
cmake -S . -B build -DENABLE_ASAN=OFF
cmake --build build
```

> **Compiler note:** on macOS use Apple `clang++` (it bundles the ASan runtime); on Linux use a
> recent `clang++` or `g++` (with its sanitizer runtime installed). Point CMake at one explicitly
> with `-DCMAKE_CXX_COMPILER=clang++` if your default compiler can't link `-fsanitize=address`.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference solution. Build any one by hand, or use the CMake targets
(`ex_01_address_and_deref`, …). Ordered easy → hard:

1. **`01_address_and_deref`** *(easy)* — take a variable's address with `&`, store it in a
   pointer, read and write through it with `*`.
2. **`02_references`** *(easy)* — bind a reference as an alias; see that it can't be null or
   re-seated, and contrast with a pointer that can.
3. **`03_null_and_safety`** *(medium)* — a search that returns a pointer (so it can return
   `nullptr` for "not found"); null-check before dereferencing. When to pick pointer vs reference.
4. **`04_new_delete`** *(medium)* — allocate a single int and an array on the heap; free them with
   `delete` / `delete[]`.
5. **`05_swap`** *(medium)* — write `swap` two ways, by pointer and by reference; see why the
   reference version reads cleaner.
6. **`06_use_after_free`** *(medium–hard)* — **the planted bug.** A program that compiles and
   "works" but reads freed memory. Build it under ASan, read the report, and fix it. The starter
   is the buggy version; the `.solution.cpp` is the fix.

Try each yourself before peeking at the solution.

---

## Your task — pointer swap + a heap-allocated linked list

[`project/src/main.cpp`](project/src/main.cpp) is a **starter** with `TODO`s — you implement it,
**combining every concept in this stage**. `main()` already drives the demo; you fill in the four
stubbed functions:

- a **pointer-based `swap_ints`** (`swap_ints(&a, &b)`);
- a **singly linked list built on the heap** — a `struct Node { int value; Node* next; };`,
  with `push_front` (takes the head by reference, `Node*&`, so it can re-seat it), `print_list`
  (walks pointers to `nullptr`), and `free_list` (a matching `delete` for every `new`);
- `nullptr` as both the empty-list value and the end-of-chain marker.

Because ownership is clean — every `new` is matched by a `delete` — a correct program is **silent
under AddressSanitizer**. That's the other half of the ASan lesson: a *correct* program produces
*no* report. (In stage 07 a destructor will do `free_list`'s job automatically; in stage 11 a
smart pointer will.)

Build & run your version (ASan is ON by default for this stage):

```bash
cmake -S . -B build && cmake --build build
./build/swap_and_list
```

Expected output once implemented:

```
before swap: a=7 b=99
after  swap: a=99 b=7

list:       [1 -> 2 -> 3]
after free: []
```

Stuck? A complete reference is in [`project/solution/`](project/solution/) — it is self-contained
with its own `CMakeLists.txt`. Try it yourself first, then compare:

```bash
cmake -S project/solution -B build-solution && cmake --build build-solution
./build-solution/swap_and_list
```

---

## Checklist before moving on

- [ ] I can explain the difference between a variable, its address (`&x`), and dereferencing
      (`*p`) — and how that maps onto Python's names-vs-objects model.
- [ ] I can state two things a reference can't do that a pointer can (be null; be re-seated), and
      pick the right one for a given API.
- [ ] I can allocate and free heap memory correctly (`new`/`delete`, `new[]`/`delete[]`), and I
      know what a use-after-free, double-free, and leak are.
- [ ] I built a program with `-fsanitize=address,undefined` and read a report top-down: the bug
      kind, the offending access, the `freed here`, and the `allocated here`.
- [ ] I implemented the mini-project starter (`project/src/main.cpp`); it runs clean under ASan
      (no report), and I understand *why* that's the goal.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference on pointers/references,
and the AddressSanitizer docs. Paste the linked list into godbolt.org to see the `new`/`delete`
calls in the generated assembly.

Next up: [Stage 07 — Classes & OOP](../07-classes-and-oop/), where a **destructor** will free
resources automatically — the first step of RAII, and the beginning of the end of manual `delete`.
