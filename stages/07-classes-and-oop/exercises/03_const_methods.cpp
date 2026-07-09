// Exercise 03 — const member functions (medium)
//
// Marking a method `const` (e.g. `int hp() const`) is a PROMISE that calling it won't change the
// object. The compiler enforces the promise: inside a const method you can't modify members or
// call non-const methods. And you may only call const methods on a `const` object or a `const&`.
//
// This matters because we pass big objects around as `const&` (stage 02) to avoid copies — and
// on a `const&` you can ONLY call const methods. Forget the `const` on your getters and they
// become unusable in exactly the read-only contexts they're meant for.
//
// PYTHON NOTE: Python has no equivalent. Any method can mutate `self`; "read-only" is a matter
// of discipline, not something the language checks.
//
// TODO:
//   1. Add `const` to the two getters (name, hp).
//   2. Leave take_damage NON-const (it mutates hp_).
//   3. Fill in `describe`, which takes a `const Character&` and prints its getters — this only
//      compiles once the getters are const.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_const_methods.cpp -o ex && ./ex

#include <iostream>
#include <string>
#include <utility>

class Character {
public:
    Character(std::string name, int hp) : name_(std::move(name)), hp_(hp) {}

    // TODO: add `const` to these two getters.
    const std::string& name() { return name_; }
    int hp() { return hp_; }

    void take_damage(int amount) { hp_ -= amount; }   // stays non-const: it mutates

private:
    std::string name_;
    int hp_;
};

// Takes the character by const reference: no copy, plus a promise not to modify it.
void describe(const Character& c) {
    // TODO: print c.name() and c.hp(). This FAILS to compile until the getters are const.
}

int main() {
    Character hero{"Aria", 30};
    hero.take_damage(5);
    describe(hero);
    return 0;
}
