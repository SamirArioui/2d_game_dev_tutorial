// Exercise 02 — Pass by reference (solution)
#include <iostream>

// 'xp' is a reference: no copy is made, and the += updates the caller's variable.
void add_xp(int& xp, int amount) {
    xp += amount;
}

// Two reference out-parameters: one call updates both of the caller's variables.
void grant_reward(int& gold, int& xp) {
    gold += 100;
    xp += 50;
}

int main() {
    int gold = 0;
    int xp = 0;

    add_xp(xp, 30);   // xp is passed by reference, so the real xp changes
    std::cout << "After add_xp:      gold=" << gold << " xp=" << xp << "\n";

    grant_reward(gold, xp);   // both originals updated in place
    std::cout << "After grant_reward: gold=" << gold << " xp=" << xp << "\n";

    return 0;
}
