// main.cpp — the mini-project driver.  (YOUR TASK)
//
// This is the STARTER — the same program you split in stage 04, now built by CMake. Implement
// it yourself: fill in main() here AND the bodies in mathutils.cpp / player.cpp, then let the
// CMakeLists.txt one level up compile the three translation units and link them for you.
//
// A complete reference is in ../solution/ — try it yourself first, then compare.
//
// GOAL — once every TODO is done, the program should print exactly this:
//
//   === Stage 05 — the stage-04 program, built with CMake ===
//
//   Party:
//     Aria the Bold (hp 8)
//
//   A +40 potion would clamp HP to 30 (max is 30, so the overheal is discarded).
//
//   Healing 8 -> 30 over 5 frames (lerp):
//     frame 0: hp = 8
//     ...
//     frame 5: hp = 30
//
//   Distance to chest: 5
//
// Build & run (from the project/ folder):
//   cmake -S . -B build
//   cmake --build build
//   ./build/mathgame
#include <iostream>

#include "mathutils.hpp"
#include "player.hpp"

int main() {
    // TODO(stage 05): print the banner "=== Stage 05 — the stage-04 program, built with CMake ===\n\n".

    // TODO(stage 05): construct the shared Player `hero` ("Aria the Bold", hp 8) and a
    // `const int max_hp = 30;`. Print "Party:\n", then call print_player(hero).

    // TODO(stage 05): clamp — a +40 potion heals, but HP can never exceed max_hp. Use
    // clamp(hero.hp + potion, 0, max_hp) (cast to/from float) and report the clamped value.

    // TODO(stage 05): lerp — ease a healing animation from hero.hp up to max_hp over 5
    // frames. Loop frame = 0..5, t = frame / 5.0f, print lerp(hero.hp, max_hp, t) each frame.

    // TODO(stage 05): distance — print distance((2,1), (5,5)) using the distance() function.

    return 0;  // success — leave this last
}
