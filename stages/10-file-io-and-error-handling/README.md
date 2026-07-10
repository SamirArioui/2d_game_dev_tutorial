# Stage 10 — File I/O & error handling

**Goal:** read and write files, parse the text you read into typed data, and handle the thing files
force you to confront — **failure**. A file might not exist, might be locked, might be garbage. By
the end you'll write a loader that returns `std::optional<SaveData>`: a real value when the save is
good, and *nothing* (gracefully) when it isn't.

**New concepts:** `std::ofstream` / `std::ifstream` / `std::fstream` · `std::getline` · parsing with
`std::stringstream` · RAII auto-close (the stream closes in its destructor) · the three error styles
— **return codes vs exceptions vs `std::optional`** · why `std::optional` is C++'s `None`.

**Python throughline:** `with open(path) as f:` → an `ifstream`/`ofstream` that auto-closes;
`for line in f:` → `while (getline(...))`; `line.split()` → `std::stringstream`; returning `None`
for "not found" → returning an empty `std::optional`.

---

## 1. Streams: the same `<<`/`>>` you already know, aimed at a file

You met `std::cout` (output) and streams in stage 00. A file stream is the same idea pointed at a
file instead of the terminal:

| Direction | Type | Header | Python |
|---|---|---|---|
| write | `std::ofstream` | `<fstream>` | `open(path, "w")` |
| read | `std::ifstream` | `<fstream>` | `open(path, "r")` |
| both | `std::fstream` | `<fstream>` | `open(path, "r+")` |

```cpp
std::ofstream out("hero.txt");   // open for writing (creates/truncates)
out << "Aria\n" << 30 << "\n";   // exactly like std::cout, but into the file

std::ifstream in("hero.txt");    // open for reading
std::string line;
while (std::getline(in, line)) { // read a line at a time; false at end-of-file
    std::cout << line << "\n";
}
```

`std::getline(stream, line)` reads one line (dropping the `\n`) into a `std::string` and returns the
stream, which is **truthy while a read succeeded** and **falsy at EOF**. That's why the read loop is
`while (std::getline(in, line))` — the direct analog of Python's `for line in f:`.

---

## 2. RAII: the file closes itself (tie back to stage 07)

In Python you either call `f.close()` by hand or, better, use a `with` block so the file closes when
the block ends. C++ has something stronger and more automatic: **RAII** (Resource Acquisition Is
Initialization), which you first met with class destructors in stage 07.

The file handle is a resource *owned by the stream object*. Opening happens in the stream's
constructor; **closing happens in its destructor**, which runs automatically when the stream goes
out of scope. You don't call `.close()` — you let scope do it:

```cpp
{
    std::ofstream out("hero.txt");
    out << "Aria\n";
}   // <-- out's destructor runs HERE: it flushes buffered data and closes the file
```

```
Python                         C++ (RAII)
------                         ----------
with open("h.txt","w") as f:   {  std::ofstream out("h.txt");
    f.write("Aria\n")             out << "Aria\n";
# f closed leaving `with`      }  // out closed leaving the scope
```

The `with` block is explicit; RAII is implicit and applies to *every* resource type (files, memory,
sockets, textures) — the same mechanism that will manage smart pointers in stage 11 and engine
resources later. Open a stream, use it, let it fall out of scope. Done.

> **Always check the stream opened.** `if (!out) { ... }` is true when the open failed. A stream
> that failed to open won't throw by default — it silently does nothing — so an unchecked open is a
> classic "why is my file empty?" bug.

---

## 3. Parsing a line with `std::stringstream`

Files hand you a `std::string` per line; you almost always need to split it into typed **fields**.
`std::stringstream` wraps a string and gives you the same `>>` and `getline` you use on files:

```cpp
#include <sstream>

std::string line = "30 12 8";
std::stringstream ss(line);
int hp, attack, defense;
ss >> hp >> attack >> defense;    // whitespace-separated, converted BY TYPE
```

`>>` skips leading whitespace and converts to the target type — it's Python's `line.split()` **and**
`int(...)` in one step. For fields separated by a specific delimiter (CSV, or names with spaces),
use the three-argument `getline`:

```cpp
std::string field;
std::stringstream ss("Health Potion|25|3");
std::getline(ss, field, '|');     // "Health Potion"  (keeps the internal space)
std::getline(ss, field, '|');     // "25"
std::getline(ss, field);          // "3"  (no delimiter: takes the rest)
```

The mini-project uses `|` delimiters precisely so item names may contain spaces.

---

## 4. Error handling: return codes vs exceptions vs `std::optional`

Files fail. C++ gives you three ways to report "this operation might not succeed". Knowing *which*
to use is a real engineering judgement, so here's the map:

| Style | Looks like | Use when | Python echo |
|---|---|---|---|
| **Return code / status** | `bool ok = save(...);` | simple, local, caller must act now | returning `True`/`False` |
| **Exception** | `throw std::runtime_error{...}` / `try`/`catch` | truly *exceptional*, rare, can't handle locally | `raise` / `try/except` |
| **`std::optional<T>`** | `std::optional<T> load(...)` | result may be **legitimately absent** | returning `None` |

### Why `std::optional` for "might not be there"

`std::optional<T>` (from `<optional>`, C++17) holds *either* a `T` *or* nothing. It's the honest
return type for a load that may find no save file — absence is **expected**, not exceptional, so an
exception would be overkill and a magic sentinel value (`-1`, empty struct) would be a lie.

