// Exercise 03 — nullptr and choosing pointer vs reference (solution)
#include <iostream>

int* find_first_dead(int hps[], int count) {
    for (int i = 0; i < count; ++i) {
        if (hps[i] == 0) {
            return &hps[i];   // hand back the address so the caller can write through it
        }
    }
    return nullptr;           // sentinel meaning "not found" — a reference couldn't say this
}

int main() {
    int hps[] = {12, 30, 0, 7};
    int count = 4;

    int* dead = find_first_dead(hps, count);

    // ALWAYS null-check before dereferencing. Dereferencing nullptr is undefined behaviour.
    if (dead != nullptr) {
        std::cout << "reviving a fallen ally (was " << *dead << ")\n";
        *dead = 10;
    } else {
        std::cout << "party is healthy\n";
    }

    std::cout << "hps:";
    for (int i = 0; i < count; ++i) {
        std::cout << " " << hps[i];
    }
    std::cout << "\n";
    return 0;
}
