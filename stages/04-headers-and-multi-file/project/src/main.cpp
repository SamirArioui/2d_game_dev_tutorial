// main.cpp — the mini-project driver.
//
// It ties together THREE headers over FOUR translation units:
//   - mathutils.hpp / mathutils.cpp  (clamp / lerp / distance)
//   - player.hpp    / player.cpp     (the shared Player type + print_player)
//   - this file                      (main)
//
// main.cpp only #includes the two HEADERS (declarations). The matching DEFINITIONS are
// supplied by mathutils.cpp and player.cpp, and stitched in by the LINKER. That is the
// whole point of the stage: compile each .cpp separately, then link the object files.
//
// Build (compile all translation units, then link them into one program):
//   clang++ -std=c++17 -Wall -Wextra src/main.cpp src/mathutils.cpp src/player.cpp -o mathgame
//   ./mathgame
#include <iostream>

#include "mathutils.hpp"
#include "player.hpp"

int main() {
    std::cout << "=== Stage 04 — multi-file math demo ===\n\n";

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