```cpp
std::optional<int> parse_int(const std::string& s);   // Python: def parse_int(s) -> int | None

std::optional<int> o = parse_int("42");
o.has_value();      // Python: o is not None
*o;  o.value();     // read it (UB / throws if empty — check first, or use value_or)
o.value_or(0);      // the value, or 0 if empty  (Python: o if o is not None else 0)

std::optional<int> none = std::nullopt;   // explicitly empty  (Python: None)
```

An optional is itself **truthy when it holds a value**, so `if (o) { use(*o); }` reads naturally.

> The three styles aren't rivals — you combine them. The mini-project's `load()` returns
> `std::optional<SaveData>` (absent = "no save yet"), but *internally* it catches the exception
> `std::stoi` throws on non-numeric text and converts it into the same empty-optional result, so the
> caller has **one** failure channel to check instead of three.

---

## Building and running

This stage uses **CMake**. From this folder:

```bash
cmake -S . -B build          # configure into ./build
cmake --build build          # compile the project + every exercise solution
./build/stage10_save_load    # run the mini-project
```

Exercise solutions build as `s10_<name>` (e.g. `./build/s10_05_round_trip`). The programs write
their output files into the *current directory*, so run them from wherever you don't mind a
`hero.txt` / `savegame.txt` appearing. Or compile one by hand:

```bash
clang++ -std=c++17 -Wall -Wextra exercises/05_round_trip.solution.cpp -o ex && ./ex
```

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference solution. Ordered easy → hard.

1. **`01_write_lines`** *(easy)* — write with `std::ofstream`, check `if (!out)`, and watch the file
   close via RAII at the end of a scope block.
2. **`02_read_lines`** *(easy–medium)* — the `while (std::getline(in, line))` read loop.
3. **`03_stringstream_parse`** *(medium)* — split a line into typed fields two ways: `>>` for
   whitespace-separated ints, delimiter-`getline` for `|`-separated fields (names with spaces).
4. **`04_optional`** *(medium)* — write `parse_int` returning `std::optional<int>`; use
   `has_value`/`value`/`value_or`; read the comment block contrasting the three error styles.
5. **`05_round_trip`** *(medium–hard)* — **the key exercise**: save a `Stats` block, load it back,
   assert the round-trip matches, and confirm that loading a **missing file yields an empty
   optional**. (Its exit code is `0` only when the round-trip succeeds.)

Try each yourself before peeking at the solution.

---

## Mini-project — Save / load the inventory + stats

**Your task.** [`project/src/savegame.cpp`](project/src/savegame.cpp) is a **starter** with `TODO`s
— you implement `save()` and `load()` (declared in
[`project/src/savegame.hpp`](project/src/savegame.hpp)) to persist the **stage-9 inventory** plus a
`Stats` block to a line-based text file and load it back, exercising every concept of the stage.

Shared course contracts used here:

```cpp
struct Item  { std::string name; int value; };
struct Stats { int hp, attack, defense; };

struct SaveData {                                  // a Stats + an inventory
    Stats stats;
    std::vector<Item> items;                       // distinct items (stage-9 shape)
    std::unordered_map<std::string, int> counts;   // name -> quantity
};

std::optional<SaveData> load(const std::string& path);   // EMPTY on missing/corrupt
```

File format (one record per line, `|`-delimited):

```
STATS|30|12|8
ITEM|Health Potion|25|3
ITEM|Iron Sword|120|1
ITEM|Arrow|5|60
```

`load()` returns an **empty optional** if the file is missing *or* any line is malformed (wrong
tag, missing field, non-numeric number) — never a half-filled struct. Once your implementation is
complete, the demo in `main.cpp` proves both the round-trip and the empty-optional path:

```
=== Saving ===
  Stats: hp=30 attack=12 defense=8
  Item: Health Potion x3 (value 25)
  Item: Iron Sword x1 (value 120)
  Item: Arrow x60 (value 5)

=== Loading savegame.txt ===
  Stats: hp=30 attack=12 defense=8
  Item: Health Potion x3 (value 25)
  Item: Iron Sword x1 (value 120)
  Item: Arrow x60 (value 5)
round-trip matches original? yes

=== Loading a missing file ===
no save found -> starting a fresh game (empty optional handled)
```

```bash
cmake -S . -B build && cmake --build build
cd build && ./stage10_save_load     # run from build/ so savegame.txt lands there
```

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone and
compare, but try it yourself first:

```bash
cmake -S project/solution -B build-solution && cmake --build build-solution
cd build-solution && ./stage10_save_load
```

This `SaveData` + `load()` shape is a **course contract**: the capstone (stage 27) reuses this exact
save/load design.

---

## Checklist before moving on

- [ ] I can open a file with `ofstream`/`ifstream`, and I always check `if (!stream)` after opening.
- [ ] I can explain how RAII closes the file for me, and how that mirrors Python's `with`.
- [ ] I can read a file with `while (std::getline(in, line))` and split a line with `std::stringstream`.
- [ ] I can name the three error styles and say when each fits.
- [ ] I know `std::optional<T>` is C++'s `None`, and can use `has_value` / `*` / `value_or`.
- [ ] I implemented `save()`/`load()` so `load()` returns an empty optional on a missing **and** a
      corrupt file, and I proved the round-trip.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference on `<fstream>`,
`std::optional`, and `std::stringstream`.

Next up: [Stage 11 — Smart pointers, move & the rule of 0/3/5](../11-smart-pointers-move-rule-of-five/).
