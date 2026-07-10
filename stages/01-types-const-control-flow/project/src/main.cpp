// ============================================================================
// Stage 01 mini-project — Dice-roll battle simulator  (YOUR TASK)
// ============================================================================
//
// This is the STARTER. Implement it yourself, combining every concept from this
// stage: the numeric types, `const` tuning values, `static_cast` for the damage,
// a `std::array` roster walked by range-`for` (and a classic `for` for the
// preview), `auto` to bind each roll, `if` for HP checks, `switch` for the
// outcome, and `<random>` for the rolls.
//
// The setup, the roster preview and the summary are given as worked scaffolding.
// You fill in the two TODOs inside the fight loop — the swing `switch` and the
// HP `if`/`else`. A complete reference is in ../solution/src/main.cpp — try it
// yourself first.
//
// GOAL — with the fixed seed (121), make the program print exactly this:
//
//   == The gauntlet ==
//    1. Goblin
//    2. Orc
//    3. Slime
//    4. Bandit
//    5. Warg
//   Hero HP: 55
//
//   Goblin rolls 3 and hits for 2 damage. Hero HP: 53
//   Orc rolls 9 and hits for 8 damage. Hero HP: 45
//   Slime rolls 2 and hits for 1 damage. Hero HP: 44
//   Bandit rolls 20 — CRITICAL HIT for 36 damage. Hero HP: 8
//   Warg rolls 1 and FUMBLES for 0 damage. Hero HP: 8
//
//   == Result ==
//   Enemies defeated : 5 / 5
//   Total damage taken: 47
//   Avg damage / round: 9.4
//   The hero cleared the gauntlet with 8 HP to spare!
//
// Build & run:
//   clang++ -std=c++17 -Wall -Wextra project/src/main.cpp -o battle
//   ./battle
//   echo $?        # -> 0
// ============================================================================

#include <array>
#include <iostream>
#include <random>
#include <string>

int main() {
    // --- Tuning constants (given): const says "these never change while the game runs". ---
    const int hero_max_hp = 55;         // int   — whole hit points
    const float damage_scale = 0.9f;    // float — turns a raw roll into a damage number
    const int enemy_count = 5;

    // --- The roster (given): a FIXED-SIZE std::array. Its size (5) is baked into the type. -
    std::array<std::string, 5> enemies = {"Goblin", "Orc", "Slime", "Bandit", "Warg"};

    // --- Mutable game state (given) --------------------------------------------------------
    int hero_hp = hero_max_hp;   // this DOES change, so it is not const
    bool hero_alive = true;      // bool — a plain yes/no flag
    int enemies_defeated = 0;
    int total_damage_taken = 0;

    // --- Randomness (given): one seeded engine + one distribution shaping it into d20 rolls.
    // A fixed seed makes every run identical (handy for a tutorial). Swap in
    // std::random_device{}() for genuine unpredictability in a real game.
    std::mt19937 rng(121);
    std::uniform_int_distribution<int> d20(1, 20);

    // --- Roster preview (given worked example): a CLASSIC counter for-loop (index-based). --
    std::cout << "== The gauntlet ==\n";
    for (int i = 0; i < enemy_count; ++i) {
        std::cout << " " << (i + 1) << ". " << enemies[i] << "\n";
    }
    std::cout << "Hero HP: " << hero_hp << "\n\n";

    // --- The fight: a RANGE-FOR walks the roster. The structure is given; you fill the two -
    // --- TODOs (the swing `switch` and the HP `if`/`else`). --------------------------------
    for (const std::string& enemy : enemies) {
        if (!hero_alive) {
            break;  // stop early: no point fighting once the hero is down
        }

        auto roll = d20(rng);                                     // 'auto' -> int, from d20
        int base_damage = static_cast<int>(roll * damage_scale); // float math, made whole
        int damage = base_damage;                                // default: a normal hit

        // TODO(stage 01): switch on `roll` to set `damage` AND print the start of the swing
        // line (no newline yet — the " for N damage." below finishes it):
        //   case 1  -> a FUMBLE:   damage = 0,               print: <enemy> rolls 1 and FUMBLES
        //   case 20 -> a CRITICAL: damage = base_damage * 2, print: <enemy> rolls 20 — CRITICAL HIT
        //   default -> a normal hit: damage = base_damage,   print: <enemy> rolls <roll> and hits
        // Then delete this placeholder line:
        std::cout << enemy << " swings";

        hero_hp -= damage;
        total_damage_taken += damage;
        std::cout << " for " << damage << " damage. ";

        // TODO(stage 01): react to the hero's remaining HP with if / else:
        //   if hero_hp <= 0 -> clamp hero_hp to 0, set hero_alive = false,
        //                      print: Hero HP: 0 — the hero falls!
        //   else            -> ++enemies_defeated, print: Hero HP: <hero_hp>
        // Then delete this placeholder newline:
        std::cout << "\n";
    }

    // --- Summary (given) -------------------------------------------------------------------
    std::cout << "\n== Result ==\n";
    std::cout << "Enemies defeated : " << enemies_defeated << " / " << enemy_count << "\n";
    std::cout << "Total damage taken: " << total_damage_taken << "\n";

    // A double average: static_cast avoids integer division so the fraction survives.
    // Guard against divide-by-zero when no enemy landed a blow.
    if (enemies_defeated + (hero_alive ? 0 : 1) > 0) {
        int rounds_fought = enemies_defeated + (hero_alive ? 0 : 1);
        double avg = static_cast<double>(total_damage_taken) / rounds_fought;
        std::cout << "Avg damage / round: " << avg << "\n";
    }

    // if for the headline verdict.
    if (hero_alive) {
        std::cout << "The hero cleared the gauntlet with " << hero_hp << " HP to spare!\n";
    } else {
        std::cout << "The hero was defeated. Better luck next seed.\n";
    }

    return 0;
}
