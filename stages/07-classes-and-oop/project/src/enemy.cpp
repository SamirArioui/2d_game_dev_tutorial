#include "enemy.hpp"

#include <algorithm>   // std::max
#include <iostream>
#include <utility>     // std::move

Enemy::Enemy(std::string name, int hp, int attack, int defense, int rage_below)
    // Construct the base part FIRST (delegating the shared fields to Character's constructor),
    // then initialise our own member. Base subobject always comes before derived members.
    : Character(std::move(name), hp, attack, defense), rage_below_(rage_below) {}

void Enemy::take_turn(Character& target) {
    // Enemy-specific behaviour: when wounded (hp below the rage threshold) it attacks with +50%
    // power. Same call `take_turn`, different behaviour — that is what `virtual` buys you.
    int power = attack_;
    if (hp_ < rage_below_) {
        power = attack_ + attack_ / 2;
        std::cout << name_ << " is enraged!\n";
    }
    int dmg = std::max(1, power - target.defense());
    std::cout << name_ << " strikes " << target.name() << " for " << dmg << " damage.\n";
    target.take_damage(dmg);
}
