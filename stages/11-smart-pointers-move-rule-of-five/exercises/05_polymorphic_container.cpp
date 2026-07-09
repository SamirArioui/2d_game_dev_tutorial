// Exercise 05 — A container of polymorphic, owned entities (medium–hard)
//
// This is the shape almost every game uses for its entity list: a
// std::vector<std::unique_ptr<Base>>. The vector OWNS the entities; each element
// is a unique_ptr to a Character (or a derived Enemy). Because we store BASE
// pointers, calling a virtual method dispatches to the right override at runtime
// (polymorphism, from stage 07) — and when the vector is destroyed, every
// unique_ptr frees its entity automatically.
//
//   Python: entities = [Character("Aria"), Enemy("Goblin")]   # a list of objects
//   C++:    std::vector<std::unique_ptr<Character>> entities;  # a list of owned objects
//
// Why unique_ptr and not just std::vector<Character>? Because a Character value
// can't hold an Enemy (object slicing) — you need a pointer to the base to keep
// the derived part and get virtual dispatch. unique_ptr gives that safely.
//
// Fill in the TODOs. Expected output:
//   Aria the hero swings for 12
//   Goblin snarls and bites for 7
//   Orc snarls and bites for 15
//   -- cleanup --
//   ~Character Aria
//   ~Enemy Goblin
//   ~Character Goblin
//   ~Enemy Orc
//   ~Character Orc
// (Each enemy runs ~Enemy THEN ~Character — the derived destructor first, then the
//  base — which only happens correctly because ~Character is virtual.)
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra 05_polymorphic_container.cpp -o ex && ./ex

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Base class. NOTE the `virtual` destructor: deleting a derived object through a
// Character* only runs ~Enemy correctly if the base destructor is virtual.
class Character {
public:
    explicit Character(std::string name, int attack) : name_(std::move(name)), attack_(attack) {}
    virtual ~Character() { std::cout << "~Character " << name_ << "\n"; }

    // A virtual method: derived classes override the behaviour.
    virtual void attack() const {
        std::cout << name_ << " the hero swings for " << attack_ << "\n";
    }

protected:
    std::string name_;
    int attack_;
};

class Enemy : public Character {
public:
    Enemy(std::string name, int attack) : Character(std::move(name), attack) {}
    ~Enemy() override { std::cout << "~Enemy " << name_ << "\n"; }

    void attack() const override {
        std::cout << name_ << " snarls and bites for " << attack_ << "\n";
    }
};

int main() {
    std::vector<std::unique_ptr<Character>> entities;

    // TODO 1: add a hero and two enemies. push_back a std::make_unique for each:
    //         entities.push_back(std::make_unique<Character>("Aria", 12));
    //         entities.push_back(std::make_unique<Enemy>("Goblin", 7));
    //         entities.push_back(std::make_unique<Enemy>("Orc", 15));
    //   (make_unique returns a unique_ptr<Enemy>, which converts to
    //    unique_ptr<Character> as it's moved into the vector.)

    // TODO 2: loop over entities with  for (const auto& e : entities)  and call
    //         e->attack();  — the Enemy override fires for enemies, the base
    //         version for the hero. That's runtime polymorphism.

    std::cout << "-- cleanup --\n";
    // TODO 3: nothing to write. When `entities` is destroyed at the end of main,
    //         each unique_ptr deletes its Character/Enemy. Thanks to the virtual
    //         destructor you'll see ~Enemy AND ~Character for the enemies.

    return 0;
}
