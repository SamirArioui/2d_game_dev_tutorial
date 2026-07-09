// ============================================================================
// Stage 07 mini-project — a tiny turn-based battle
// ============================================================================
// Pulls together every idea from this stage:
//   - a Character class with a constructor + member-initializer list
//   - encapsulation: protected data (name_/hp_/attack_/defense_) behind const getters
//   - const member functions (name(), hp(), is_alive(), ...)
//   - inheritance + virtual dispatch: Enemy overrides take_turn()
//   - RAII: a BattleLog whose DESTRUCTOR prints a closing summary automatically
//
// Coming from Python: Character/Enemy is a base class + subclass, but here methods are only
// polymorphic when marked `virtual`, data can be genuinely hidden (not just _underscored), and
// the BattleLog destructor runs deterministically at scope exit (unlike Python's __del__).
//
// Build & run (from this stage folder):
//   cmake -S . -B build && cmake --build build
//   ./build/battle
// ============================================================================

#include <iostream>
#include <string>

#include "battle_log.hpp"
#include "character.hpp"
#include "enemy.hpp"

int main() {
    // Two combatants. `hero` is a plain Character; `goblin` is an Enemy with a rage threshold.
    // They are stored here as their real types; stage 11 will hold them as unique_ptr<Character>
    // and still dispatch take_turn() correctly.
    Character hero{"Aria the Bold", 30, 12, 4};
    Enemy goblin{"Goblin Raider", 24, 10, 2, /*rage_below=*/10};

    // The BattleLog is created here and destroyed automatically when main returns. Its
    // destructor — the RAII payload — prints the closing summary with no explicit call.
    BattleLog log{"Aria vs Goblin"};

    int round = 1;
    while (hero.is_alive() && goblin.is_alive()) {
        log.record("Round " + std::to_string(round));

        // Dynamic dispatch in action: goblin.take_turn resolves to Enemy::take_turn at run time.
        hero.take_turn(goblin);
        if (!goblin.is_alive()) {
            break;
        }
        goblin.take_turn(hero);
        ++round;
    }

    // Report the winner through the const getters. We use a base-class POINTER so it can aim at
    // either combatant (an Enemy* converts to a Character*); the getters are the same for both.
    const Character* winner = hero.is_alive() ? &hero : &goblin;
    std::cout << "\nWinner: " << winner->name() << " (" << winner->hp() << " hp left)\n";

    // main returns here -> `log` goes out of scope -> ~BattleLog() runs -> closing line prints.
    return 0;
}
