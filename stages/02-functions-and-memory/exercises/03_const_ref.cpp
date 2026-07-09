// Exercise 03 — Pass by const reference (medium)
//
// There are THREE ways to pass an argument, and picking the right one is a core C++ habit:
//
//   void f(BigThing x)         // by value       — makes a full COPY (can be expensive)
//   void f(BigThing& x)        // by reference    — no copy, CAN modify the original
//   void f(const BigThing& x)  // by const ref    — no copy, CANNOT modify the original
//
// `const T&` is the everyday default for passing anything bigger than a number you only
// want to READ: you avoid the copy AND you promise not to change it. For a plain int the
// copy is free, so we pass ints by value — but a std::array (or a string, or a Vec2 later)
// is worth passing by const&.
//
// Python has no copies-vs-reference choice and no const, so this trade-off is new.
//
// TODO:
//   1. Define  int sum(const std::array<int, 5>& values)  that adds the elements with a
//      range-for and returns the total. The const& means: look, don't copy, don't touch.
//   2. (Read-only proof) Inside sum you may NOT write to values — try `values[0] = 0;`
//      to see the compiler reject it, then remove that line.
//   3. In main: make a 5-element array of party HP, call sum(...), and print the total.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_const_ref.cpp -o ex && ./ex

#include <array>
#include <iostream>

// TODO: define sum(const std::array<int, 5>& values).

int main() {
    std::array<int, 5> party_hp = {30, 24, 40, 18, 33};
    // TODO: call sum(party_hp) and print the total.
    (void)party_hp;  // remove once you use it
    return 0;
}
