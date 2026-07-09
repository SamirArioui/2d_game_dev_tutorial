// Exercise 02 — Pass by reference (easy-medium)
//
// A REFERENCE parameter (written with &) is an ALIAS for the caller's variable, not a
// copy. Writing through it changes the caller's original. This is how a C++ function
// can "give back" more than one result, or modify something in place.
//
//   void add_xp(int& xp, int amount) {   // xp is a reference: it IS the caller's variable
//       xp += amount;                    // this changes the original
//   }
//
// Python surprise: in Python you CANNOT do this with immutable values like ints — a
// function can never reassign the caller's integer. C++ references make it explicit and
// possible. (Python gets similar effect only via mutable containers or returning a value.)
//
// TODO:
//   1. Define  void add_xp(int& xp, int amount)  that does  xp += amount;
//   2. Define  void grant_reward(int& gold, int& xp)  that adds 100 gold and 50 xp
//      through the two reference parameters (one call, TWO values updated in place).
//   3. In main: start with gold = 0, xp = 0. Call add_xp(xp, 30). Then grant_reward(gold, xp).
//      Print gold and xp after each call to watch the ORIGINALS change.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_pass_by_reference.cpp -o ex && ./ex

#include <iostream>

// TODO: define add_xp() and grant_reward() with reference parameters.

int main() {
    int gold = 0;
    int xp = 0;

    // TODO: call add_xp(xp, 30); print xp.
    // TODO: call grant_reward(gold, xp); print gold and xp.
    (void)gold;  // remove once you use them
    (void)xp;
    return 0;
}
