// Exercise 04 — Loops & control flow (solution)
#include <iostream>

int main() {
    int hp = 20;
    const int poison = 4;
    int turns = 0;

    // Classic counter loop, counting DOWN. --i decrements; i > 0 is the continue test.
    std::cout << "Fight starts in: ";
    for (int i = 3; i > 0; --i) {
        std::cout << i << " ";
    }
    std::cout << "\n";

    // while: keep looping as long as the condition holds.
    while (hp > 0) {
        hp -= poison;      // shorthand for hp = hp - poison
        ++turns;
        int shown = hp > 0 ? hp : 0;   // don't display negative HP
        std::cout << "Turn " << turns << ": hp = " << shown << "\n";
    }

    // if / else if / else — the C++ spelling of Python's if / elif / else.
    if (turns <= 3) {
        std::cout << "The poison was deadly!\n";
    } else if (turns <= 5) {
        std::cout << "A close call.\n";
    } else {
        std::cout << "Barely a scratch.\n";
    }

    return 0;
}
