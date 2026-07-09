// Exercise 04 — One Definition Rule: the duplicate symbol (solution)
//
// Fix: this file no longer DEFINES max_party_size(); it only pulls in the DECLARATION from
// the header. The single definition in 04_duplicate_symbol_odr_lib.cpp is now the one and
// only body — the ODR is satisfied and the program links.
//
//   clang++ -std=c++17 -Wall -Wextra 04_duplicate_symbol_odr.solution.cpp \
//           04_duplicate_symbol_odr_lib.cpp -o ex && ./ex
#include "04_duplicate_symbol_odr_lib.hpp"

#include <iostream>

int main() {
    std::cout << "Max party size: " << max_party_size() << "\n";
    return 0;
}
