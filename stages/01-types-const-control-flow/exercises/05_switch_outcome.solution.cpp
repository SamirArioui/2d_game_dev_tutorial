// Exercise 05 — switch (solution)
#include <array>
#include <iostream>

int main() {
    std::array<int, 6> attack_rolls = {1, 7, 13, 20, 4, 20};

    for (auto roll : attack_rolls) {
        // switch compares 'roll' against each case label. break stops fall-through.
        switch (roll) {
            case 1:
                std::cout << "Roll  1: Fumble!\n";
                break;
            case 20:
                std::cout << "Roll 20: CRITICAL HIT!\n";
                break;
            default:
                // Everything not matched above lands here (like Python's else).
                std::cout << "Roll " << roll << ": hit\n";
                break;
        }
    }

    return 0;
}
