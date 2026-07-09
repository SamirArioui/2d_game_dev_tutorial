// Exercise 05 — A container of polymorphic, owned entities (solution)
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Character {
public:
    explicit Character(std::string name, int attack) : name_(std::move(name)), attack_(attack) {}
    virtual ~Character() { std::cout << "~Character " << name_ << "\n"; }

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
    entities.push_back(std::make_unique<Character>("Aria", 12));
    entities.push_back(std::make_unique<Enemy>("Goblin", 7));
    entities.push_back(std::make_unique<Enemy>("Orc", 15));

    for (const auto& e : entities) {
        e->attack();  // virtual dispatch: base or override, chosen at runtime
    }

    std::cout << "-- cleanup --\n";
    return 0;  // entities destroyed -> each unique_ptr frees its object
}
