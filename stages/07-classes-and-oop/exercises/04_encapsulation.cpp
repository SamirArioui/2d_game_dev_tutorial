// Exercise 04 — Encapsulation & invariants (medium)
//
// Making data `private` isn't about secrecy — it's about INVARIANTS: rules that must always
// hold. If hp_ were public, any code could set it to -999. If it's private and only changeable
// through take_damage()/heal(), those methods can GUARANTEE that 0 <= hp_ <= max_hp_ always.
//
// PYTHON NOTE: Python leans on `@property` and leading-underscore *convention*; nothing stops a
// caller writing obj._hp = -999. C++ `private` is enforced by the compiler.
//
// TODO:
//   1. Implement heal(amount): raise hp_, but never above max_hp_.
//   2. Implement take_damage(amount): lower hp_, but never below 0.
//   3. In main, over-heal and over-damage a Character and show hp stays within [0, max_hp_].
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_encapsulation.cpp -o ex && ./ex

#include <algorithm>   // std::min, std::max
#include <iostream>

class Character {
public:
    Character(int hp, int max_hp) : hp_(hp), max_hp_(max_hp) {}

    int hp() const { return hp_; }

    void heal(int amount) {
        // TODO: hp_ += amount, but clamp to at most max_hp_  (hint: std::min)
    }

    void take_damage(int amount) {
        // TODO: hp_ -= amount, but clamp to at least 0        (hint: std::max)
    }

private:
    int hp_;
    int max_hp_;
};

int main() {
    Character hero{20, 30};
    // TODO: hero.heal(1000);        print hp (should cap at 30)
    // TODO: hero.take_damage(1000); print hp (should floor at 0)
    return 0;
}
