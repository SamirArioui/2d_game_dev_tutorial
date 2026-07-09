// Exercise 01 — Declaration vs definition (easy)
//
// A DECLARATION tells the compiler "this function exists — here are its name, parameter
// types, and return type. Trust me, the body lives somewhere." A DEFINITION provides the
// actual body. In a single file you can declare a function at the top and define it lower
// down; the declaration is enough for calls in between to type-check.
//
// This split is the seed of the whole stage: a header (.hpp) is a file full of
// DECLARATIONS, and a matching .cpp file provides the DEFINITIONS.
//
// Python note: Python has neither concept. You write `def f(): ...` once and call it —
// there is no separate "promise the function exists" step, and no compile phase that would
// need one.
//
// TODO: below main, write the DEFINITION of xp_for_level matching its declaration.
//       Formula: level * level * 100  (a classic RPG curve).
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 01_declaration_vs_definition.cpp -o ex && ./ex

#include <iostream>

// DECLARATION (a.k.a. prototype): signature only, no body, ends with a semicolon.
int xp_for_level(int level);

int main() {
    for (int level = 1; level <= 5; ++level) {
        // We can CALL xp_for_level here even though its body appears later in the file:
        // the declaration above is all the compiler needs to check this call.
        std::cout << "Level " << level << " needs " << xp_for_level(level) << " XP\n";
    }
    return 0;
}

// TODO: write the definition of xp_for_level here, e.g.
// int xp_for_level(int level) {
//     return ...;
// }
