// Exercise 01 — Your first struct (solution)
#include <iostream>
#include <string>

// A new type bundling a name and hit points. Members are public by default in a struct.
struct Monster {
    std::string name;
    int hp;
};  // <-- required semicolon

int main() {
    // Brace-init fills members in the order they were declared: name first, hp second.
    Monster goblin{"Goblin", 12};

    std::cout << "Name: " << goblin.name << "\n";   // '.' reads a member, like a Python attribute
    std::cout << "HP  : " << goblin.hp << "\n";

    // Members are variables you can change:
    goblin.hp -= 5;
    std::cout << goblin.name << " takes 5 damage -> HP " << goblin.hp << "\n";

    return 0;
}
