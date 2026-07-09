// Exercise 04 — Encapsulation & invariants (solution)
#include <algorithm>
#include <iostream>

class Character {
public:
    Character(int hp, int max_hp) : hp_(hp), max_hp_(max_hp) {}

    int hp() const { return hp_; }

    // The invariant 0 <= hp_ <= max_hp_ lives entirely inside these two methods, so it can never
    // be violated from outside — that is the payoff of keeping hp_ private.
    void heal(int amount) { hp_ = std::min(max_hp_, hp_ + amount); }
    void take_damage(int amount) { hp_ = std::max(0, hp_ - amount); }

private:
    int hp_;
    int max_hp_;
};

int main() {
    Character hero{20, 30};

    hero.heal(1000);
    std::cout << "after over-heal:   " << hero.hp() << "\n";   // capped at 30

    hero.take_damage(1000);
    std::cout << "after over-damage: " << hero.hp() << "\n";   // floored at 0
    return 0;
}
