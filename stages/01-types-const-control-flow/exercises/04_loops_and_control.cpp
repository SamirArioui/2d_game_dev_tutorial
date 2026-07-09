// Exercise 04 — Loops & control flow (medium)
//
// C++ has the same looping ideas as Python but with C-style syntax and braces:
//
//   for (int i = 0; i < 3; ++i) { ... }   // classic counter loop
//   while (condition) { ... }             // repeat while true
//   if (x) { ... } else if (y) { ... } else { ... }
//
// Python uses indentation for blocks; C++ uses { }. Always brace the body, even
// for one line (course style rule — it prevents a whole class of bugs).
//
// Scenario: a hero with 20 HP is poisoned and loses 4 HP each turn until dead.
//
// TODO:
//   1. Print a 3-2-1 countdown using a classic for loop (i from 3 down to 1).
//   2. Using a WHILE loop, subtract 4 from hp each turn, counting the turns, until
//      hp <= 0. Each turn print the turn number and remaining hp (clamp display to 0).
//   3. After the loop, use if / else if / else to print a verdict based on `turns`:
//        turns <= 3  -> "The poison was deadly!"
//        turns <= 5  -> "A close call."
//        else        -> "Barely a scratch."
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_loops_and_control.cpp -o ex && ./ex

#include <iostream>

int main() {
    int hp = 20;
    const int poison = 4;
    int turns = 0;

    // TODO 1: for-loop countdown 3, 2, 1.

    // TODO 2: while-loop draining hp by `poison` each turn, counting turns.

    // TODO 3: if / else if / else verdict based on turns.

    return 0;
}
