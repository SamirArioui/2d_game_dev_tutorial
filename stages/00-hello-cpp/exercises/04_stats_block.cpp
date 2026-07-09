// Exercise 04 — Stats block (medium)
//
// Goal: mix text and numbers in one std::cout chain, and line things up.
//
// Print a stats block exactly like this (numbers are literals — just type them):
//
//   -- STATS --
//   HP     : 30
//   Attack : 12
//   Defense: 8
//
// Notes:
//   - You can send an int straight to std::cout: `std::cout << 30;` (no quotes around numbers).
//   - Mix freely: std::cout << "HP     : " << 30 << "\n";
//   - The spaces after the labels are what make the colons line up. Count them.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 04_stats_block.cpp -o ex && ./ex

#include <iostream>

int main() {
    // TODO: print the four lines above, sending the numbers as integer literals (not strings).
    return 0;
}
