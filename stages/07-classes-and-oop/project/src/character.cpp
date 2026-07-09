#include "character.hpp"

#include <algorithm>   // std::max
#include <iostream>
#include <utility>     // std::move

Character::Character(std::string name, int hp, int attack, int defense)
    : name_(std::move(name)), hp_(hp), attack_(attack), defense_(defense) {
    // The body is empty on purpose: all the real initialization happened in the member-init
    // list above, which constructs each member exactly once with its final value.
}

void Character::take_damage(int amount) {
    // Never let hp drop below 0. Keeping this rule inside the class (rather than trusting every
    // caller to remember it) is the whole point of encapsulation.
    hp_ = std::max(0, hp_ - amount);
}

void Character::take_turn(Character& target) {
    // Base behaviour: a basic attack. Damage is attack minus the target's defense, floored at
    // 1 so a turn is never completely wasted.
    int dmg = std::max(1, attack_ - target.defense());
    std::cout << name_ << " attacks " << target.name() << " for " << dmg << " damage.\n";
    target.take_damage(dmg);
}
