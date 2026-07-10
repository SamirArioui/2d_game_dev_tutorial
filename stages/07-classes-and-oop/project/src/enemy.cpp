// YOUR TASK — implement the Enemy methods declared in enemy.hpp.
// A complete reference is in ../solution/src/enemy.cpp.
#include "enemy.hpp"

#include <algorithm>   // std::max
#include <iostream>
#include <utility>     // std::move

// The constructor is given: its member-initializer list constructs the base Character subobject
// FIRST (forwarding the shared fields), then initialises the derived member rage_below_. The base
// subobject is always constructed before derived members.
Enemy::Enemy(std::string name, int hp, int attack, int defense, int rage_below)
    : Character(std::move(name), hp, attack, defense), rage_below_(rage_below) {}

void Enemy::take_turn(Character& target) {
    // TODO(stage 07): override the base behaviour. Start with power = attack_. If hp_ is below
    // rage_below_, print "<name_> is enraged!\n" and boost power by 50% (attack_ + attack_ / 2).
    // Then damage = std::max(1, power - target.defense()); print
    // "<name_> strikes <target name> for <dmg> damage.\n"; and target.take_damage(dmg).
}
