// Exercise 02 — Reading a text file line by line (easy–medium)
//
// std::ifstream is an INPUT file stream. std::getline(stream, line) reads one line
// (without the trailing '\n') into a std::string and returns the stream, which is
// "truthy" while a line was successfully read and "falsy" at end-of-file. So the
// idiomatic read loop is:
//
//   while (std::getline(in, line)) { ... }     // Python: for line in f:
//
// This exercise writes a small file first (so it's self-contained), then reads it
// back and prints each line with a line number.
//
// Fill in the TODOs. Expected output:
//   1: Aria
//   2: Ranger
//   3: 30
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 02_read_lines.cpp -o ex && ./ex

#include <fstream>
#include <iostream>
#include <string>

int main() {
    // --- set up: write a file to read back ---
    {
        std::ofstream out("party.txt");
        out << "Aria\n" << "Ranger\n" << "30\n";
    }

    // --- read it back ---
    std::ifstream in("party.txt");
    if (!in) {
        std::cerr << "error: could not open party.txt\n";
        return 1;
    }

    std::string line;
    int number = 1;
    // TODO 1: loop with  while (std::getline(in, line)) { ... }
    //         Inside, print  number: line   (e.g. "1: Aria"), then increment number.

    return 0;
}
