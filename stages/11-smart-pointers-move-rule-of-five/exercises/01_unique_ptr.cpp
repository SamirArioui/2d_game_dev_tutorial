// Exercise 01 — std::unique_ptr (easy)
//
// In Python every object lives on the heap and the garbage collector frees it when
// no names refer to it anymore. In C++ YOU decide where memory lives and who owns
// it. A std::unique_ptr<T> is a heap object with a SINGLE, exclusive owner: when
// the unique_ptr is destroyed (goes out of scope), it deletes what it points to.
// No manual `delete`, no leaks. This is RAII (stage 07/10) applied to raw memory.
//
//   raw C++ (don't):   Enemy* e = new Enemy("Goblin");  ... delete e;  // easy to forget
//   modern C++:        auto e = std::make_unique<Enemy>("Goblin");     // freed automatically
//
// You use it like a pointer: e->method(), (*e).field.
//
// Fill in the TODOs. Expected output:
//   spawned Goblin (hp 12)
//   Goblin takes 5 damage (hp now 7)
//   ~Enemy Goblin
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 01_unique_ptr.cpp -o ex && ./ex

#include <iostream>
#include <memory>  // std::unique_ptr, std::make_unique
#include <string>

struct Enemy {
    std::string name;
    int hp;

    Enemy(std::string n, int h) : name(std::move(n)), hp(h) {
        std::cout << "spawned " << name << " (hp " << hp << ")\n";
    }
    ~Enemy() { std::cout << "~Enemy " << name << "\n"; }  // proves auto-cleanup

    void take_damage(int amount) {
        hp -= amount;
        std::cout << name << " takes " << amount << " damage (hp now " << hp << ")\n";
    }
};

int main() {
    // TODO 1: create a unique_ptr<Enemy> named `goblin` for an Enemy("Goblin", 12)
    //         using std::make_unique<Enemy>("Goblin", 12).

    // TODO 2: call take_damage(5) through it with the -> operator.

    // TODO 3: nothing! When `goblin` goes out of scope at the end of main, its
    //         destructor deletes the Enemy for you — you should see "~Enemy Goblin"
    //         printed WITHOUT any manual delete. (That's the whole point.)

    return 0;
}
