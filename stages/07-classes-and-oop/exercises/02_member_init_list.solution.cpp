// Exercise 02 — The member-initializer list (solution)
#include <iostream>
#include <string>
#include <utility>   // std::move

class Character {
public:
    // All three members constructed directly, in declaration order. std::move avoids copying
    // the string. The const member max_hp_ can ONLY be set here in the init list.
    Character(std::string name, int hp, int max_hp)
        : name_(std::move(name)), hp_(hp), max_hp_(max_hp) {}

    const std::string& name() const { return name_; }
    int hp() const { return hp_; }
    int max_hp() const { return max_hp_; }

private:
    std::string name_;
    int hp_;
    const int max_hp_;
};

int main() {
    Character hero{"Aria", 30, 30};
    std::cout << hero.name() << ": " << hero.hp() << "/" << hero.max_hp() << " hp\n";
    return 0;
}
