// Exercise 01 — std::unique_ptr (solution)
#include <iostream>
#include <memory>
#include <string>

struct Enemy {
    std::string name;
    int hp;

    Enemy(std::string n, int h) : name(std::move(n)), hp(h) {
        std::cout << "spawned " << name << " (hp " << hp << ")\n";
    }
    ~Enemy() { std::cout << "~Enemy " << name << "\n"; }

    void take_damage(int amount) {
        hp -= amount;
        std::cout << name << " takes " << amount << " damage (hp now " << hp << ")\n";
    }
};

int main() {
    auto goblin = std::make_unique<Enemy>("Goblin", 12);  // owns the heap Enemy
    goblin->take_damage(5);                                // use it like a pointer
    return 0;                                              // goblin freed here, automatically
}
