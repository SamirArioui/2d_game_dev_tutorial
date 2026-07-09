// Exercise 01 — Declaration vs definition (solution)
#include <iostream>

// Declaration: the compiler only needs this to accept the calls in main.
int xp_for_level(int level);

int main() {
    for (int level = 1; level <= 5; ++level) {
        std::cout << "Level " << level << " needs " << xp_for_level(level) << " XP\n";
    }
    return 0;
}

// Definition: the body. Its signature must match the declaration exactly.
int xp_for_level(int level) {
    return level * level * 100;
}
