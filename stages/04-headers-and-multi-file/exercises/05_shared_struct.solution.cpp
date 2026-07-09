// Exercise 05 — A struct shared across translation units (solution)
#include "05_shared_struct.hpp"

#include <iostream>

int total_threat(const Enemy& enemy) {
    return enemy.hp + enemy.attack;
}

void print_enemy(const Enemy& enemy) {
    // Reuse the other function — both are defined in this same translation unit.
    std::cout << enemy.name << " (hp " << enemy.hp << ", threat " << total_threat(enemy) << ")\n";
}
