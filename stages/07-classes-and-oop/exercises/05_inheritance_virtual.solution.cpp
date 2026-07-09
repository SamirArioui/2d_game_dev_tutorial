// Exercise 05 — Inheritance & virtual dispatch (solution)
#include <iostream>
#include <string>
#include <utility>

class Character {
public:
    explicit Character(std::string name) : name_(std::move(name)) {}
    virtual ~Character() = default;   // virtual dtor: safe to delete an Enemy via a Character*

    virtual void speak() const { std::cout << name_ << ": I am a hero.\n"; }

protected:
    std::string name_;
};

class Enemy : public Character {
public:
    explicit Enemy(std::string name) : Character(std::move(name)) {}

    // `override`: the compiler checks this really replaces a base virtual with a matching
    // signature. Misspell it (e.g. `speak() const` vs `speak()`) and you get a compile error
    // instead of a silent non-override.
    void speak() const override { std::cout << name_ << ": I am your doom!\n"; }
};

int main() {
    Enemy goblin{"Goblin"};

    // A base reference bound to a derived object. The call below is resolved AT RUN TIME to the
    // object's real type (Enemy) — that's dynamic dispatch.
    Character& ref = goblin;
    ref.speak();   // prints "Goblin: I am your doom!"

    // Without `virtual` on speak(), ref.speak() would print the Character version instead,
    // because the choice would be made from the reference's static type (Character).
    return 0;
}
