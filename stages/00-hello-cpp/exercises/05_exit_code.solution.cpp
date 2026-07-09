// Exercise 05 — Exit codes & stderr (solution)
#include <iostream>

int main() {
    std::cout << "Loading assets..." << std::endl;

    // Diagnostic goes to the error stream, not the normal output stream.
    std::cerr << "ERROR: could not find player.png" << std::endl;

    // Non-zero exit code tells the OS / calling shell that we failed.
    return 1;
}
