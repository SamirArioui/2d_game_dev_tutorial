# Stage 03 — Enums, structs & strings

**Goal:** bundle related data into your own types. So far a "character" has been loose variables and
literals; here it becomes a `Stats` struct, items become an `Item` struct, and a fixed vocabulary
(damage types) becomes an `enum class`. You'll also meet `std::string`, C++'s text type. These are
the data shapes the rest of the course is built on.

**New concepts:** `enum class` + `switch` over it · `struct` vs `class` · brace / aggregate
initialization · `std::string` basics (vs Python `str`).

> Prerequisite: [stages 01–02](../01-types-const-control-flow/) — types, `const`, `switch`, and
> functions (we pass structs to functions here).

---

## 1. `struct` — bundling named fields into a new type

A `struct` groups several named values into **one new type**. It's C++'s tool for "a thing with
fields," closest to a Python **dataclass** (or a dict with fixed keys):

```cpp
struct Stats {
    int hp;
    int attack;
    int defense;
};   // <-- the semicolon after a struct is REQUIRED (a classic beginner error)
```

```python
from dataclasses import dataclass
@dataclass
class Stats:
    hp: int
    attack: int
    defense: int
```

You access fields (called **members**) with a dot, just like Python attributes:

```cpp
Stats hero{30, 14, 8};
hero.hp -= 5;                     // members are normal variables you can read and write
std::cout << hero.attack;         // 14
```

**Memory model — worth internalizing:** a `struct` is just its members laid out **back-to-back in
memory**, in declaration order, as one contiguous block. A `Stats` is essentially three `int`s
side by side — no hidden header, no boxing, no pointer indirection like a Python object. That's why
a `struct` is cheap to create on the stack and fast to copy, and why the field *order* is part of
its layout. (A copy is a member-by-member copy, which is why we pass big structs by `const&`.)

---

## 2. `struct` vs `class` — the same thing, one default apart

In C++ `struct` and `class` are **nearly identical**. The *only* language difference:

