// ============================================================================
// Stage 01 mini-project — Dice-roll battle simulator
// ============================================================================
//
// A lone hero runs a gauntlet against a fixed roster of enemies. Each enemy takes
// one swing; a d20 roll decides how the swing lands. The hero fights until the
// roster is cleared or their HP hits zero.
//
// This ONE program exercises every concept from stage 01:
//   - static typing & the numeric types  int / float / double / bool
//   - const                              (values that must not change)
//   - static_cast                        (explicit, visible type conversions)
//   - std::array + range-for             (a fixed roster, looped without indices)
//   - auto                               (let the compiler deduce a roll's type)
//   - loops                              (a classic for AND a range-for)
//   - if / switch                        (branch on state and on the roll)
//   - <random>                           (std::mt19937 + a uniform distribution)
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
    // --- Tuning constants: const says "these never change while the game runs". ----------
    const int hero_max_hp = 55;         // int   — whole hit points
    const float damage_scale = 0.9f;    // float — turns a raw roll into a damage number
    const int enemy_count = 5;

    // --- The roster: a FIXED-SIZE std::array. Its size (5) is baked into the type. --------
    std::array<std::string, 5> enemies = {"Goblin", "Orc", "Slime", "Bandit", "Warg"};

    // --- Mutable game state -------------------------------------------------------------
    int hero_hp = hero_max_hp;   // this DOES change, so it is not const
    bool hero_alive = true;      // bool — a plain yes/no flag
    int enemies_defeated = 0;
    int total_damage_taken = 0;

    // --- Randomness: one seeded engine + one distribution shaping it into d20 rolls. -----
    // A fixed seed makes every run identical (handy for a tutorial). Swap in
    // std::random_device{}() for genuine unpredictability in a real game.
    std::mt19937 rng(121);
    std::uniform_int_distribution<int> d20(1, 20);

    // --- Roster preview: a CLASSIC counter for-loop (index-based). -----------------------
    std::cout << "== The gauntlet ==\n";
    for (int i = 0; i < enemy_count; ++i) {
        std::cout << " " << (i + 1) << ". " << enemies[i] << "\n";
    }
    std::cout << "Hero HP: " << hero_hp << "\n\n";

    // --- The fight: a RANGE-FOR walks the roster without us managing an index. -----------
    for (const std::string& enemy : enemies) {
        if (!hero_alive) {
            break;  // stop early: no point fighting once the hero is down
        }

        // 'auto' asks the compiler to deduce the roll's type (int, from the distribution).
        auto roll = d20(rng);

        // A raw roll times a float scale is float arithmetic; static_cast<int> makes the
        // truncation to a whole "damage" number explicit and visible.
        int base_damage = static_cast<int>(roll * damage_scale);
        int damage = 0;

        // switch on the roll: two special faces, everything else is a normal hit.
        switch (roll) {
            case 1:
                // A fumble: the enemy trips over itself and deals nothing.
                damage = 0;
                std::cout << enemy << " rolls 1 and FUMBLES";
                break;
            case 20:
                // A critical: double the base damage.
                damage = base_damage * 2;
                std::cout << enemy << " rolls 20 — CRITICAL HIT";
                break;
            default:
                damage = base_damage;
                std::cout << enemy << " rolls " << roll << " and hits";
                break;
        }

        hero_hp -= damage;
        total_damage_taken += damage;
        std::cout << " for " << damage << " damage. ";

        // if / else: react to the hero's remaining HP.
        if (hero_hp <= 0) {
            hero_hp = 0;
            hero_alive = false;
            std::cout << "Hero HP: 0 — the hero falls!\n";
        } else {
            ++enemies_defeated;   // survived this foe
            std::cout << "Hero HP: " << hero_hp << "\n";
        }
    }

    // --- Summary --------------------------------------------------------------------------
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
