// Exercise 02 — std::move and transferring ownership (medium)
//
// A unique_ptr has EXCLUSIVE ownership, so it cannot be COPIED — two unique_ptrs
// owning the same object would both try to delete it (a double-free). Instead you
// MOVE it: transfer ownership from one to another with std::move. After the move
// the source is left empty (null).
//
//   auto a = std::make_unique<Enemy>("Orc", 20);
//   auto b = std::move(a);   // ownership moves a -> b; `a` is now null
//
// std::move doesn't move anything by itself — it's a cast that says "you may take
// my guts". The unique_ptr's move-constructor does the actual pointer hand-off.
//
// Python has no direct analog: names are just references and the GC counts them.
// C++ makes ownership explicit — exactly one unique_ptr owns the object at a time.
//
// Fill in the TODOs. Expected output:
//   made Orc
//   after move: original is empty, holder owns Orc
//   holder -> Orc (hp 20)
//   ~Enemy Orc
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 02_move_unique.cpp -o ex && ./ex

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

    // A copy is a COMPILE ERROR — uncomment to see the compiler forbid it:
    // std::unique_ptr<Enemy> copy = original;   // error: call to deleted copy ctor

    // TODO 1: move ownership from `original` into a new unique_ptr `holder`
    //         using std::move(original).
    std::unique_ptr<Enemy> holder;  // starts empty

    // TODO 2: print the state after the move. A unique_ptr is truthy when it owns
    //         something. Expected:
    //         "after move: original is empty, holder owns Orc"
    //         Hint: check !original and (bool)holder.

    // TODO 3: print  holder -> <name> (hp <hp>)  by accessing through holder.

    return 0;  // holder frees the Orc; original owns nothing, so it frees nothing
}
