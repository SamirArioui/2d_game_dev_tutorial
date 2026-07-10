# Stage 09 — STL, lambdas & `std::function`

**Goal:** stop hand-rolling containers and start using the **Standard Template Library** — the
part of C++ that gives you Python's `list` and `dict` (and a lot more). By the end you'll pick the
right container, iterate it with range-`for`, sort and filter it with `<algorithm>`, and write
functions inline as **lambdas** — even storing one in a variable as a `std::function`.

**New concepts:** `std::vector` · `std::array` · `std::unordered_map` **vs** `std::map` ·
`std::string` as a map key · iterators & range-`for` · `<algorithm>` (`std::sort`, `std::count_if`,
`std::copy_if`) · lambdas & closures · a stored `std::function`.

**Python throughline:** `list` → `std::vector`, `dict` → `std::unordered_map`, comprehensions and
`sorted(key=...)` → `<algorithm>` + lambdas, first-class functions → `std::function`.

---

## 1. Why the STL exists (and why it looks the way it does)

In Python, `list` and `dict` are *built into the language*. In C++ they are **library** types,
shipped in headers you `#include`. That's the STL: a set of **containers** (vector, map, …),
**iterators** (a uniform way to walk any container), and **algorithms** (sort, find, count — that
work on *any* container through its iterators).

The payoff of that design: `std::sort` doesn't know or care whether it's sorting a `vector` or an
`array`. It only speaks "iterators". Learn iterators once and every algorithm works everywhere.

```
 containers  ──provide──▶  iterators  ──consumed by──▶  algorithms
 vector, array,           begin()/end()               sort, find, count_if,
 unordered_map, map       (a [begin, end) range)       copy_if, max_element, ...
```

> **Mental model for a Python dev:** the STL is Python's built-in `list`/`dict`/`sorted`/`filter`,
> but unbundled into pieces you assemble — and with the element type fixed at compile time.

---

## 2. `std::vector` — your default container (Python's `list`)

`std::vector<T>` is a **dynamic array**: contiguous in memory, grows on demand, knows its `.size()`.
It is the container you'll reach for 90% of the time.

```cpp
std::vector<std::string> party;   // like Python:  party = []
party.push_back("Aria");          //               party.append("Aria")
party.push_back("Borin");
std::cout << party.size();        //               len(party)
std::cout << party[0];            //               party[0]   (no bounds check!)
std::cout << party.back();        //               party[-1]
```

Key differences from a Python `list`:

| Python `list` | `std::vector<T>` |
|---|---|
| holds anything, mixed types | holds exactly one type `T`, checked at compile time |
| `xs.append(x)` | `xs.push_back(x)` |
| `xs[i]` raises `IndexError` if out of range | `xs[i]` is **undefined behaviour** out of range; `xs.at(i)` throws |
| `len(xs)` | `xs.size()` (an unsigned `size_t`) |
| grows invisibly | grows by reallocating; `xs.reserve(n)` pre-allocates to avoid churn |

---

## 3. `std::array` — fixed size, on the stack

When the length never changes, `std::array<T, N>` bakes the size `N` into the type. No heap
allocation, zero overhead over a C array, but with `.size()` and range-`for`.

```cpp
std::array<int, 6> rolls = {1, 2, 3, 4, 5, 6};   // exactly six ints, on the stack
```

Python has no direct equivalent — a tuple is the nearest idea, but tuples can mix types. Use
`std::array` for the die faces, the RGBA channels of a colour, the corners of a rectangle: things
whose count is genuinely fixed.

---

## 4. `std::unordered_map` vs `std::map` (this is the important one)

A Python `dict` is a **hash map**. The direct C++ equivalent is **`std::unordered_map`**, *not*
`std::map`. This trips up everyone coming from Python, so it's worth being precise:

| | `std::unordered_map<K,V>` | `std::map<K,V>` |
|---|---|---|
| implementation | **hash table** | balanced binary **tree** |
| lookup / insert | **O(1)** average | O(log n) |
| iteration order | **unspecified** | **sorted by key**, always |
| Python analog | **`dict`** ✅ | a `dict` you always iterate via `sorted(d)` |
| header | `<unordered_map>` | `<map>` |

**Rule of thumb:** reach for `std::unordered_map` — it's the `dict`. Choose `std::map` only when you
specifically want the keys visited in sorted order (e.g. printing a leaderboard alphabetically).

