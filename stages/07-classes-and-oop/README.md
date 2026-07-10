# Stage 07 — Classes & OOP

**Goal:** turn the plain `struct`s of stage 03 into real **classes** — data plus the functions
that guard it — and meet the object-oriented machinery C++ gives you: constructors and
destructors, the member-initializer list, encapsulation, `const` member functions, inheritance
with `virtual` dispatch, and the single most important idiom in C++: **RAII**.

**New concepts:** constructors & destructors · the member-initializer list · encapsulation
(private/protected data behind getters) · `const` member functions · inheritance & `virtual`
(polymorphism, `override`, virtual destructors) · **RAII** (destructors that release resources
automatically).

> You already know classes from Python. This stage is mostly about the things Python *doesn't*
> have: compiler-enforced access control, a separate "construct directly vs. assign" step,
> `const` methods, opt-in virtual dispatch, and **deterministic destructors** — which are what
> make RAII work and what start replacing the manual `delete` you did in stage 06.

---

## 1. A class is a struct that protects its own rules

In stage 03 a `struct` was a bag of public fields. A **class** bundles that data with the methods
that operate on it, and — crucially — controls who may touch the data. Here is the same idea in
both languages:

```python
# Python
class Health:
    def __init__(self, hp):     # `self` is explicit
        self.hp = hp            # public attribute; anyone can write target.hp = -999
    def heal(self, amount):
        self.hp += amount
```

```cpp
// C++
class Health {
public:
    explicit Health(int hp) : hp_(hp) {}   // constructor; hp_ set in the init list
    int hp() const { return hp_; }         // a const getter
    void heal(int amount) { hp_ += amount; }
private:
    int hp_;                               // PRIVATE: only Health's methods can touch it
};
```

Two differences you feel immediately:

- **There is no `self`.** Inside a method you name members directly (`hp_`), or write
  `this->hp_` if you need to be explicit. `this` is a *pointer* to the current object.
- **Access is enforced, not suggested.** Members are `private` by default in a `class`
  (`public` in a `struct` — that's the *only* language-level difference between the two
  keywords). Python's leading-underscore `_hp` is a convention the runtime ignores; C++'s
  `private` is checked by the compiler.

---

## 2. Constructors, the member-initializer list, and why it matters

A constructor runs when an object is created. It can set members in two places, and the
difference is real:

```cpp
// (a) member-initializer list — after the ':' , before the body.  PREFERRED.
Character(std::string name, int hp) : name_(std::move(name)), hp_(hp) { }

// (b) assignment in the body.
Character(std::string name, int hp) { name_ = name; hp_ = hp; }
```

Form (a) **constructs each member once, directly with its final value.** Form (b) first
**default-constructs** every member (an empty `std::string`, an uninitialised `int`) and *then*
overwrites it — wasted work for a string, and outright **impossible** for two kinds of member:

- **`const` members** — they can be initialised but never assigned;
- **reference members** (`int& count_`) — they must be bound when the object is born.

So the init list isn't just a style preference; for const/reference members it's the only legal
option. Exercise 02 makes this concrete with a `const int max_hp_` that simply won't compile
unless you put it in the list. Members are initialised in **declaration order**, not the order
you write them in the list — a good reason to keep the two consistent.

> `std::move(name)` hands the string's internals over instead of copying them. You'll learn the
> full story in stage 11 (move semantics); for now, read it as "take this string by value, then
> move it into the member — the idiomatic, copy-free way to accept a string parameter."

---

## 3. `const` member functions

A method marked `const` promises **"calling me does not change the object."** The compiler
enforces it: inside a `const` method you can't modify members or call non-const methods.

```cpp
int hp() const { return hp_; }        // const: safe to call on a read-only object
void take_damage(int a) { hp_ -= a; } // non-const: it mutates, so no `const`
```

Why care? Because this course passes big objects around by `const&` (stage 02) to avoid copies —
and **on a `const&` you can only call `const` methods.** A getter that forgets its `const` is
unusable in exactly the read-only, no-copy contexts it exists for:

```cpp
void describe(const Character& c) {
    std::cout << c.name() << " " << c.hp();  // compiles ONLY if name()/hp() are const
}
```

Python has no equivalent — any method may mutate `self`, and "read-only" is a matter of
discipline. In C++ it's part of the type. This is the `const`-correctness thread from stages
01–02 reaching methods; it continues into `Vec2`/physics APIs in stage 08+.

