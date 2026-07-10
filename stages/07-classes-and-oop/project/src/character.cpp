// YOUR TASK — implement the Character methods declared in character.hpp.
// A complete reference is in ../solution/src/character.cpp.
#include "character.hpp"

#include <algorithm>   // std::max
#include <iostream>
#include <utility>     // std::move

// The constructor is given as your worked example of a MEMBER-INITIALIZER LIST: the part after
// the ':' constructs each member directly with its final value (cheaper than assigning in the
// body, and the only option for const/reference members). `name` is a sink parameter, moved in.
Character::Character(std::string name, int hp, int attack, int defense)
    : name_(std::move(name)), hp_(hp), attack_(attack), defense_(defense) {}

void Character::take_damage(int amount) {
    // TODO(stage 07): subtract `amount` from hp_, but never let hp_ drop below 0
    // (std::max(0, hp_ - amount)). Keeping this invariant inside the class is encapsulation.
}

void Character::take_turn(Character& target) {
    // TODO(stage 07): basic attack — damage = std::max(1, attack_ - target.defense()); print
    // "<name_> attacks <target name> for <dmg> damage.\n"; then target.take_damage(dmg).
}