Both are *used* like a dict:

```cpp
std::unordered_map<std::string, int> counts;
counts["arrow"]++;                 // operator[] inserts 0 for a new key, then increments
counts.at("arrow");                // like d["arrow"] — throws if missing (bounds-checked)
counts.count("arrow");             // 1 if present, 0 if not  (like  "arrow" in counts)
counts.find("arrow");              // iterator to the pair, or counts.end() if absent

for (const auto& [name, n] : counts) {   // C++17 structured bindings == Python's .items()
    std::cout << name << ": " << n << "\n";
}
```

> **Gotcha:** `counts["ghost"]` *inserts* `"ghost"` with value `0` if it wasn't there — reading a
> missing key mutates the map. To check without inserting, use `.count()` or `.find()`. Python's
> `d["ghost"]` raises instead; `d.get("ghost", 0)` is the closer match to `operator[]`.

`std::string` works as a key in either map (it hashes and compares out of the box) and is itself a
first-class STL type you can pass to `<algorithm>`.

---

## 5. Iterators & range-`for`

An **iterator** is a generalized pointer: it marks a position in a container. Every container hands
you `begin()` (first element) and `end()` (one-past-the-last). Algorithms take a **`[begin, end)`
range** — half-open, like Python's `range(begin, end)`.

You rarely write raw iterator loops. Prefer **range-`for`**, which is exactly Python's
`for x in xs`:

```cpp
for (const Item& it : items) { ... }   // Python:  for it in items:
```

Use `const T&` to visit read-only without copying each element; drop the `const` (`T&`) when you
need to modify elements in place.

---

## 6. `<algorithm>` — the batteries

`#include <algorithm>` gives you the equivalents of Python's `sorted`, `filter`, `any`, `max`, …
They operate on iterator ranges:

```cpp
std::sort(v.begin(), v.end());                       // sorted(v) in place
std::sort(v.begin(), v.end(), comparator);           // sorted(v, key=...) — but a comparator
int n = std::count_if(v.begin(), v.end(), pred);     // sum(1 for x in v if pred(x))
std::copy_if(v.begin(), v.end(), out, pred);         // [x for x in v if pred(x)]
auto it = std::max_element(v.begin(), v.end());      // max(v) — returns an iterator; * to read
```

The comparator/predicate is where **lambdas** come in.

---

## 7. Lambdas (Python's `lambda`, but bigger)

A **lambda** is an anonymous function written inline. Anatomy:

```cpp
[capture](parameters) { body }

[](const Item& a, const Item& b) { return a.value < b.value; }
//^         ^                        ^
//capture   parameters               body (return type deduced)
```

- **`std::sort` wants a comparator**, not a key function. It answers one question — *"should `a`
  come before `b`?"* — so `return a.value < b.value` sorts ascending. (Python's `sorted(key=...)`
  is a convenience layer; C++ hands you the more general comparator.)
- The **capture list `[...]`** is the closure — it decides what surrounding variables the lambda
  can see:

```cpp
int min_value = 50;
auto pred = [min_value](const Item& i) { return i.value >= min_value; };
//           ^^^^^^^^^ captured BY COPY, so it stays valid even after min_value goes out of scope
```

`[x]` captures `x` by copy, `[&x]` by reference, `[=]`/`[&]` capture everything used by copy/ref.
Prefer capturing by copy for small values (like Python closures capturing by value semantics for
immutables).

---

## 8. Storing a function: `std::function`

Every lambda has its own unique, unnameable type. That's fine when you pass it straight to
`std::sort`. But to **store** a callable in a variable, a struct field, or a function parameter, wrap
it in **`std::function<Signature>`**:

```cpp
#include <functional>
std::function<bool(const Item&)> keep = [](const Item& i) { return i.value >= 20; };
//              ^^^^^^^^^^^^^^^^ "any callable taking const Item& and returning bool"
```

This is how C++ treats functions as first-class values — the thing Python gives you for free when
you write `f = lambda i: i.value >= 20` and pass `f` around. The mini-project stores exactly this
kind of predicate so the *rule* for filtering can be chosen at runtime.

> `std::function` has a small runtime cost (it may allocate and calls through a pointer). When you
> can, pass callables as a template parameter or `auto` and reserve `std::function` for when you
> genuinely need to *store* one. For a course-scale inventory that cost is irrelevant.

