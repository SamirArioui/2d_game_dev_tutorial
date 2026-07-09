// Exercise 02 — Brace / aggregate initialization (easy-medium)
//
// A plain struct (all public members, no custom constructor) is called an AGGREGATE, and
// you can initialise it directly from a braced list. Forms worth knowing:
//
//   Stats a{30, 12, 8};      // direct brace-init: hp=30, attack=12, defense=8 (BY ORDER)
//   Stats b = {30, 12, 8};   // same thing, with an '='
//   Stats c{};               // ALL members zero-initialised: hp=0, attack=0, defense=0
//
// The order matters — members fill in the order they are declared. Prefer braces {} over
// old parentheses: braces also FORBID NARROWING (e.g. Stats{3.9} won't silently chop 3.9
// to 3 — it's a compile error, which catches bugs).
//
// A note on PARTIAL init: `Stats{50}` is legal and zero-fills the rest (attack=0,
// defense=0). BUT with -Wextra the compiler warns ("missing field initializer") because a
// half-filled struct is often a mistake. So: use {} when you want all zeros, and list
// every field otherwise. (This is a good example of a warning that is trying to help.)
//
// Given this struct:
//   struct Stats { int hp; int attack; int defense; };
//
// TODO:
//   1. Create `full`   with all three values, e.g. {30, 12, 8}.
//   2. Create `copy`   using the '=' form:  Stats copy = {12, 4, 2};
//   3. Create `zeroed` with {} and confirm every member is 0.
//   4. Print all three structs' members.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_brace_init.cpp -o ex && ./ex

#include <iostream>

struct Stats {
    int hp;
    int attack;
    int defense;
};

int main() {
    // TODO: make full{...}, copy = {...}, zeroed{}; then print each one's hp/attack/defense.
    return 0;
}
