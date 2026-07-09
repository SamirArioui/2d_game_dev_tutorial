// Exercise 02 — Overloading operator== (solution)
#include <iostream>
#include <string>

struct Card {
    std::string suit;
    int rank;

    bool operator==(const Card& other) const {
        return suit == other.suit && rank == other.rank;
    }

    // Define != in terms of == so the two can never disagree — a common, tidy idiom.
    bool operator!=(const Card& other) const { return !(*this == other); }
};

int main() {
    Card a{"hearts", 7};
    Card b{"hearts", 7};
    Card c{"spades", 7};
    std::cout << "a == b? " << (a == b ? "yes" : "no") << "\n";   // yes
    std::cout << "a != c? " << (a != c ? "yes" : "no") << "\n";   // yes
    return 0;
}
