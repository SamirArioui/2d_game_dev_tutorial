#pragma once

// (REFERENCE SOLUTION — the starter version is two levels up in ../../src/)

#include <string>

// ---------------------------------------------------------------------------
// Character — the base class for everyone who can fight.
//
// This is the shared contract the rest of the course builds on. Stage 11 will own these
// objects through a std::unique_ptr<Character> and call take_turn() on each, which is WHY
// the destructor below is virtual: deleting a derived object (Enemy) through a Character*
// must still run the derived cleanup.
// ---------------------------------------------------------------------------
class Character {
public:
    // Constructor. The MEMBER-INITIALIZER LIST (everything after the ':' and before the '{')
    // constructs each member directly with its final value. That is cheaper than assigning in
    // the body, and for const/reference members it is the ONLY option. The std::string is
    // taken by value and moved in — the idiomatic "sink parameter" since C++11.
    Character(std::string name, int hp, int attack, int defense);

    // A base class used polymorphically needs a VIRTUAL destructor. `= default` asks the
    // compiler to generate the ordinary destructor body; `virtual` guarantees the right
    // destructor runs when an object is deleted through a base-class pointer (stage 11).
    virtual ~Character() = default;

    // --- const getters -----------------------------------------------------------------
    // The trailing `const` is a promise: "calling this does not modify the object". Only
    // const methods may be called on a `const Character&`, so const-correctness here is what
    // lets the rest of the code pass characters around by const reference without copying.
    const std::string& name() const { return name_; }
    int hp() const { return hp_; }
    int attack() const { return attack_; }
    int defense() const { return defense_; }
    bool is_alive() const { return hp_ > 0; }

    // Apply damage. NOT const — it changes hp_. Clamped so hp never goes below 0; that
    // invariant is exactly what encapsulation (hidden data + methods) exists to protect.
    void take_damage(int amount);

    // The polymorphic action. `virtual` means "a derived class may replace this, and which
    // version runs is chosen at RUN TIME from the object's real type". The base version is a
    // plain attack. The target is taken BY REFERENCE so the turn hits the real opponent.
    virtual void take_turn(Character& target);

protected:
    // `protected` (not private): derived classes such as Enemy may use these directly, while
    // code outside the hierarchy must still go through the public getters above.
    std::string name_;
    int hp_;
    int attack_;
    int defense_;
};
