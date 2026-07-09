// Exercise 01 — Your first class (easy)
//
// A class bundles DATA with the FUNCTIONS that operate on it, and controls who may touch the
// data. This is the C++ version of a Python class, with two differences you'll feel right away:
//   * members are `private` by default — real, compiler-enforced access control (not Python's
//     leading-underscore convention);
//   * you write the types of the data members explicitly.
//
// PYTHON NOTE:
//   class Health:
//       def __init__(self, hp):   # `self` is explicit; hp is a public attribute
//           self.hp = hp
//   In C++ there is no `self`: inside a method you name members directly (or `this->name`).
//
// TODO:
//   1. Give Health a private data member `hp_` (int).
//   2. Add a public constructor Health(int hp) that initialises hp_ (use a member-init list).
//   3. Add a public method `int hp() const` returning hp_.
//   4. Add a public method `void heal(int amount)` that adds to hp_.
//   5. In main, make a Health{20}, heal it by 5, and print hp().
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_first_class.cpp -o ex && ./ex

#include <iostream>

class Health {
    // TODO: a private data member hp_

    // TODO: a public constructor, hp(), and heal()
};

int main() {
    // TODO: create a Health, heal it, print hp()
    return 0;
}
