// Exercise 03 — Fix the error (medium)
//
// This program does NOT compile. There are exactly THREE mistakes.
//
// Your job: compile it, READ the first error (file:line:column + message + caret), fix that one
// mistake, then recompile. Repeat until it compiles and runs. Fixing the first error first often
// makes several later errors disappear.
//
// Expected output once fixed:
//   Loading level 1...
//   Spawning enemies
//   Ready to play!
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 03_fix_the_error.cpp -o ex && ./ex

#include <iostream>

int main() {
    std::cout << "Loading level 1..." << std::endl   // mistake #1

    std::cuot << "Spawning enemies" << std::endl;    // mistake #2

    std::cout << "Ready to play!" << std::endl;

    return 0                                          // mistake #3
}