---

## 4. Encapsulation is about invariants, not secrecy

Making `hp_` private isn't about hiding it — it's about guaranteeing a **rule** (an *invariant*)
that must always hold. If `hp_` is public, any line anywhere can set it to `-999`. If it's
private and only changed through `heal`/`take_damage`, those methods enforce `0 <= hp_ <= max_hp_`
for the whole program:

```cpp
void heal(int amount)        { hp_ = std::min(max_hp_, hp_ + amount); }  // never overheal
void take_damage(int amount) { hp_ = std::max(0, hp_ - amount); }        // never below zero
```

The invariant lives in one place instead of being re-checked (and forgotten) at every call site.
That's the real argument for `private`, and it's why C++'s enforced access control is worth the
extra keyword over Python's underscore convention.

---

## 5. Inheritance and `virtual`: opt-in polymorphism

Inheritance lets a derived class reuse and specialise a base. `Enemy : public Character` means
"an `Enemy` *is a* `Character`" and can be used wherever a `Character` is expected.

The interesting part is **`virtual`**. Calling a virtual method through a base pointer/reference
runs the version matching the object's **real** type, chosen at run time — *dynamic dispatch*:

```cpp
Enemy goblin{"Goblin"};
Character& ref = goblin;   // base reference to a derived object
ref.speak();               // runs Enemy::speak(), because speak() is virtual
```

Three things C++ needs that Python doesn't:

1. **`virtual` on the base method.** In Python every method is virtual automatically; in C++ you
   opt in. A non-virtual call is resolved from the *static* type (here it would print the
   `Character` version).
2. **`override` on the derived method.** Not required, but it makes the compiler verify you
   actually overrode something — misspell the signature and you get an error instead of a silent
   second method.
3. **A `virtual` destructor on the base.** If you'll ever `delete` a derived object through a
   base pointer (stage 11 does exactly this with `unique_ptr<Character>`), the base destructor
   must be `virtual` so the derived one runs too. Forgetting this leaks the derived parts.

| Python | C++ |
|--------|-----|
| every method overridable by default | mark base method `virtual` |
| override by just redefining | redefine + `override` (checked) |
| `super().__init__(...)` | base constructor in the init list: `Enemy(...) : Character(...)` |
| `__del__` (runs eventually, if ever) | `~Character()` (runs deterministically); make it `virtual` in a base |

---

## 6. RAII — the idea this whole course leans on

**RAII** stands for *Resource Acquisition Is Initialization*. The pattern: **acquire a resource
in the constructor, release it in the destructor.** Because a C++ destructor runs
*deterministically* the moment an object leaves scope — in reverse order of construction, even if
you `return` early or an exception unwinds the stack — cleanup becomes automatic and impossible to
forget.

```cpp
{
    BattleLog log{"Aria vs Goblin"};   // ctor ACQUIRES: prints the opening banner
    // ... use log ...
}                                       // dtor RELEASES: prints the closing summary — HERE,
                                        // automatically, no matter how we leave the block
```

This is the deterministic cousin of Python's `with open(...) as f:` context manager. But Python's
`__del__` runs whenever the garbage collector gets around to it (maybe never); a C++ destructor
runs at a precise, predictable point. That reliability is why RAII, not manual cleanup, is the C++
way to manage *every* resource:

- the **heap memory** you `delete`d by hand in stage 06 → a destructor frees it (this stage);
- **files** → `std::ifstream` closes itself (stage 10);
- **ownership** → `unique_ptr`/`shared_ptr` `delete` automatically (stage 11);
- **textures and engine resources** later on (stages 25–26).

Exercise 06 and the mini-project both *demonstrate* RAII — a destructor doing visible, real work —
rather than just naming it.

---

## 7. Building and running

This is a **CMake** stage. The mini-project is split across several files (`character`, `enemy`,
`battle_log`, `main`) and linked into one executable — the multi-file pattern from stage 04, now
driven by CMake:

```bash
cmake -S . -B build
cmake --build build

./build/battle              # the turn-based battle

# each exercise solution is a target too:
./build/ex_06_raii
```

> **Compiler note:** if your default compiler can't link this stage, point CMake at a specific
> one, e.g. `cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++`.

---

## Exercises

In [`exercises/`](exercises/). Each `NN_name.cpp` is a starter with `TODO`s; the matching
`NN_name.solution.cpp` is a reference solution. Ordered easy → hard:

1. **`01_first_class`** *(easy)* — a class with a private member, a constructor, a `const` getter,
   and a mutating method. Your first `private:`/`public:` split.
2. **`02_member_init_list`** *(easy–medium)* — use the member-initializer list, including a
   `const` member that *only* the init list can set. (This starter won't compile until you fix
   it — that's the lesson.)
3. **`03_const_methods`** *(medium)* — add `const` to getters so a `const Character&` parameter
   can call them; see the compile error you get if you don't.
4. **`04_encapsulation`** *(medium)* — keep `hp_` private and enforce `0 <= hp_ <= max_hp_`
   entirely inside `heal`/`take_damage`.
5. **`05_inheritance_virtual`** *(medium–hard)* — derive `Enemy` from `Character`, `override` a
   `virtual` method, and watch dynamic dispatch pick the derived version through a base reference.
6. **`06_raii`** *(medium–hard)* — a `LiveCount` whose constructor/destructor bump a shared
   counter, so you can watch objects clean up automatically (and in reverse order) at scope exit.

Try each yourself before peeking at the solution.

---

## Your task — a turn-based battle with a self-closing log

The code in [`project/src/`](project/src/) is a **starter**: the class **declarations** are given
in the `.hpp` headers, but the method bodies (in `character.cpp`, `enemy.cpp`, `battle_log.cpp`)
and the battle loop in `main.cpp` are stubbed with `TODO`s — you implement them. It **combines
every concept in this stage** and builds the `Character`/`Enemy` hierarchy the rest of the course
reuses (stage 11 will own these through `unique_ptr<Character>`):

- **`Character`** — a base class with a constructor + member-init list, `protected` data
  (`name_`, `hp_`, `attack_`, `defense_`), `const` getters, and a `virtual void take_turn(...)`
  plus a `virtual` destructor.
- **`Enemy : public Character`** — overrides `take_turn` with enrage-when-wounded behaviour;
  same call, different result, chosen by dynamic dispatch.
- **`BattleLog`** — the RAII piece: its constructor prints an opening banner, and its
  **destructor does real cleanup** — it prints a closing summary with the number of rounds
  logged, automatically, when `main` returns.

Build & run your version:

```bash
cmake -S . -B build && cmake --build build
./build/battle
```

Expected output once implemented:

```
=== Aria vs Goblin ===
  Round 1
Aria the Bold attacks Goblin Raider for 10 damage.
Goblin Raider strikes Aria the Bold for 6 damage.
  Round 2
Aria the Bold attacks Goblin Raider for 10 damage.
Goblin Raider is enraged!
Goblin Raider strikes Aria the Bold for 11 damage.
  Round 3
Aria the Bold attacks Goblin Raider for 10 damage.

Winner: Aria the Bold (13 hp left)
=== Aria vs Goblin closed (3 entries logged) ===
```

Notice the final line: nothing in `main` calls it. The `BattleLog` destructor printed it on its
own when `log` left scope. That's RAII, demonstrated rather than described.

Stuck? A complete reference is in [`project/solution/`](project/solution/) — it is self-contained
with its own `CMakeLists.txt`. Try it yourself first, then compare:

```bash
cmake -S project/solution -B build-solution && cmake --build build-solution
./build-solution/battle
```

---

## Checklist before moving on

- [ ] I can write a class with `private` data, a constructor using a **member-initializer list**,
      and `const` getters — and explain why the init list beats assigning in the body.
- [ ] I know why a `const` method is required to be callable through a `const&`, and which methods
      should be `const`.
- [ ] I can explain encapsulation in terms of *invariants*, not secrecy.
- [ ] I can derive a class, `override` a `virtual` method, and predict which version runs through
      a base pointer/reference — and I know why a base class needs a `virtual` destructor.
- [ ] I implemented the mini-project starter (`project/src/`), and I can state what RAII is — the
      `BattleLog` destructor I wrote did real cleanup automatically at scope exit.

Further reading: [`../../RESOURCES.md`](../../RESOURCES.md) — cppreference on constructors,
`virtual`, and destructors; the C++ Core Guidelines sections on classes and RAII; and the
"Back to Basics: RAII" CppCon talk.

Next up: [Stage 08 — Operator overloading, templates & namespaces](../08-operators-templates-namespaces/),
where you'll build the `gmath::Vec2<T>` type the graphics and physics stages are built on.
