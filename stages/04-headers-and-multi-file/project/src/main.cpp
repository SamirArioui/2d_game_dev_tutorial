// main.cpp — the mini-project driver.  (YOUR TASK)
//
// This is the STARTER. Implement it yourself using what the README taught: this file
// #includes only the two HEADERS (declarations); you write the DEFINITIONS in
// mathutils.cpp and player.cpp, and the LINKER stitches all four translation units
// together. That is the whole point of the stage: compile each .cpp separately, then link.
//
// A complete reference is in ../solution/src/ — try it yourself first, then compare.
//
// GOAL — once you fill in main() here AND the bodies in mathutils.cpp / player.cpp, the
// program should print exactly this:
//
//   === Stage 04 — multi-file math demo ===
//
//   Party:
//     Aria the Bold (hp 8)
//
//   A +40 potion would clamp HP to 30 (max is 30, so the overheal is discarded).
//
//   Healing 8 -> 30 over 5 frames (lerp):
//     frame 0: hp = 8
//     frame 1: hp = 12.4
//     frame 2: hp = 16.8
//     frame 3: hp = 21.2
//     frame 4: hp = 25.6
//     frame 5: hp = 30
//
//   Distance to chest: 5
//
// Build (compile all translation units, then link them into one program). List every .cpp;
// headers are NOT listed (they're #included):
//   clang++ -std=c++17 -Wall -Wextra src/main.cpp src/mathutils.cpp src/player.cpp -o mathgame
//   ./mathgame
#include <iostream>

#include "mathutils.hpp"
#include "player.hpp"

int main() {
    // TODO(stage 04): print the banner line "=== Stage 04 — multi-file math demo ===\n\n".

    // TODO(stage 04): construct the shared Player `hero` ("Aria the Bold", hp 8) and a
    // `const int max_hp = 30;`. Print "Party:\n", then call print_player(hero).

    // TODO(stage 04): clamp — a +40 potion heals, but HP can never exceed max_hp. Use
    // clamp(hero.hp + potion, 0, max_hp) (cast to/from float) and report the clamped value.

    // TODO(stage 04): lerp — ease a healing animation from hero.hp up to max_hp over 5
    // frames. Loop frame = 0..5, t = frame / 5.0f, print lerp(hero.hp, max_hp, t) each frame.

    // TODO(stage 04): distance — print distance((2,1), (5,5)) using the distance() function.

    return 0;  // success — leave this last
}
