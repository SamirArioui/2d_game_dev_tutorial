// Exercise 05 — Inheritance & virtual dispatch (medium-hard)
//
// Inheritance lets a derived class reuse and specialise a base class. `virtual` methods are the
// key to POLYMORPHISM: when you call a virtual method through a base pointer or reference, C++
// picks the version matching the object's REAL type, decided at run time.
//
// PYTHON NOTE: in Python every method is effectively virtual and any override "just works". In
// C++ you must (a) mark the base method `virtual`, (b) write `override` on the derived one, and
// (c) give the base class a `virtual` destructor if objects will be deleted via a base pointer.
//
// TODO:
//   1. Make Character::speak() virtual.
//   2. Make Enemy derive from Character (`: public Character`) and OVERRIDE speak() with a
//      different message. Give Enemy a constructor that forwards the name to Character.
//   3. In main, bind a Character& to an Enemy and call speak() — watch Enemy::speak run.
//
// Compile & run:
//   clang++ -std=c++17 -Wall -Wextra 05_inheritance_virtual.cpp -o ex && ./ex

#include <iostream>
#include <string>
#include <utility>

class Character {
public:
    explicit Character(std::string name) : name_(std::move(name)) {}
    virtual ~Character() = default;

    // TODO: make this virtual.
    void speak() const { std::cout << name_ << ": I am a hero.\n"; }

protected:
    std::string name_;
};

// TODO: make Enemy inherit from Character and override speak().
class Enemy {
};

int main() {
    // TODO:
    //   Enemy goblin{"Goblin"};
    //   Character& ref = goblin;   // a base reference bound to a derived object
    //   ref.speak();               // should call Enemy::speak thanks to `virtual`
    return 0;
}
