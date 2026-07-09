// Exercise 02 — Brace / aggregate initialization (solution)
#include <iostream>
#include <string>

struct Stats {
    int hp;
    int attack;
    int defense;
};

// A small helper so we don't repeat the printing three times.
void print_stats(const std::string& label, const Stats& s) {
    std::cout << label << ": hp=" << s.hp
              << " attack=" << s.attack
              << " defense=" << s.defense << "\n";
}

int main() {
    Stats full{30, 12, 8};        // fills hp, attack, defense in declaration order
    Stats copy = {12, 4, 2};      // the '=' form does the same thing
    Stats zeroed{};               // every member value-initialised to 0 — the clean way to zero

    print_stats("full  ", full);
    print_stats("copy  ", copy);
    print_stats("zeroed", zeroed);

    // Partial init IS legal: `Stats half{50};` gives hp=50, attack=0, defense=0. We avoid it
    // here because -Wextra warns (missing-field-initializers) — a helpful nudge, since a
    // half-filled struct is usually an accident. Use {} for all-zero, or list every field.

    return 0;
}
