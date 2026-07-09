// Exercise 03 — The undefined reference (medium)   [THE LINKER-ERROR EXERCISE]
//
// This one shows the difference between a COMPILE error and a LINK error, in a single file.
//
// `armor_rating` below is DECLARED (so the compiler is satisfied — it trusts the promise)
// but never DEFINED anywhere. So the COMPILE step SUCCEEDS, and the LINK step FAILS: the
// linker goes looking for the body and finds nothing.
//
// 1. Watch it fail at the LINK step:
//      clang++ -std=c++17 -Wall -Wextra 03_undefined_reference.cpp -o ex
//    You'll see something like:
//      Undefined symbols for architecture arm64:
//        "armor_rating(int, int)", referenced from: _main in ...o
//      ld: symbol(s) not found for architecture arm64
//      clang: error: linker command failed with exit code 1
//    The "ld:" prefix is the LINKER talking — not the compiler. Different tool, different
//    stage of the pipeline (see the README).
//
// 2. Prove the COMPILE step alone is fine (-c = compile only, do NOT link):
//      clang++ -std=c++17 -Wall -Wextra -c 03_undefined_reference.cpp -o ex.o
//    That succeeds and produces ex.o. The compiler had no problem — only the linker does.
//
// TODO: fix the link error by writing the DEFINITION of armor_rating.
//       Formula: base + level * 2

#include <iostream>

// Declaration only — there is no body for this function anywhere in the program.
int armor_rating(int base, int level);

int main() {
    std::cout << "Armor at level 5: " << armor_rating(10, 5) << "\n";
    return 0;
}

// TODO: define armor_rating here so the linker can find its body.
// int armor_rating(int base, int level) {
//     return ...;
// }
