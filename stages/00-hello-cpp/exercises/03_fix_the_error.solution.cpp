// Exercise 03 — Fix the error (solution)
//
// The three fixes:
//   #1  added the missing semicolon after the first line.
//   #2  "std::cuot" was a typo for "std::cout".
//   #3  added the missing semicolon after "return 0".
#include <iostream>

int main() {
    std::cout << "Loading level 1..." << std::endl;

    std::cout << "Spawning enemies" << std::endl;

    std::cout << "Ready to play!" << std::endl;

    return 0;
}
