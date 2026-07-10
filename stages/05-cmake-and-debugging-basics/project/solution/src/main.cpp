// main.cpp — the mini-project driver.  (REFERENCE SOLUTION)
//
// This is the complete reference implementation. The starter you fill in is two levels up
// at ../../src/ (project/src/). Try it yourself there first, then compare with this.
//
// It ties together THREE headers over FOUR translation units:
//   - mathutils.hpp / mathutils.cpp  (clamp / lerp / distance)
//   - player.hpp    / player.cpp     (the shared Player type + print_player)
//   - this file                      (main)
//
// This is the same program you split by hand in stage 04 — but now CMake compiles the three
// translation units and links them for you (see the CMakeLists.txt one level up).
//
// Build & run this reference (from the project/solution/ folder):
//   cmake -S . -B build
//   cmake --build build
//   ./build/mathgame
#include <iostream>

#include "mathutils.hpp"
#include "player.hpp"

int main() {
    std::cout << "=== Stage 05 — the stage-04 program, built with CMake ===\n\n";

    // --- The shared Player type: constructed here in main, printed by player.cpp. ---
    Player hero{"Aria the Bold", 8};   // wounded: 8 out of a possible 30 HP
    const int max_hp = 30;

    std::cout << "Party:\n";
    print_player(hero);

    // clamp: a big potion heals +40, but HP can never exceed max_hp (no overheal).
    const int potion = 40;
    const int healed = static_cast<int>(
        clamp(static_cast<float>(hero.hp + potion), 0.0f, static_cast<float>(max_hp)));
    std::cout << "\nA +" << potion << " potion would clamp HP to " << healed << " (max is "
              << max_hp << ", so the overheal is discarded).\n";

    // lerp: ease a healing animation from current HP up to full over 5 frames.
    std::cout << "\nHealing " << hero.hp << " -> " << max_hp << " over 5 frames (lerp):\n";
    for (int frame = 0; frame <= 5; ++frame) {
        const float t = static_cast<float>(frame) / 5.0f;
        const float hp = lerp(static_cast<float>(hero.hp), static_cast<float>(max_hp), t);
        std::cout << "  frame " << frame << ": hp = " << hp << "\n";
    }

    // distance: how far is the hero (2, 1) from a treasure chest at (5, 5)?
    const float hero_x = 2.0f;
    const float hero_y = 1.0f;
    const float chest_x = 5.0f;
    const float chest_y = 5.0f;
    std::cout << "\nDistance to chest: " << distance(hero_x, hero_y, chest_x, chest_y) << "\n";

    return 0;
}
