// Exercise 02 — Overloading operator== (medium)
//
// Define equality for your type so `a == b` works — needed by tests (stage 13) and by containers
// that compare elements. Python's version is __eq__.
//
// TODO:
//   1. Implement operator== : two Cards are equal when BOTH suit and rank match.
//   2. Implement operator!= in terms of operator==  (return !(*this == other)) — don't repeat
//      the comparison logic.
//   3. In main, compare a few cards.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 02_equality.cpp -o ex && ./ex

#include <iostream>
#include <string>

struct Card {
    std::string suit;
    int rank;

    // TODO: return true when both fields match.
    bool operator==(const Card& other) const {
        return false;  // <-- replace
    }

    // TODO: implement using operator== so the two never disagree.
    bool operator!=(const Card& other) const {
        return true;  // <-- replace
    }
};

int main() {
    Card a{"hearts", 7};
    Card b{"hearts", 7};
    Card c{"spades", 7};
    std::cout << "a == b? " << (a == b ? "yes" : "no") << "\n";
    std::cout << "a != c? " << (a != c ? "yes" : "no") << "\n";
    return 0;
}
