// Exercise 01 — Vector basics (easy)
//
// std::vector is C++'s dynamic array — the closest thing to a Python `list`.
// It grows on demand, stores its elements contiguously in memory, and always
// knows its own size. Unlike a Python list it holds ONE element type (here,
// std::string), which the compiler enforces.
//
// Fill in the TODOs. Expected output:
//   party size: 3
//   Aria
//   Borin
//   Cera
//   last hero: Cera
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 01_vector_basics.cpp -o ex && ./ex

#include <iostream>
#include <string>
#include <vector>

int main() {
    // A vector of strings. Compare to Python:  party = []
    std::vector<std::string> party;

    // TODO 1: add "Aria", "Borin", "Cera" with push_back.
    //         Python: party.append("Aria")  ->  C++: party.push_back("Aria");

    // TODO 2: print  party size: <n>
    //         .size() returns how many elements are stored (Python: len(party)).

    // TODO 3: print each hero on its own line with a range-for loop.
    //         Python: for hero in party: print(hero)
    //         C++:    for (const std::string& hero : party) { ... }
    //         (const& avoids copying each string as we visit it.)

    // TODO 4: print  last hero: <name>  using party.back() (the final element).

    return 0;
}
