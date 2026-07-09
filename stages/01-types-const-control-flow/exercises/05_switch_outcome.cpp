// Exercise 05 — switch (medium-hard)
//
// A switch chooses one branch based on an integer (or char / enum) value. It's often
// clearer than a long if/else-if chain when you're comparing ONE value against many
// fixed cases. Python has no switch (3.10+ has match/case, which is close).
//
//   switch (value) {
//       case 1:  ...; break;   // 'break' stops fall-through into the next case
//       case 2:  ...; break;
//       default: ...; break;   // runs when nothing else matched (like 'else')
//   }
//
// IMPORTANT: without `break`, execution "falls through" into the next case. That is a
// classic C++ bug — always break unless you fall through on purpose.
//
// Scenario: classify d20 attack rolls. 1 = fumble, 20 = critical hit, anything else = hit.
//
// TODO:
//   1. Make `std::array<int, 6> attack_rolls = {1, 7, 13, 20, 4, 20};`.
//   2. Range-for over the rolls (use `auto`).
//   3. For each roll, use a switch:
//        case 1:  print  "Roll  1: Fumble!"
//        case 20: print  "Roll 20: CRITICAL HIT!"
//        default: print  "Roll <n>: hit"
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_switch_outcome.cpp -o ex && ./ex

#include <array>
#include <iostream>

int main() {
    std::array<int, 6> attack_rolls = {1, 7, 13, 20, 4, 20};

    // TODO: range-for over attack_rolls and switch on each value.
    (void)attack_rolls;  // placeholder so the starter compiles; delete once you use it.

    return 0;
}
