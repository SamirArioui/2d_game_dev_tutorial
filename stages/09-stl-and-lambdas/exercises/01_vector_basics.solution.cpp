// Exercise 01 — Vector basics (solution)
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> party;

    // push_back appends to the end and grows the vector as needed.
    party.push_back("Aria");
    party.push_back("Borin");
    party.push_back("Cera");

    std::cout << "party size: " << party.size() << "\n";

    // Range-for visits each element in order. const& = read-only, no copy.
    for (const std::string& hero : party) {
        std::cout << hero << "\n";
    }

    // back() returns a reference to the last element (undefined if empty).
    std::cout << "last hero: " << party.back() << "\n";

    return 0;
}
