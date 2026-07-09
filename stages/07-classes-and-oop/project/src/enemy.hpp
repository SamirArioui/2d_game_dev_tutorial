#pragma once

#include <string>

#include "character.hpp"

// ---------------------------------------------------------------------------
// Enemy — an "IS-A Character" with its own take_turn behaviour.
//
// `: public Character` is public inheritance: an Enemy can be used anywhere a Character is
// expected. Stage 11 will store a mix of Characters and Enemies in one container of
// unique_ptr<Character> and call take_turn() on each — dynamic dispatch picks the right one.
// ---------------------------------------------------------------------------
class Enemy : public Character {
public:
    // Enemy adds a "rage" threshold: when its hp drops below rage_below it hits harder. The
    // shared fields are forwarded to the Character constructor (see the .cpp init list).
    Enemy(std::string name, int hp, int attack, int defense, int rage_below);

    // `override` tells the compiler "this is meant to replace a base virtual with a matching
    // signature". If it didn't actually override anything, compilation fails — a free safety net.
    void take_turn(Character& target) override;

private:
    int rage_below_;
};