- `struct` members are **`public` by default** (anyone can touch them).
- `class` members are **`private` by default** (only the type's own functions can).

```cpp
struct Point { int x; int y; };          // x, y are public — free to read/write

class Account {
    int balance_;                        // private by default: hidden from outside
public:
    int balance() const { return balance_; }   // controlled read access
};
```

The **convention** this course follows: use `struct` for **plain data** with no invariant to
protect (public fields, like `Stats` and `Item`), and use `class` when the type must **guard its
own state** behind methods (encapsulation). Full `class` mechanics — constructors, `private`,
`const` methods, RAII — are [stage 07](../07-classes-and-oop/); here we stay with plain-data
structs. Python has no such split (everything is a class; "private" is only a `_name` convention).

---

## 3. Brace / aggregate initialization

A struct with all-public members and no custom constructor is an **aggregate**, and you can fill it
directly from a braced list. Several forms are worth knowing:

```cpp
Stats a{30, 12, 8};      // direct: hp=30, attack=12, defense=8 (by DECLARATION ORDER)
Stats b = {30, 12, 8};   // same, with '='
Stats c{};               // ALL members zero-initialized: 0, 0, 0
```

Two reasons to **prefer braces `{}`** over the older parentheses:

- **They forbid narrowing.** `Stats{3.9}` is a *compile error* (you can't silently chop `3.9` to
  `3`), which catches real bugs. Parentheses would let it slide.
- `{}` also gives you free **zero-initialization** — `Stats c{};` is guaranteed all-zeros, no
  uninitialized garbage.

> **Partial init and a helpful warning.** `Stats{50}` is legal — it sets `hp=50` and zero-fills the
> rest — but with `-Wextra` the compiler warns (`-Wmissing-field-initializers`), because a
> half-filled struct is usually a mistake. So the habit: use `{}` when you want all zeros, and list
> every field otherwise. It's a good example of a warning that's trying to help.

> Python fills a dataclass by argument (positional or keyword); C++17 aggregate init is **positional
> only** — order matters, so keep member order meaningful. (Named/"designated" initializers exist
> but are a C++20 feature, outside this course's C++17 baseline.)

---

## 4. `enum class` — a small set of named options

An `enum class` defines a type whose values are a fixed set of **names**. Use it instead of magic
numbers or bare strings for a closed vocabulary — damage types, game states, directions:

```cpp
enum class DamageType { Physical, Fire, Ice };   // three named values of a NEW type
DamageType t = DamageType::Fire;                  // must qualify with DamageType::
```

It pairs naturally with `switch`, and the compiler will even *warn* if you forget a case:

```cpp
float damage_multiplier(DamageType type) {
    switch (type) {
        case DamageType::Physical: return 1.0f;
        case DamageType::Fire:     return 1.5f;
        case DamageType::Ice:      return 0.75f;
        default:                   return 1.0f;   // safety net if the enum grows
    }
}
```

**Why `enum class` and not a plain `enum`?** Two safety wins the course insists on:

- **Scoped.** The names live *inside* `DamageType`, so `Fire` here can't collide with `Fire` from
  another enum. You always write `DamageType::Fire`. A plain `enum` dumps its names into the
  surrounding scope.
- **Typed.** It will **not** implicitly convert to `int`. `int n = DamageType::Fire;` is an error;
  you must ask on purpose with `static_cast<int>(DamageType::Fire)` (which gives `0, 1, 2, …` by
  declaration order). That stops a whole category of "I accidentally did arithmetic on an enum"
  bugs.

Python's closest analogue is `enum.Enum`. `enum class` is the course default from here on.

---

## 5. `std::string` — C++'s text type

`std::string` behaves a lot like Python's `str`, with a few twists:

```cpp
#include <string>
std::string hero  = "Aria";
std::string full  = hero + " the Bold";   // '+' concatenates (like Python)
full.size();                              // 13  (Python: len(full))
full[0];                                  // 'A' — a single CHAR, not a 1-char string
full.substr(0, 4);                        // "Aria" — start at index 0, take 4 chars
(hero == "Aria");                         // true — '==' compares by VALUE
for (char c : full) { /* each character */ }
```

| Python `str` | C++ `std::string` |
|--------------|-------------------|
| `a + b` | `a + b` |
| `len(s)` | `s.size()` |
| `s[0]` → a 1-char **str** | `s[0]` → a **`char`** (single quotes: `'A'`) |
| `s[5:8]` | `s.substr(5, 3)` (start, **count**) |
| `s == "x"` | `s == "x"` (value comparison) |
| immutable | **mutable** — you can change characters in place |

Key differences to keep straight:

- **A `char` is not a string.** `'A'` (single quotes) is one character; `"A"` (double quotes) is a
  string. `full[0]` gives you a `char`.
- `std::string` is **mutable** — unlike Python's immutable `str`, you can assign `full[0] = 'a';`.
- `substr` takes a **start and a count**, not a Python-style `[start:stop]` slice.
- A `std::string` owns a heap buffer for its characters, so it's a "big" type — this is exactly the
  kind of value you pass by **`const&`** (the habit from [stage 02](../02-functions-and-memory/))
  rather than copying on every call.

---

## Compiling and running

One file per program, built by hand:

```bash
# an exercise
clang++ -std=c++17 -Wall -Wextra exercises/03_enum_class.solution.cpp -o ex && ./ex

# the mini-project
clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o combat
./combat
echo $?        # -> 0
```

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference. Ordered easy → hard.

1. **`01_first_struct`** *(easy)* — define a `Monster` struct, brace-init it, read/write members
   with the dot.
2. **`02_brace_init`** *(easy–medium)* — the initialization forms: full, the `=` form, and `{}`
   zero-init (plus a note on why `-Wextra` flags partial init).
3. **`03_enum_class`** *(medium)* — an `enum class` + a `switch` that names each value, plus
   `static_cast<int>` to see the underlying number.
4. **`04_strings`** *(medium)* — `std::string` basics: concatenation, `size`, indexing, `substr`,
   and a range-`for` over characters.
5. **`05_combat_summary`** *(medium–hard)* — combine `Stats`, `enum class DamageType`, a
   `damage_multiplier` `switch`, and `static_cast` to compute and print a damage line. A rehearsal
   for the mini-project.

Try each yourself before peeking at the solution.

---

## Mini-project — Combat summary

In [`project/src/main.cpp`](project/src/main.cpp). A hero equips an item and lands a typed attack on
an enemy; the program prints a formatted combat report. It defines the course's **shared data
contracts** — reused unchanged by later stages (inventory in 09, save/load in 10, the capstone in
27):

```cpp
struct Stats { int hp, attack, defense; };   // three ints, in this order
struct Item  { std::string name; int value; };
enum class DamageType { Physical, Fire, Ice };
float damage_multiplier(DamageType);          // switch: Physical 1.0, Fire 1.5, Ice 0.75
```

It **exercises every concept from this stage**: two `struct`s (one with a `std::string` member),
brace initialization, an `enum class` driving a `switch`, and `std::string` building the report.
Damage reuses `static_cast` from stage 01 (`float` multiplier → whole number). Output:

```
------------------------------
        COMBAT SUMMARY
------------------------------
Attacker : Aria the Bold
Weapon   : Ember Blade (worth 120 gold)
Attack   : Fire (x1.5)
------------------------------
Hero  attack : 14
Enemy defense: 6
Damage dealt : 12
Enemy HP     : 40 -> 28
------------------------------
The enemy staggers but fights on.
```

This closes Phase A: you can now model game data with your own types. [Stage 04](../04-headers-and-multi-file/)
takes this same data and code and splits it across `.hpp` / `.cpp` files so a program can grow
beyond one translation unit.

---

## Checklist before moving on

- [ ] I can define a `struct`, brace-initialize it, and access members with `.` (and I remember the
      trailing `;`).
- [ ] I can state the one real difference between `struct` and `class`, and when I'd reach for each.
- [ ] I can predict what `Stats{}` and `Stats{50}` produce, and why braces beat parentheses.
- [ ] I can declare an `enum class`, `switch` over it, and explain "scoped" and "typed."
- [ ] I know why `enum class` values don't turn into `int` on their own, and how to convert one.
- [ ] I can concatenate, index (`char`!), and `substr` a `std::string`, and I know it's mutable and
      worth passing by `const&`.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md).

Next up: [Stage 04 — Headers & multi-file compilation](../04-headers-and-multi-file/).
