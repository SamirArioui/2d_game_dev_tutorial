// player.cpp — the definition of print_player, declared in player.hpp.
#include "player.hpp"

#include <iostream>

void print_player(const Player& player) {
    std::cout << "  " << player.name << " (hp " << player.hp << ")\n";
}