---

## Building and running

This stage uses **CMake** (introduced in stage 05). From this folder:

```bash
cmake -S . -B build          # configure into ./build
cmake --build build          # compile the project + every exercise solution
./build/stage09_inventory    # run the mini-project
```

Each exercise solution also builds as its own target, named `s09_<name>`:

```bash
./build/s09_04_sort_lambda
```

Or compile a single exercise by hand, the stage 00 way:

```bash
clang++ -std=c++17 -Wall -Wextra exercises/04_sort_lambda.solution.cpp -o ex && ./ex
```

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference solution. Ordered easy → hard.

1. **`01_vector_basics`** *(easy)* — `push_back`, `.size()`, `.back()`, range-`for`. The `list`
   analog.
2. **`02_std_array`** *(easy–medium)* — a fixed-size `std::array`, summed with range-`for` and
   `std::max_element` (your first algorithm).
3. **`03_map_vs_unordered_map`** *(medium)* — build item counts in an `unordered_map`, then feel the
   difference by re-sorting them through a `std::map`. The key mental model of the stage.
4. **`04_sort_lambda`** *(medium)* — sort a `std::vector<Item>` by value ascending **and** descending
   with a lambda comparator.
5. **`05_function_filter`** *(medium–hard)* — store a `std::function<bool(const Item&)>` predicate,
   apply it with `std::count_if`/`std::copy_if`, and capture a threshold in a closure.

Try each yourself before peeking at the solution.

---

## Mini-project — Inventory (keyed, sorted, filtered)

**Your task.** [`project/src/inventory.cpp`](project/src/inventory.cpp) is a **starter** with `TODO`s
— you implement the `Inventory` class declared in
[`project/src/inventory.hpp`](project/src/inventory.hpp), combining **every** concept above:

- distinct items in a **`std::vector<Item>`** (`struct Item { std::string name; int value; };`),
- quantities in a **`std::unordered_map<std::string,int>`** keyed by name — the `dict` analog, used
  for O(1) `count_of` and `total_value`,
- **`sorted_by_value(descending)`** returning a copy sorted with a **lambda comparator**,
- **`matching(pred)`** taking a **stored `std::function<bool(const Item&)>`** so the filter rule is
  data, not hard-coded.

The demo in `main.cpp` builds an inventory (note "Arrow" added twice — one entry, climbing count),
prints the keyed totals, sorts by value, then filters to "items worth ≥ 50". Once your
implementation is complete it prints exactly:

```
=== Inventory ===
  Health Potion x3 (value 25 each)
  Iron Sword x1 (value 120 each)
  Arrow x60 (value 5 each)
  Shield x1 (value 60 each)
total value (value * quantity): 555

=== Sorted by value (descending) ===
  Iron Sword (value 120)
  Shield (value 60)
  Health Potion (value 25)
  Arrow (value 5)

=== Items worth >= 50 ===
  Iron Sword (value 120)
  Shield (value 60)
```

This `Item`/inventory shape is a **course contract**: stage 10 saves and loads exactly this
inventory (plus a `Stats` block) to a file, and the capstone (stage 27) revives it.

```bash
cmake -S . -B build && cmake --build build
./build/stage09_inventory
```

Stuck? A complete reference is in [`project/solution/`](project/solution/) — build it standalone and
compare, but try it yourself first:

```bash
cmake -S project/solution -B build-solution && cmake --build build-solution
./build-solution/stage09_inventory
```

---

## Checklist before moving on

- [ ] I know `std::vector` is the `list` analog and `std::unordered_map` — **not** `std::map` — is
      the `dict` analog, and I can say why.
- [ ] I can explain when I'd pick `std::map` (sorted iteration) or `std::array` (fixed size).
- [ ] I can iterate a map with `for (const auto& [k, v] : m)` and know `operator[]` inserts on read.
- [ ] I can sort a container with `std::sort` + a lambda comparator (`a < b` for ascending).
- [ ] I can write a lambda, capture a variable in its closure, and store one in a `std::function`.
- [ ] I implemented the inventory mini-project (`project/src/inventory.cpp`) and it prints the
      keyed/sorted/filtered output.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference's container and algorithm
pages are the canonical reference; skim the "Containers library" overview.

Next up: [Stage 10 — File I/O & error handling](../10-file-io-and-error-handling/).
