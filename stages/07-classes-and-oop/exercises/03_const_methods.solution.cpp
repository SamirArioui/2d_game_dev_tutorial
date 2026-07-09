// Exercise 03 — const member functions (solution)
#include <iostream>
#include <string>
#include <utility>

class Character {
public:
    Character(std::string name, int hp) : name_(std::move(name)), hp_(hp) {}

    const std::string& name() const { return name_; }   // const: read-only
    int hp() const { return hp_; }                       // const: read-only

    void take_damage(int amount) { hp_ -= amount; }      // non-const: mutates hp_

private:
    std::string name_;
    int hp_;
};

void describe(const Character& c) {
    // Only legal because name() and hp() are const methods — you can't call a non-const method
    // through a const reference.
    std::cout << c.name() << " has " << c.hp() << " hp\n";
}

int main() {
    Character hero{"Aria", 30};
    hero.take_damage(5);
    describe(hero);   // prints "Aria has 25 hp"
    return 0;
}
