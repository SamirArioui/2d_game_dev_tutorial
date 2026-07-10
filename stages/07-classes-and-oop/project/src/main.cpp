// ============================================================================
// Stage 07 mini-project — a tiny turn-based battle             (YOUR TASK)
// ============================================================================
// This is the STARTER. Implement it yourself, combining every idea from this stage:
//   - a Character class with a constructor + member-initializer list
//   - encapsulation: protected data (name_/hp_/attack_/defense_) behind const getters
//   - const member functions (name(), hp(), is_alive(), ...)
//   - inheritance + virtual dispatch: Enemy overrides take_turn()
//   - RAII: a BattleLog whose DESTRUCTOR prints a closing summary automatically
//
// WHAT TO FILL IN:
//   - the method bodies in character.cpp, enemy.cpp and battle_log.cpp (the class DECLARATIONS
//     in the .hpp headers are given — do not change the signatures);
//   - the battle-loop skeleton in main() below.
//
// A complete reference is in ../solution/ (build it standalone) — try it yourself first.
//
// Build & run (from this stage folder):
//   cmake -S . -B build && cmake --build build
//   ./build/battle
//
// GOAL — once implemented, the program prints exactly:
//   === Aria vs Goblin ===
//     Round 1
//   Aria the Bold attacks Goblin Raider for 10 damage.
//   Goblin Raider strikes Aria the Bold for 6 damage.
//     Round 2
//   Aria the Bold attacks Goblin Raider for 10 damage.
//   Goblin Raider is enraged!
//   Goblin Raider strikes Aria the Bold for 11 damage.
//     Round 3
//   Aria the Bold attacks Goblin Raider for 10 damage.
//
//   Winner: Aria the Bold (13 hp left)
//   === Aria vs Goblin closed (3 entries logged) ===
// ============================================================================

#include <iostream>
#include <string>

#include "battle_log.hpp"
#include "character.hpp"
#include "enemy.hpp"

int main() {
    // TODO(stage 07): create the two combatants — a plain Character `hero`
    //   {"Aria the Bold", 30, 12, 4} and an Enemy `goblin`
    //   {"Goblin Raider", 24, 10, 2, /*rage_below=*/10}.

    // TODO(stage 07): create a `BattleLog log{"Aria vs Goblin"};`. Its constructor prints the
    //   opening banner now; its destructor prints the closing summary automatically when main
    //   returns — do NOT call the destructor yourself.

    // TODO(stage 07): run the battle loop while both are alive. Each round:
    //   log.record("Round " + std::to_string(round)); hero.take_turn(goblin); if the goblin is
    //   dead, break; else goblin.take_turn(hero); then ++round. `goblin.take_turn` dispatches to
    //   Enemy::take_turn via `virtual`.

    // TODO(stage 07): pick the winner through a base-class pointer (Character*) — whichever is
    //   still alive — and print "\nWinner: <name> (<hp> hp left)\n" using the const getters.

    return 0;  // main returns -> `log` leaves scope -> ~BattleLog() prints the closing line
}
