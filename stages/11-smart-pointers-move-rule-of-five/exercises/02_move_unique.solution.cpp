// Exercise 02 — std::move and transferring ownership (solution)
#include <iostream>
#include <memory>
#include <string>

struct Enemy {
    std::string name;
    int hp;
    Enemy(std::string n, int h) : name(std::move(n)), hp(h) {
        std::cout << "made " << name << "\n";
    }
    ~Enemy() { std::cout << "~Enemy " << name << "\n"; }
};

int main() {
    std::unique_ptr<Enemy> original = std::make_unique<Enemy>("Orc", 20);

    // std::unique_ptr<Enemy> copy = original;   // would not compile: copy is deleted

    std::unique_ptr<Enemy> holder = std::move(original);  // ownership transfers

    std::cout << "after move: original is "
              << (!original ? "empty" : "NOT empty") << ", holder owns "
              << (holder ? holder->name : std::string{"nothing"}) << "\n";

    std::cout << "holder -> " << holder->name << " (hp " << holder->hp << ")\n";

    return 0;
}
